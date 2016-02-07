#include "dotsceneloader.hh"
//#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainMaterialGeneratorA.h>

#include <OgreParticleSystemManager.h>

#include <OgreMeshManager.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>

//#include "PagedGeometry/PagedGeometry.h"
//#include "PagedGeometry/GrassLoader.h"
//#include "PagedGeometry/BatchPage.h"
//#include "PagedGeometry/ImpostorPage.h"
//#include "PagedGeometry/TreeLoader3D.h"

namespace Shackle
{

Ogre::TerrainGroup *StaticGroupPtr = 0;
/*
Ogre::Real OgitorTerrainGroupHeightFunction(Ogre::Real x, Ogre::Real z, void *userData)
{
    return StaticGroupPtr->getHeightAtWorldPosition(x,0,z);
}
*/
DotSceneLoader::DotSceneLoader() :
//mGrassLoaderHandle(0),
    mSceneMgr(0),
    mTerrainGroup(0),
    resLocationsBaseDir(""),
    first_(true),
    nodes_ (0)
{}


DotSceneLoader::~DotSceneLoader()
{}

void DotSceneLoader::init()
{
    Ogre::LogManager::getSingleton().logMessage
    ( "[DotSceneLoader] Initializing" );
    mTerrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
}

void DotSceneLoader::cleanup()
{
    Ogre::LogManager::getSingleton().logMessage
    ( "[DotSceneLoader] Destroying everything" );
    /*
    if(mGrassLoaderHandle)
        delete mGrassLoaderHandle;

    std::vector<Forests::PagedGeometry *>::iterator it = mPGHandles.begin();
    while(it != mPGHandles.end())
    {
        delete it[0];
        it++;
    }
    mPGHandles.clear();
    */
    if(mTerrainGroup)
    {
        OGRE_DELETE mTerrainGroup;
    }

    OGRE_DELETE mTerrainGlobalOptions;
}
/*
void ParseStringVector(Ogre::String &str, Ogre::StringVector &list)
{
    list.clear();
    Ogre::StringUtil::trim(str,true,true);
    if(str == "")
        return;

    int pos = str.find(";");
    while(pos != -1)
    {
        list.push_back(str.substr(0,pos));
        str.erase(0,pos + 1);
        pos = str.find(";");
    }

    if(str != "")
        list.push_back(str);
}

*/

void DotSceneLoader::parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName,
                                   Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode,
                                   const Ogre::String &sPrependNode)
{
    // set up shared object values
    m_sGroupName = groupName;
    mSceneMgr = yourSceneMgr;
    m_sPrependNode = sPrependNode;
    staticObjects.clear();
    dynamicObjects.clear();

    Ogre::LogManager::getSingleton().logMessage
    ( "[DotSceneLoader] Scene parsing started, ResGroup: " + m_sGroupName );


    // if the resource group doesn't exists create it
    if(!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(m_sGroupName))
    {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup(m_sGroupName);
    }

    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource
                                 ( SceneName, groupName );
    char* scene = strdup(stream->getAsString().c_str());
    dotscene_.parse(scene);

    //
    // Grab the scene node and Validate the File
    if( !dotscene_.pushChild("scene") || dotscene_.getAttrib("formatVersion", "") == "")
    {
        Ogre::LogManager::getSingleton().logMessage
        ( "[DotSceneLoader] Error: Invalid .scene File. Missing <scene>" );
        delete scene;
        return;
    }

    // figure out where to attach any nodes we create
    if(!pAttachNode)
    {
        mAttachNode = mSceneMgr->getRootSceneNode();
    }
    else
    {
        mAttachNode = pAttachNode;
    }

    // Process the scene
    processScene();

    delete scene;

    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Scene parsing finished" );
}

void DotSceneLoader::processScene()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Scene" );
    // Process the scene parameters
    Ogre::String version = dotscene_.getAttrib("formatVersion", "unknown");

    Ogre::String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
    /*
    if(XMLRoot->first_attribute("ID"))
        message += ", id " + Ogre::String(XMLRoot->first_attribute("ID")->value());
    if(XMLRoot->first_attribute("sceneManager"))
        message += ", scene manager " + Ogre::String(XMLRoot->first_attribute("sceneManager")->value());
    if(XMLRoot->first_attribute("minOgreVersion"))
        message += ", min. Ogre version " + Ogre::String(XMLRoot->first_attribute("minOgreVersion")->value());
    if(XMLRoot->first_attribute("author"))
        message += ", author " + Ogre::String(XMLRoot->first_attribute("author")->value());
    Ogre::LogManager::getSingleton().logMessage(message);
    */
    // Process resources (?)
    if(dotscene_.pushChild("resourceLocations"))
    {
        processResourceLocations();
        dotscene_.popNode();
    }
    // Process environment (?)
    if(dotscene_.pushChild("environment"))
    {
        processEnvironment();
        dotscene_.popNode();
    }
    // Process lights (?)
    while(multipleNodes("light"))
    {
        processLight();
    }
    // Process camera (?)
    while(multipleNodes("camera"))
    {
        processCamera();
    }
    // Process terrain (?)
    if(dotscene_.pushChild("terrain"))
    {
        processTerrain();
        dotscene_.popNode();
    }
    // Process nodes (?)
    if(dotscene_.pushChild("nodes"))
    {
        processNodes();
        dotscene_.popNode();
    }
    // Process externals (?)
    if(dotscene_.pushChild("externals"))
    {
        processExternals();
        dotscene_.popNode();
    }
    // Process userDataReference (?)
    if(dotscene_.pushChild("userDataReference"))
    {
        processUserDataReference();
        dotscene_.popNode();
    }
    // Process octree (?)
    if(dotscene_.pushChild("octree"))
    {
        processOctree();
        dotscene_.popNode();
    }
}

