#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H


#include "output/xmlwrapper.hh"

// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreResourceGroupManager.h>
#include <vector>

#include "rapidxml.hpp"

// Forward declarations
namespace Ogre
{
class SceneManager;
class SceneNode;
class TerrainGroup;
class TerrainGlobalOptions;
}
/*
    namespace Forests
    {
        class PagedGeometry;
        class TreeLoader3D;
        class GrassLoader;
        class GrassLayer;
    }
*/
class nodeProperty
{
public:
    Ogre::String nodeName;
    Ogre::String propertyNm;
    Ogre::String valueName;
    Ogre::String typeName;

    nodeProperty(const Ogre::String &node, const Ogre::String &propertyName, const Ogre::String &value, const Ogre::String &type)
        : nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
};
namespace Shackle
{

class DotSceneLoader
{
public:

    Ogre::TerrainGlobalOptions *mTerrainGlobalOptions;

    DotSceneLoader();
    virtual ~DotSceneLoader();

    void init();
    void cleanup();

    void parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode = NULL, const Ogre::String &sPrependNode = "");
    Ogre::String getProperty(const Ogre::String &ndNm, const Ogre::String &prop);

    Ogre::String getResourceLocationBaseDir()
    {
        return resLocationsBaseDir;
    };
    void setResourceLocationBaseDir( Ogre::String newBaseDir )
    {
        resLocationsBaseDir = newBaseDir;
    };

    Ogre::TerrainGroup* getTerrainGroup()
    {
        return mTerrainGroup;
    }


    std::vector<nodeProperty>               nodeProperties;
    std::vector<Ogre::String>               staticObjects;
    std::vector<Ogre::String>               dynamicObjects;
    //std::vector<Forests::PagedGeometry *>   mPGHandles;
    //std::vector<Forests::TreeLoader3D *>    mTreeHandles;
    //Forests::GrassLoader*                   mGrassLoaderHandle;   /** Handle to Forests::GrassLoader object */
    std::vector<Ogre::ParticleSystem *>     particleSystemList;	// contains all the Particle Systems created

protected:
    void processScene();

    void processNodes();
    void processExternals();
    void processEnvironment();
    void processResourceLocations();
    void processTerrain();
    void processTerrainPage();
    void processGrassLayers();
    void processBlendmaps();
    void processUserDataReference(Ogre::SceneNode *pParent = 0);
    void processUserDataReference(Ogre::Entity *pEntity);
    void processOctree();
    void processLight(Ogre::SceneNode *pParent = 0);
    void processCamera(Ogre::SceneNode *pParent = 0);

    void processNode(Ogre::SceneNode *pParent = 0);
    void processLookTarget(Ogre::SceneNode *pParent);
    void processTrackTarget(Ogre::SceneNode *pParent);
    void processEntity(Ogre::SceneNode *pParent);
    void processSubEntity(Ogre::Entity *pEntity);
    void processParticleSystem(Ogre::SceneNode *pParent);
    void processBillboardSet(Ogre::SceneNode *pParent);
    void processPlane(Ogre::SceneNode *pParent);
    void processPagedGeometry(Ogre::SceneNode *pParent);

    void processFog();
    void processSkyBox();
    void processSkyDome();
    void processSkyPlane();
    void processClipping();

    void processLightRange(Ogre::Light *pLight);
    void processLightAttenuation(Ogre::Light *pLight);

    Ogre::SceneManager *mSceneMgr;
    Ogre::SceneNode *mAttachNode;
    Ogre::String m_sGroupName;
    Ogre::String m_sPrependNode;
    Ogre::TerrainGroup* mTerrainGroup;
    Ogre::Vector3 mTerrainPosition;
    Ogre::Vector3 mLightDirection;

    // paged geometry related values
    //int mPGPageSize;
    //int mPGDetailDistance;
    // resource locations basedir
    Ogre::String resLocationsBaseDir;


    private:

    // to be called only from while
    bool multipleNodes(std::string name);
    bool first_;
    unsigned int nodes_;

    XMLWrapper dotscene_;
    std::auto_ptr<DotSceneProcessor> processor_;
};

}

#endif // DOT_SCENELOADER_H
