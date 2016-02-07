#ifndef DOTSCENEPROCESSOR_HH
#define DOTSCENEPROCESSOR_HH





namespace Shackle
{

struct Resource
{
    Ogre::String location;
    Ogre::String type;
};

class DotSceneProcessor
{
    public:
        /** Default constructor */
        DotSceneProcessor();
        /** Default destructor */
        virtual ~DotSceneProcessor();

        void init();
        void cleanup();



    protected:
    void processScene();

    void processNodes();
    void processExternals();
    void processEnvironment();
    void processResourceLocations(std::vector<Ogre::String> locations);
    void processTerrain();
    void processTerrainPage();
    void processGrassLayers();
    void processBlendmaps();
    void processUserDataReference();
    void processEntityDataReference();
    void processOctree();
    void processLight();
    void processCamera();

    void processNode();
    void processLookTarget();
    void processTrackTarget();
    void processEntity();
    void processSubEntity();
    void processParticleSystem();
    void processBillboardSet();
    void processPlane();
    void processPagedGeometry();

    void processFog();
    void processSkyBox();
    void processSkyDome();
    void processSkyPlane();
    void processClipping();

    void processLightRange();
    void processLightAttenuation();

    Ogre::SceneManager *mSceneMgr;

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




    DotSceneProcessor(const DotSceneProcessor& other);
    DotSceneProcessor& operator=(const DotSceneProcessor& other);
};



}
#endif // DOTSCENEPROCESSOR_HH