void DotSceneLoader::processResourceLocations()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Resource Locations" );
    //rapidxml::xml_node<>* pElement;

    // Process resources (?)
    std::vector<Ogre::String> locations;
    while(dotscene_.nextNode("resourceLocation"))
    {
        locations.push_back(dotscene_.getAttrib("name"));
    }
    processor_.processResourceLocations(locations);
}

void DotSceneLoader::processNodes()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Nodes" );

    // Process nodes

    while(multipleNodes("node"))
    {
        processNode();
    }

// Process position (?)
    if(dotscene_.pushChild("position"))
    {
        mAttachNode->setPosition(dotscene_.parseVector3());
        mAttachNode->setInitialState();
        dotscene_.popNode();
    }

// Process rotation (?)
    if(dotscene_.pushChild("rotation"))
    {
        mAttachNode->setOrientation(dotscene_.parseQuaternion());
        mAttachNode->setInitialState();
        dotscene_.popNode();
    }

// Process scale (?)
    if(dotscene_.pushChild("scale"))
    {
        mAttachNode->setScale(dotscene_.parseVector3());
        mAttachNode->setInitialState();
        dotscene_.popNode();
    }
}

void DotSceneLoader::processExternals()
{
    //! @todo Implement this
}

void DotSceneLoader::processEnvironment()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Environment" );


    // Process camera (?)
    if(dotscene_.pushChild("camera"))
    {
        processCamera();
        dotscene_.popNode();
    }

    // Process fog (?)
    if(dotscene_.pushChild("fog"))
    {
        processFog();
        dotscene_.popNode();
    }

    // Process skyBox (?)
    if(dotscene_.pushChild("skyBox"))
    {
        processSkyBox();
        dotscene_.popNode();
    }

    // Process skyDome (?)
    if(dotscene_.pushChild("skyDome"))
    {
        processSkyDome();
        dotscene_.popNode();
    }

    // Process skyPlane (?)
    if(dotscene_.pushChild("skyPlane"))
    {
        processSkyPlane();
        dotscene_.popNode();
    }

    // Process clipping (?)
    if(dotscene_.pushChild("clipping"))
    {
        processClipping();
        dotscene_.popNode();
    }

    // Process colourAmbient (?)
    if(dotscene_.pushChild("colourAmbient"))
    {
        mSceneMgr->setAmbientLight(dotscene_.parseColour());
        dotscene_.popNode();
    }

    // Process colourBackground (?)
    //! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
    if(dotscene_.pushChild("colourBackground"))
    {

        //mSceneMgr->set(dotscene_.parseColour());
        dotscene_.popNode();
    }

    // Process userDataReference (?)
    if(dotscene_.pushChild("userDataReference"))
    {
        processUserDataReference();
        dotscene_.popNode();
    }
}

void DotSceneLoader::processTerrain()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Terrain" );

    Ogre::Real worldSize = dotscene_.getAttribReal("worldSize");
    int mapSize = dotscene_.getAttribInt("mapSize");
    //bool colourmapEnabled = getAttribBool(XMLNode, "colourmapEnabled");
    //int colourMapTextureSize = Ogre::StringConverter::dotscene_.parseInt(XMLNode->first_attribute("colourMapTextureSize")->value());
    int compositeMapDistance = dotscene_.getAttribInt("tuningCompositeMapDistance");
    int maxPixelError = dotscene_.getAttribInt("tuningMaxPixelError");

    Ogre::Vector3 lightdir(0, -0.3, 0.75);
    lightdir.normalise();
    Ogre::Light* l = mSceneMgr->createLight("tstLight");
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(Ogre::ColourValue(1.0, 1.0, 1.0));
    l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
//    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));

    mTerrainGlobalOptions->setMaxPixelError((Ogre::Real)maxPixelError);
    mTerrainGlobalOptions->setCompositeMapDistance((Ogre::Real)compositeMapDistance);
    mTerrainGlobalOptions->setLightMapDirection(lightdir);
    mTerrainGlobalOptions->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobalOptions->setCompositeMapDiffuse(l->getDiffuseColour());

    mSceneMgr->destroyLight("tstLight");

    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr,
                    Ogre::Terrain::ALIGN_X_Z,
                    mapSize, worldSize);

    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    mTerrainGroup->setResourceGroup(m_sGroupName);

    // Process terrain pages (*)
    if(dotscene_.pushChild("terrainPages"))
    {
        while(multipleNodes("terrainPage"))
        {
            processTerrainPage();
        }
        dotscene_.popNode();
    }
    mTerrainGroup->loadAllTerrains(true);

    mTerrainGroup->freeTemporaryResources();
    //mTerrain->setPosition(mTerrainPosition);
}

