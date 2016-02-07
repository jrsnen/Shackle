#ifndef GRAPHICENGINE_HH
#define GRAPHICENGINE_HH




//#include "output/dotsceneloader.hh"

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>


#include <memory>
namespace OGRE
{
class RenderWindow;
}


namespace Shackle
{

struct Location
{
    Ogre::Vector3 position;
    Ogre::Vector3 direction;
};

struct Graphicfiles
{
    Ogre::String configFileName;
    Ogre::String pluginsFileName;
    Ogre::String logFileName;
    Ogre::String resourceFileName;
};

struct Windowsettings
{
    Ogre::String title;
    Ogre::String customCapacities;
};



class Scripter;


class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    /// \brief Loads ogre and starts a window
    ///
    /// \param scripter For executing scripts
    ///
    ///
    bool init(Scripter& scripter);

    /// \brief Renders one frame
    ///

    void cleanup();


    void loadDotScene(std::string scene);

    void render();


    //void getInput();

    /// \brief Saves message to default log
    ///
    /// \param message Logged string
    ///

    //void logMessage(Ogre::String message);

    Ogre::Root* giveRoot();

    Ogre::SceneManager* giveSceneMgr();

    Ogre::RenderWindow* giveRenderWnd();

    Ogre::Timer* giveTimer();



private:

    std::auto_ptr<Ogre::Root> apRoot_;

    Windowsettings settings_;

    Ogre::SceneManager*			pSceneMgr_;
    Ogre::RenderWindow*			pRenderWnd_;



    Ogre::Log*				    pLog_;
    Ogre::Timer*				pTimer_;

    //DotSceneLoader sceneLoader_;

    //std::auto_ptr<OgreBites::SdkTrayManager>	pTrayMgr_;
    //Ogre::FrameEvent            frameEvent_;

    int					        iNumScreenShots_;



    GraphicsEngine(const GraphicsEngine& other);
    GraphicsEngine& operator=(const GraphicsEngine& other);
};

}
#endif // GRAPHICENGINE_HH
