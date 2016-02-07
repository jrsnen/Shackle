#include "graphicengine.hh"
#include "../filesystem/scripter.hh"


//#include "OGRE/OgreRoot.h"
//#include "OGRE/OgreRenderSystem.h"
//#include "OGRE/OgreRenderWindow.h"



#include "OGRE/OgreWindowEventUtilities.h"

#include <OGRE/OgreConfigFile.h>

#include <exception>

#include <iostream>



namespace Shackle
{


GraphicsEngine::GraphicsEngine()
    :
    apRoot_(0),
    settings_(),
    pSceneMgr_(0),
    pRenderWnd_(0),
    pLog_(0),
    pTimer_(0),

    //pTrayMgr_(0),
    //frameEvent_(Ogre::FrameEvent()),
    iNumScreenShots_()//,

{}


GraphicsEngine::~GraphicsEngine()
{}

bool GraphicsEngine::init(Scripter& scripter)
{
    scripter.dofile("engine/graphics.lua");

    Graphicfiles files;
    luabind::call_function<void>(scripter, "graphicsettings", boost::ref(files), boost::ref(settings_));

    apRoot_.reset (new Ogre::Root(files.pluginsFileName, files.configFileName, files.logFileName));

    Ogre::LogManager::getSingleton().logMessage("Root iniated");
    // restores old settings f any
    if(!apRoot_->restoreConfig())
    {
        // if old settngs couldn't be loaded, a dialog to determine new ones is displayed
        if(apRoot_->showConfigDialog())
        {
            apRoot_->saveConfig();

        }
        else
        {
            //if cancel was pressed renderer is chosen automatically
            const Ogre::RenderSystemList& lRenderSystemList = apRoot_->getAvailableRenderers();
            if( lRenderSystemList.size() == 0 )
            {
                Ogre::LogManager::getSingleton().logMessage("Sorry, no rendersystem was found.");
                return false;
            }

            apRoot_->setRenderSystem(lRenderSystemList[0]);
        }
    }

    bool lCreateAWindowAutomatically = true;
    pRenderWnd_ = apRoot_->initialise(lCreateAWindowAutomatically, settings_.title);

    pSceneMgr_ = apRoot_->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
    pSceneMgr_->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));


    Ogre::String secName;
    Ogre::String typeName;
    Ogre::String archName;

    Ogre::ConfigFile cf;
    cf.load(files.resourceFileName);

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    //Ogre::LogManager::getSingleton().createLog("engine.log");


    //sceneLoader_.init();

    pTimer_ = new Ogre::Timer();
    pTimer_->reset();

    pRenderWnd_->setActive(true);


    return true;
}

void GraphicsEngine::cleanup()
{
    //sceneLoader_.cleanup();
    apRoot_.get()->shutdown();
}

void GraphicsEngine::loadDotScene(std::string scene)
{
    //sceneLoader_.parseDotScene(scene, "General", giveSceneMgr());
}

void GraphicsEngine::render()
{
    //moveScale_ = moveSpeed_   * (float)timeSinceLastFrame;
    //rotScale_  = rotateSpeed_ * (float)timeSinceLastFrame;

    //translateVector_ = Ogre::Vector3::ZERO;


    //getInput();
    //moveCamera();

    apRoot_->renderOneFrame();
    //frameEvent_.timeSinceLastFrame = timeSinceLastFrame;
    //pTrayMgr_->frameRenderingQueued(frameEvent_);

    Ogre::WindowEventUtilities::messagePump();
}


/*
void GraphicsEngine::moveCamera()
{
    if(pKeyboard_->isKeyDown(OIS::KC_LSHIFT))
        pCamera_->moveRelative(translateVector_);
    else
        pCamera_->moveRelative(translateVector_ / 10);
}

void GraphicsEngine::getInput()
{
    if(pKeyboard_->isKeyDown(OIS::KC_A))
        translateVector_.x = -moveScale_;

    if(pKeyboard_->isKeyDown(OIS::KC_D))
        translateVector_.x = moveScale_;

    if(pKeyboard_->isKeyDown(OIS::KC_W))
        translateVector_.z = -moveScale_;

    if(pKeyboard_->isKeyDown(OIS::KC_S))
        translateVector_.z = moveScale_;
}

bool GraphicsEngine::keyPressed(const OIS::KeyEvent &keyEventRef)
{

    if(pKeyboard_->isKeyDown(OIS::KC_SYSRQ))
    {
        pRenderWnd_->writeContentsToTimestampedFile("Screenshot_", ".png");
        return true;
    }

    return true;
}

bool GraphicsEngine::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

bool GraphicsEngine::mouseMoved(const OIS::MouseEvent &evt)
{
    //pCamera_->yaw(Ogre::Degree(evt.state.X.rel * -0.1f));
    //pCamera_->pitch(Ogre::Degree(evt.state.Y.rel * -0.1f));

    return true;
}

bool GraphicsEngine::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

bool GraphicsEngine::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

    void GraphicsEngine::logMessage(Ogre::String message)
    {
        return Ogre::LogManager::getSingleton().logMessage(message);
    }
*/
    Ogre::Root* GraphicsEngine::giveRoot()
    {
        return apRoot_.get();
    }

    Ogre::SceneManager* GraphicsEngine::giveSceneMgr()
    {
        assert(pSceneMgr_ != 0);
        return pSceneMgr_;
    }

    Ogre::RenderWindow* GraphicsEngine::giveRenderWnd()
    {
        assert(pRenderWnd_ != 0);
        return pRenderWnd_;
    }

    Ogre::Timer* GraphicsEngine::giveTimer()
    {
        assert(pTimer_ != 0);
        return pTimer_;
    }


}