void DotSceneLoader::processTerrainPage()
{
    Ogre::String name = dotscene_.getAttrib("name");

    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Terrain Page: " + name );

    int pageX = dotscene_.getAttribInt("pageX");
    int pageY = dotscene_.getAttribInt("pageY");
    //mPGPageSize       = Ogre::StringConverter::parseInt(XMLNode->first_attribute("pagedGeometryPageSize")->value());
    //mPGDetailDistance = Ogre::StringConverter::parseInt(XMLNode->first_attribute("pagedGeometryDetailDistance")->value());
    // error checking
    /*
    if(mPGPageSize < 10){
        Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] pagedGeometryPageSize value error!", Ogre::LML_CRITICAL);
        mPGPageSize = 10;
    }
    if(mPGDetailDistance < 100){
        Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] pagedGeometryDetailDistance value error!", Ogre::LML_CRITICAL);
        mPGDetailDistance = 100;
    }*/

    if (Ogre::ResourceGroupManager::getSingleton().resourceExists
            (mTerrainGroup->getResourceGroup(), name))
    {
        mTerrainGroup->defineTerrain(pageX, pageY, name);
        dotscene_.popNode();
    }
    // grass layers
    if(dotscene_.pushChild("grassLayers"))
    {
        processGrassLayers();
        dotscene_.popNode();
    }
}

void DotSceneLoader::processGrassLayers()
{
    /*
       Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Grass Layers" );

       Ogre::String dMapName = getAttrib(XMLNode, "densityMap");
       mTerrainGlobalOptions->setVisibilityFlags(Ogre::StringConverter::parseUnsignedInt(XMLNode->first_attribute("visibilityFlags")->value()));

       // create a temporary camera
       Ogre::Camera* tempCam = mSceneMgr->createCamera("ThIsNamEShoUlDnOtExisT");

       // create paged geometry what the grass will use
       Forests::PagedGeometry * mPGHandle = new PagedGeometry(tempCam, mPGPageSize);
       mPGHandle->addDetailLevel<GrassPage>(mPGDetailDistance);

       //Create a GrassLoader object
       mGrassLoaderHandle = new GrassLoader(mPGHandle);

       //Assign the "grassLoader" to be used to load geometry for the PagedGrass instance
       mPGHandle->setPageLoader(mGrassLoaderHandle);

       // set the terrain group pointer
       StaticGroupPtr = mTerrainGroup;

       //Supply a height function to GrassLoader so it can calculate grass Y values
       mGrassLoaderHandle->setHeightFunction(OgitorTerrainGroupHeightFunction);

       // push the page geometry handle into the PGHandles array
       mPGHandles.push_back(mPGHandle);

       // create the layers and load the options for them
       rapidxml::xml_node<>* pElement = XMLNode->first_node("grassLayer");
       rapidxml::xml_node<>* pSubElement;
       //Forests::GrassLayer* gLayer;
       Ogre::String tempStr;
       while(pElement)
       {
           // grassLayer
           //gLayer = mGrassLoaderHandle->addLayer(pElement->first_attribute("material")->value());
           //gLayer->setMaxSlope(Ogre::StringConverter::parseReal(pElement->first_attribute("maxSlope")->value()));
           //gLayer->setLightingEnabled(Ogre::StringConverter::parseBool(pElement->first_attribute("lighting")->value()));

           // densityMapProps
           pSubElement = pElement->first_node("densityMapProps");
           tempStr = pSubElement->first_attribute("channel")->value();
           MapChannel mapCh;
           if(!tempStr.compare("ALPHA")) mapCh = CHANNEL_ALPHA; else
           if(!tempStr.compare("BLUE"))  mapCh = CHANNEL_BLUE;  else
           if(!tempStr.compare("COLOR")) mapCh = CHANNEL_COLOR; else
           if(!tempStr.compare("GREEN")) mapCh = CHANNEL_GREEN; else
           if(!tempStr.compare("RED"))   mapCh = CHANNEL_RED;

           gLayer->setDensityMap(dMapName, mapCh);
           gLayer->setDensity(Ogre::StringConverter::parseReal(pSubElement->first_attribute("density")->value()));

           // mapBounds
           pSubElement = pElement->first_node("mapBounds");
           gLayer->setMapBounds( TBounds(
                           Ogre::StringConverter::parseReal(pSubElement->first_attribute("left")->value()),  // left
                           Ogre::StringConverter::parseReal(pSubElement->first_attribute("top")->value()),   // top
                           Ogre::StringConverter::parseReal(pSubElement->first_attribute("right")->value()), // right
                           Ogre::StringConverter::parseReal(pSubElement->first_attribute("bottom")->value()) // bottom
                                   )
                               );

           // grassSizes
           pSubElement = pElement->first_node("grassSizes");
           gLayer->setMinimumSize( Ogre::StringConverter::parseReal(pSubElement->first_attribute("minWidth")->value()),   // width
                                   Ogre::StringConverter::parseReal(pSubElement->first_attribute("minHeight")->value()) );// height
           gLayer->setMaximumSize( Ogre::StringConverter::parseReal(pSubElement->first_attribute("maxWidth")->value()),   // width
                                   Ogre::StringConverter::parseReal(pSubElement->first_attribute("maxHeight")->value()) );// height

           // techniques
           pSubElement = pElement->first_node("techniques");
           tempStr = pSubElement->first_attribute("renderTechnique")->value();
           GrassTechnique rendTech;

           if(!tempStr.compare("QUAD"))       rendTech = GRASSTECH_QUAD;       else
           if(!tempStr.compare("CROSSQUADS")) rendTech = GRASSTECH_CROSSQUADS; else
           if(!tempStr.compare("SPRITE"))     rendTech = GRASSTECH_SPRITE;
           gLayer->setRenderTechnique( rendTech,
                                       Ogre::StringConverter::parseBool(pSubElement->first_attribute("blend")->value()) );

           tempStr = pSubElement->first_attribute("fadeTechnique")->value();
           FadeTechnique fadeTech;
           if(!tempStr.compare("ALPHA")) fadeTech = FADETECH_ALPHA; else
           if(!tempStr.compare("GROW"))  fadeTech = FADETECH_GROW; else
           if(!tempStr.compare("ALPHAGROW")) fadeTech = FADETECH_ALPHAGROW;
           gLayer->setFadeTechnique(fadeTech);

           // animation
           pSubElement = pElement->first_node("animation");
           gLayer->setAnimationEnabled(Ogre::StringConverter::parseBool(pSubElement->first_attribute("animate")->value()));
           gLayer->setSwayLength(Ogre::StringConverter::parseReal(pSubElement->first_attribute("swayLength")->value()));
           gLayer->setSwaySpeed(Ogre::StringConverter::parseReal(pSubElement->first_attribute("swaySpeed")->value()));
           gLayer->setSwayDistribution(Ogre::StringConverter::parseReal(pSubElement->first_attribute("swayDistribution")->value()));

           // next layer
           pElement = pElement->next_sibling("grassLayer");


       }

       mSceneMgr->destroyCamera(tempCam);
       */
}

void DotSceneLoader::processUserDataReference(Ogre::SceneNode *pParent)
{
    //! @todo Implement this
}

void DotSceneLoader::processOctree()
{
    //! @todo Implement this
}

void DotSceneLoader::processLight(Ogre::SceneNode *pParent)
{
    // Process attributes
    Ogre::String name = dotscene_.getAttrib("name");
    Ogre::String id = dotscene_.getAttrib("id");

    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Light: " + name );

    // Create the light
    Ogre::Light *pLight = mSceneMgr->createLight(name);
    if(pParent)
    {
        pParent->attachObject(pLight);
        dotscene_.popNode();
    }

    Ogre::String sValue = dotscene_.getAttrib("type");
    if(sValue == "point")
        pLight->setType(Ogre::Light::LT_POINT);
    else if(sValue == "directional")
        pLight->setType(Ogre::Light::LT_DIRECTIONAL);
    else if(sValue == "spot")
        pLight->setType(Ogre::Light::LT_SPOTLIGHT);
    else if(sValue == "radPoint")
        pLight->setType(Ogre::Light::LT_POINT);

    pLight->setVisible(dotscene_.getAttribBool("visible", true));
    pLight->setCastShadows(dotscene_.getAttribBool("castShadows", true));

    // Process position (?)
    if(dotscene_.pushChild("position"))
    {
        pLight->setPosition(dotscene_.parseVector3());
        dotscene_.popNode();
    }
    // Process normal (?)
    if(dotscene_.pushChild("normal"))
    {
        pLight->setDirection(dotscene_.parseVector3());
        dotscene_.popNode();
    }

    if(dotscene_.pushChild("direction"))
    {
        pLight->setDirection(dotscene_.parseVector3());
        mLightDirection = dotscene_.parseVector3();
        dotscene_.popNode();
    }

    // Process colourDiffuse (?)
    if(dotscene_.pushChild("colourDiffuse"))
    {
        pLight->setDiffuseColour(dotscene_.parseColour());
        dotscene_.popNode();
    }

    // Process colourSpecular (?)
    if(dotscene_.pushChild("colourSpecular"))
    {
        pLight->setSpecularColour(dotscene_.parseColour());
        dotscene_.popNode();
    }

    if(sValue != "directional")
    {
        // Process lightRange (?)
        if(dotscene_.pushChild("lightRange"))
        {
            processLightRange(pLight);
            dotscene_.popNode();
        }

        // Process lightAttenuation (?)
        if(dotscene_.pushChild("lightAttenuation"))
        {
            processLightAttenuation(pLight);
            dotscene_.popNode();
        }
    }
    // Process userDataReference (?)
    if(dotscene_.pushChild("userDataReference"))
    {
        //processUserDataReference(pLight);
        dotscene_.popNode();
    }
}

void DotSceneLoader::processCamera(Ogre::SceneNode *pParent)
{
    // Process attributes
    Ogre::String name = dotscene_.getAttrib("name");
    Ogre::String id = dotscene_.getAttrib("id");
    Ogre::Real fov = dotscene_.getAttribReal("fov", 45);
    Ogre::Real aspectRatio = dotscene_.getAttribReal("aspectRatio", 1.3333);
    Ogre::String projectionType = dotscene_.getAttrib("projectionType", "perspective");

    Ogre::LogManager::getSingleton().logMessage
    ( "[DotSceneLoader] Process Camera: " + name );

    // Create the camera
    Ogre::Camera *pCamera = mSceneMgr->createCamera(name);

    // set auto Aspest ratio true. If later aspect ratio comes this can be deleted
    pCamera->setAutoAspectRatio(true);

    //TODO: make a flag or attribute indicating whether or not the camera should be attached to any parent node.
    if(pParent)
        pParent->attachObject(pCamera);

    // Set the field-of-view
    //! @todo Is this always in degrees?
    pCamera->setFOVy(Ogre::Radian(fov));

    // Set the aspect ratio
    pCamera->setAutoAspectRatio(false);
    pCamera->setAspectRatio(aspectRatio);

    // Set the projection type
    if(projectionType == "perspective")
    {
        pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
    }
    else if(projectionType == "orthographic")
    {
        pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    }

    // Process clipping (?)
    if(dotscene_.pushChild("clipping"))
    {
        Ogre::Real nearDist = dotscene_.getAttribReal("near");
        pCamera->setNearClipDistance(nearDist);

        Ogre::Real farDist =  dotscene_.getAttribReal("far");
        pCamera->setFarClipDistance(farDist);

        dotscene_.popNode();
    }

    // Process position (?)
    if(dotscene_.pushChild("position"))
    {
        pCamera->setPosition(dotscene_.parseVector3());
        dotscene_.popNode();
    }

    // Process rotation (?)
    if(dotscene_.pushChild("rotation"))
    {
        pCamera->setOrientation(dotscene_.parseQuaternion());
        dotscene_.popNode();
    }

    // Process normal (?)
    if(dotscene_.pushChild("normal"))
    {
        //!< @todo What to do with this element?
        dotscene_.popNode();
    }

    // Process lookTarget (?)
    if(dotscene_.pushChild("lookTarget"))
    {
        //!< @todo Implement the camera look target
        dotscene_.popNode();
    }

    // Process trackTarget (?)
    if(dotscene_.pushChild("trackTarget"))
    {
        //!< @todo Implement the camera track target
        dotscene_.popNode();
    }

    // Process userDataReference (?)
    if(dotscene_.pushChild("userDataReference"))
    {
        //!< @todo Implement the camera user data reference
        dotscene_.popNode();
    }
    /*
        // construct a scenenode is no parent
        if(!pParent)
        {
            Ogre::SceneNode* pNode = mAttachNode->createChildSceneNode(name);
            pNode->setPosition(pCamera->getPosition());
            pNode->setOrientation(pCamera->getOrientation());
            pNode->scale(1,1,1);
        }
    */
}

void DotSceneLoader::processNode(Ogre::SceneNode *pParent)
{
    // Construct the node's name
    Ogre::String name = m_sPrependNode + dotscene_.getAttrib("name");

    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Node: " + name );

    // Create the scene node
    Ogre::SceneNode *pNode;

    if(name.empty())
    {
        // Let Ogre choose the name
        if(pParent)
            pNode = pParent->createChildSceneNode();
        else
            pNode = mAttachNode->createChildSceneNode();
    }
    else
    {
        // Provide the name
        if(pParent)
            pNode = pParent->createChildSceneNode(name);
        else
            pNode = mAttachNode->createChildSceneNode(name);
    }

    // Process other attributes
    Ogre::String id = dotscene_.getAttrib("id");
    //bool isTarget = getAttribBool("isTarget");

    // Process position (?)
    if(dotscene_.pushChild("position"))
    {
        pNode->setPosition(dotscene_.parseVector3());
        pNode->setInitialState();
        dotscene_.popNode();
    }

    // Process rotation (?)
    if(dotscene_.pushChild("rotation"))
    {
        pNode->setOrientation(dotscene_.parseQuaternion());
        pNode->setInitialState();
        dotscene_.popNode();
    }

    // Process scale (?)
    if(dotscene_.pushChild("scale"))
    {
        pNode->setScale(dotscene_.parseVector3());
        pNode->setInitialState();
        dotscene_.popNode();
    }

    // Process lookTarget (?)
    if(dotscene_.pushChild("lookTarget"))
    {
        processLookTarget(pNode);
        dotscene_.popNode();
    }

    // Process trackTarget (?)
    if(dotscene_.pushChild("trackTarget"))
    {
        processTrackTarget(pNode);
        dotscene_.popNode();
    }
    // Process node (*)
    if(dotscene_.pushChild("node"))
    {
        unsigned int pops = 1;
        processNode( pNode);
        while(dotscene_.pushSibling("node"))
        {
            processNode( pNode);
            ++pops;
            //pElement = pElement->next_sibling("node");
        }
        for(unsigned int i = 0; i < pops; ++i)
        {
            dotscene_.popNode();
        }
    }
    // Process entity (*)
    while(multipleNodes("entity"))
    {
        processEntity(pNode);
        //pElement = pElement->next_sibling("entity");
    }
    // Process light (*)
    while(multipleNodes("light"))
    {
        processLight(pNode);
        //pElement = pElement->next_sibling("light");
    }
    // Process camera (*)
    while(multipleNodes("camera"))
    {
        processCamera(pNode);
        //pElement = pElement->next_sibling("camera");
    }
    // Process particleSystem (*)
    while(multipleNodes("particleSystem"))
    {
        processParticleSystem(pNode);
        //pElement = pElement->next_sibling("particleSystem");
    }
    // Process billboardSet (*)
    while(multipleNodes("billboardSet"))
    {
        processBillboardSet(pNode);
        //pElement = pElement->next_sibling("billboardSet");
    }
    // Process plane (*)
    while(multipleNodes("plane"))
    {
        processPlane(pNode);
        //pElement = pElement->next_sibling("plane");
    }

    // Process userDataReference (?)
    if(dotscene_.pushChild("userDataReference"))
    {
        processUserDataReference(pNode);
        dotscene_.popNode();
    }

    // Process entity (*)
    while(multipleNodes("pagedgeometry"))
    {
        processPagedGeometry(pNode);
        //pElement = pElement->next_sibling("pagedgeometry");
    }
}

void DotSceneLoader::processLookTarget(Ogre::SceneNode *pParent)
{
    //! @todo Is this correct? Cause I don't have a clue actually

    // Process attributes
    Ogre::String nodeName = dotscene_.getAttrib("nodeName");

    Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT;
    Ogre::String sValue = dotscene_.getAttrib("relativeTo");
    if(sValue == "local")
        relativeTo = Ogre::Node::TS_LOCAL;
    else if(sValue == "parent")
        relativeTo = Ogre::Node::TS_PARENT;
    else if(sValue == "world")
        relativeTo = Ogre::Node::TS_WORLD;

    // Process position (?)
    Ogre::Vector3 position;
    if(dotscene_.pushChild("position"))
    {
        position = dotscene_.parseVector3();
        dotscene_.popNode();
    }

    // Process localDirection (?)
    Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
    if(dotscene_.pushChild("localDirection"))
    {
        localDirection = dotscene_.parseVector3();
        dotscene_.popNode();
    }

    // Setup the look target
    try
    {
        if(!nodeName.empty())
        {
            Ogre::SceneNode *pLookNode = mSceneMgr->getSceneNode(nodeName);
            position = pLookNode->_getDerivedPosition();
        }

        pParent->lookAt(position, relativeTo, localDirection);
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage
        ("[DotSceneLoader] Error processing a look target!");
    }
}

void DotSceneLoader::processTrackTarget(Ogre::SceneNode *pParent)
{
    // Process attributes
    Ogre::String nodeName = dotscene_.getAttrib("nodeName");

    // Process localDirection (?)
    Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
    if(dotscene_.pushChild("localDirection"))
    {
        localDirection = dotscene_.parseVector3();
    }

    // Process offset (?)
    Ogre::Vector3 offset = Ogre::Vector3::ZERO;
    if(dotscene_.pushChild("offset"))
    {
        offset = dotscene_.parseVector3();
    }

    // Setup the track target
    try
    {
        Ogre::SceneNode *pTrackNode = mSceneMgr->getSceneNode(nodeName);
        pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage
        ("[DotSceneLoader] Error processing a track target!");
    }
}

void DotSceneLoader::processEntity(Ogre::SceneNode *pParent)
{
    // Process attributes
    Ogre::String name = dotscene_.getAttrib("name");
    Ogre::String id = dotscene_.getAttrib("id");
    Ogre::String meshFile = dotscene_.getAttrib("meshFile");
    Ogre::String materialFile = dotscene_.getAttrib("materialFile");
    bool isStatic = dotscene_.getAttribBool("static", false);;
    bool castShadows = dotscene_.getAttribBool("castShadows", true);

    Ogre::LogManager::getSingleton().logMessage
    ( "[DotSceneLoader] Process Entity: " + name );

    // TEMP: Maintain a list of static and dynamic objects
    if(isStatic)
        staticObjects.push_back(name);
    else
        dynamicObjects.push_back(name);

    // Process vertexBuffer (?)
    if(dotscene_.pushChild("vertexBuffer"))
    {
        //processVertexBuffer(pElement);
        dotscene_.popNode();
    }

    // Process indexBuffer (?)
    if(dotscene_.pushChild("indexBuffer"))
    {
        //processIndexBuffer(pElement);
        dotscene_.popNode();
    }

    // Create the entity
    Ogre::Entity *pEntity = 0;
    try
    {
        Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
        pEntity = mSceneMgr->createEntity(name, meshFile);
        pEntity->setCastShadows(castShadows);
        pParent->attachObject(pEntity);

        if(!materialFile.empty())
            pEntity->setMaterialName(materialFile);

        // Process subentity (*)
        /* if materials defined within subentities, those
           materials will be used instead of the materialFile */

        if(dotscene_.pushChild("subentities"))
        {
            processSubEntity(pEntity);
            dotscene_.popNode();
        }
        else
        {
            // if the .scene file contains the subentites without
            // the <subentities> </subentities>
            processSubEntity(pEntity);
        }
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage
        ("[DotSceneLoader] Error loading an entity!");
    }

    // Process userDataReference (?)
    if(dotscene_.pushChild("userDataReference"))
    {
        processUserDataReference(pEntity);
        dotscene_.popNode();
    }


}

void DotSceneLoader::processSubEntity(Ogre::Entity *pEntity)
{
    int index = 0;
    Ogre::String materialName;

    // Process subentity

    while(multipleNodes("subentity"))
    {

        materialName.clear();

        index = dotscene_.getAttribInt("index", -1);				// submesh index
        materialName = dotscene_.getAttrib("materialName");	// new material for submesh

        if(index != -1 && !materialName.empty())
        {

            try
            {
                pEntity->getSubEntity(index)->setMaterialName(materialName);
            }
            catch (...)
            {
                Ogre::LogManager::getSingleton().logMessage
                ("[DotSceneLoader] Subentity material index invalid!");
            }
        }
        dotscene_.popNode();
    }
}

void DotSceneLoader::processParticleSystem(Ogre::SceneNode *pParent)
{
    // Process attributes
    Ogre::String name = dotscene_.getAttrib("name");
    Ogre::String id = dotscene_.getAttrib("id");
    Ogre::String scriptName = dotscene_.getAttrib("script");

    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process ParticleSystem: " + name );


    // Create the particle system
    try
    {
        Ogre::ParticleSystem *pParticles = mSceneMgr->createParticleSystem(name, scriptName);
        pParent->attachObject(pParticles);
        particleSystemList.push_back(pParticles);
        // there is a bug with particles and paged geometry if particle's
        // renderQueue is value is smaller than the grass's renderQueue
        //if(mGrassLoaderHandle)
        //pParticles->setRenderQueueGroup(mGrassLoaderHandle->getRenderQueueGroup());
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage
        ("[DotSceneLoader] Error creating a particle system!");
    }
}

void DotSceneLoader::processBillboardSet(Ogre::SceneNode *pParent)
{
    //! @todo Implement this
}

void DotSceneLoader::processPlane(Ogre::SceneNode *pParent)
{
    Ogre::String name = dotscene_.getAttrib("name");
    Ogre::Real distance = dotscene_.getAttribReal("distance");
    Ogre::Real width = dotscene_.getAttribReal("width");
    Ogre::Real height = dotscene_.getAttribReal("height");
    int xSegments = dotscene_.getAttribInt("xSegments");
    int ySegments = dotscene_.getAttribInt("ySegments");
    int numTexCoordSets = dotscene_.getAttribInt("numTexCoordSets");
    Ogre::Real uTile = dotscene_.getAttribReal("uTile");
    Ogre::Real vTile = dotscene_.getAttribReal("vTile");
    Ogre::String material = dotscene_.getAttrib("material");
    bool hasNormals = dotscene_.getAttribBool("hasNormals");
    Ogre::Vector3 normal;
    Ogre::Vector3 up;

    if(dotscene_.pushChild("normal"))
    {
        normal = dotscene_.parseVector3();
        dotscene_.popNode();
    }

    if(dotscene_.pushChild("upVector"))
    {
        up = dotscene_.parseVector3();
        dotscene_.popNode();
    }

    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Plane: " + name );

    Ogre::Plane plane(normal, distance);
    Ogre::MeshPtr res = Ogre::MeshManager::getSingletonPtr()->createPlane(
                            name + "mesh", "General", plane, width, height,
                            xSegments, ySegments, hasNormals, numTexCoordSets,
                            uTile, vTile, up);

    Ogre::Entity* ent = mSceneMgr->createEntity(name, name + "mesh");

    ent->setMaterialName(material);

    pParent->attachObject(ent);
}

struct PGInstanceInfo
{
    Ogre::Vector3 pos;
    Ogre::Real    scale;
    Ogre::Real    yaw;
};

typedef std::vector<PGInstanceInfo> PGInstanceList;

void DotSceneLoader::processPagedGeometry(Ogre::SceneNode *pParent)
{
    /*
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Paged Geometry" );

    Ogre::String filename = "../Projects/SampleScene3/" + getAttrib(XMLNode, "fileName");
    Ogre::String model = getAttrib(XMLNode, "model");
    Ogre::Real pagesize = getAttribReal(XMLNode, "pageSize");
    Ogre::Real batchdistance = getAttribReal(XMLNode, "batchDistance");
    Ogre::Real impostordistance = getAttribReal(XMLNode, "impostorDistance");
    Ogre::Vector4 bounds = Ogre::StringConverter::dotscene_.parseVector4(getAttrib(XMLNode, "bounds"));

    PagedGeometry *mPGHandle = new PagedGeometry();
    mPGHandle->setCamera(mSceneMgr->getCameraIterator().begin()->second);
    mPGHandle->setPageSize(pagesize);
    mPGHandle->setInfinite();

    mPGHandle->addDetailLevel<BatchPage>(batchdistance,0);
    mPGHandle->addDetailLevel<ImpostorPage>(impostordistance,0);

    TreeLoader3D *mHandle = new TreeLoader3D(mPGHandle, Forests::TBounds(bounds.x, bounds.y, bounds.z, bounds.w));
    mPGHandle->setPageLoader(mHandle);

    //mPGHandles.push_back(mPGHandle);
    //mTreeHandles.push_back(mHandle);

    std::ifstream stream(filename.c_str());

    if(!stream.is_open())
        return;

    Ogre::StringVector list;

    char res[128];

    PGInstanceList mInstanceList;

    while(!stream.eof())
    {
        stream.getline(res, 128);
        Ogre::String resStr(res);

        dotscene_.parseStringVector(resStr, list);

        if(list.size() == 3)
        {
            PGInstanceInfo info;

            info.pos = Ogre::StringConverter::dotscene_.parseVector3(list[0]);
            info.scale = Ogre::StringConverter::dotscene_.parseReal(list[1]);
            info.yaw = Ogre::StringConverter::dotscene_.parseReal(list[2]);

            mInstanceList.push_back(info);
        }
        else if(list.size() == 4)
        {
            PGInstanceInfo info;

            info.pos = Ogre::StringConverter::parseVector3(list[1]);
            info.scale = Ogre::StringConverter::parseReal(list[2]);
            info.yaw = Ogre::StringConverter::parseReal(list[3]);

            mInstanceList.push_back(info);
        }
    }

    stream.close();

    if(model != "")
    {
        Ogre::Entity *mEntityHandle = mSceneMgr->createEntity(model + ".mesh");

        PGInstanceList::iterator it = mInstanceList.begin();

        while(it != mInstanceList.end())
        {
            mHandle->addTree(mEntityHandle, it->pos, Ogre::Degree(it->yaw), it->scale);

            it++;
        }
    }
    */
}

void DotSceneLoader::processFog()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process Fog" );

    // Process attributes
    Ogre::Real expDensity = dotscene_.getAttribReal("density", 0.001);
    Ogre::Real linearStart = dotscene_.getAttribReal("start", 0.0);
    Ogre::Real linearEnd = dotscene_.getAttribReal("end", 1.0);

    Ogre::FogMode mode = Ogre::FOG_NONE;
    Ogre::String sMode = dotscene_.getAttrib("mode");
    if(sMode == "none")
        mode = Ogre::FOG_NONE;
    else if(sMode == "exp")
        mode = Ogre::FOG_EXP;
    else if(sMode == "exp2")
        mode = Ogre::FOG_EXP2;
    else if(sMode == "linear")
        mode = Ogre::FOG_LINEAR;
    else
        mode = (Ogre::FogMode)Ogre::StringConverter::parseInt(sMode);

    // Process colourDiffuse (?)
    Ogre::ColourValue colourDiffuse = Ogre::ColourValue::White;
    if(dotscene_.pushChild("colour"))
    {
        colourDiffuse = dotscene_.parseColour();
        dotscene_.popNode();
    }

    // Setup the fog
    mSceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}

void DotSceneLoader::processSkyBox()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process SkyBox" );

    // Process attributes
    Ogre::String material = dotscene_.getAttrib("material", "BaseWhite");
    Ogre::Real distance = dotscene_.getAttribReal("distance", 5000);
    bool drawFirst = dotscene_.getAttribBool("drawFirst", true);
    bool active = dotscene_.getAttribBool("active", false);
    if(!active)
        return;

    // Process rotation (?)
    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
    if(dotscene_.pushChild("rotation"))
    {
        rotation = dotscene_.parseQuaternion();
        dotscene_.popNode();
    }

    // Setup the sky box
    mSceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
}

void DotSceneLoader::processSkyDome()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process SkyDome" );

    // Process attributes
    Ogre::String material = dotscene_.getAttrib("material");
    Ogre::Real curvature = dotscene_.getAttribReal("curvature", 10);
    Ogre::Real tiling = dotscene_.getAttribReal("tiling", 8);
    Ogre::Real distance = dotscene_.getAttribReal("distance", 4000);
    bool drawFirst = dotscene_.getAttribBool("drawFirst", true);
    bool active = dotscene_.getAttribBool("active", false);
    if(!active)
        return;


    // Process rotation (?)
    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;

    if(dotscene_.pushChild("rotation"))
    {
        rotation = dotscene_.parseQuaternion();
        dotscene_.popNode();
    }

    // Setup the sky dome
    mSceneMgr->setSkyDome(true, material, curvature, tiling, distance,
                          drawFirst, rotation, 16, 16, -1, m_sGroupName);
}

void DotSceneLoader::processSkyPlane()
{
    Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Process SkyPlane" );

    // Process attributes
    Ogre::String material = dotscene_.getAttrib("material");
    Ogre::Real planeX = dotscene_.getAttribReal("planeX", 0);
    Ogre::Real planeY = dotscene_.getAttribReal("planeY", -1);
    Ogre::Real planeZ = dotscene_.getAttribReal("planeX", 0);
    Ogre::Real planeD = dotscene_.getAttribReal("planeD", 5000);
    Ogre::Real scale = dotscene_.getAttribReal("scale", 1000);
    Ogre::Real bow = dotscene_.getAttribReal("bow", 0);
    Ogre::Real tiling = dotscene_.getAttribReal("tiling", 10);
    bool drawFirst = dotscene_.getAttribBool("drawFirst", true);

    // Setup the sky plane
    Ogre::Plane plane;
    plane.normal = Ogre::Vector3(planeX, planeY, planeZ);
    plane.d = planeD;
    mSceneMgr->setSkyPlane(true, plane, material, scale,
                           tiling, drawFirst, bow, 1, 1, m_sGroupName);
}

void DotSceneLoader::processClipping()
{
    //! @todo Implement this

    // Process attributes
    //Ogre::Real fNear = getAttribReal("near", 0);
    //Ogre::Real fFar = getAttribReal("far", 1);
}

void DotSceneLoader::processLightRange(Ogre::Light *pLight)
{
    // Process attributes
    Ogre::Real inner = dotscene_.getAttribReal("inner");
    Ogre::Real outer = dotscene_.getAttribReal("outer");
    Ogre::Real falloff = dotscene_.getAttribReal("falloff", 1.0);

    // Setup the light range
    pLight->setSpotlightRange(Ogre::Angle(inner), Ogre::Angle(outer), falloff);
}

void DotSceneLoader::processLightAttenuation(Ogre::Light *pLight)
{
    // Process attributes
    Ogre::Real range = dotscene_.getAttribReal("range");
    Ogre::Real constant = dotscene_.getAttribReal("constant");
    Ogre::Real linear = dotscene_.getAttribReal("linear");
    Ogre::Real quadratic = dotscene_.getAttribReal("quadratic");

    // Setup the light attenuation
    pLight->setAttenuation(range, constant, linear, quadratic);
}



Ogre::String DotSceneLoader::getProperty(const Ogre::String &ndNm, const Ogre::String &prop)
{
    for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
    {
        if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
        {
            return nodeProperties[i].valueName;
        }
    }

    return "";
}

void DotSceneLoader::processUserDataReference(Ogre::Entity *pEntity)
{
    Ogre::String str = dotscene_.getAttrib("id");
    pEntity->setUserAny(Ogre::Any(str));
}

bool DotSceneLoader::multipleNodes(std::string name)
{
    // if this is first with given name
    if(first_)
    {
        if(dotscene_.pushChild(name))
        {
            ++nodes_;
            first_ = false;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(dotscene_.pushSibling(name))
    {
        ++nodes_;
        return true;
    }

    // remove all nodes that were added
    for (unsigned int i = 0; i < nodes_; ++i)
    {
        dotscene_.popNode();
    }

    first_ = true;
    nodes_ = 0;
    return false;
}

}
