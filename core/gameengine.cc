// Game engine Shackle
// made by Joni Räsänen

#include <sstream>


#include "filesystem/bind.hh"

#include "gameengine.hh"
#include "tasks/task.hh"
#include "tasks/simpletask.hh"

#include "input/inputmanager.hh"

#include "common.hh"




namespace Shackle
{

int const SECOND = 1000;

GameEngine::GameEngine()
    :
    controller_(),
    stateManager_(),
    scripter_(),
    worker_(),
    graphics_(),
    gui_(0),
    inputManager_(),
    ticksPerSecond_(0),
    skipTicks_(0),
    maxFrameSkip_(0)
{
    //ctor
}

GameEngine::~GameEngine()
{
    //dtor
}




void GameEngine::start(GameState* firstState)
{

    init(firstState);
    Ogre::LogManager::getSingleton().logMessage("game started");

    skipTicks_ = SECOND/(ticksPerSecond_);
    unsigned long nextTick = graphics_.giveTimer()->getMilliseconds();
    unsigned int loops = 0;
    float interpolation = 0.0;

    unsigned long lastRender = graphics_.giveTimer()->getMilliseconds();
    float lastFPS = 0.0;


    bool displayFPS = false;

    while(controller_.stillGoing() && stateManager_.statesLeft())
    {
        // POSSIBLE ADDITION: restrict drawing to screen to some frequency
        Ogre::LogManager::getSingleton().logMessage("main loop");
        loops = 0;

        while(graphics_.giveTimer()->getMilliseconds() > nextTick  && loops < maxFrameSkip_
                && controller_.stillGoing() && stateManager_.statesLeft())
        {
            Ogre::LogManager::getSingleton().logMessage("updating");

            // if a state was iniated while updating logic
            if(stateManager_.update(controller_, inputManager_, graphics_, gui_.get(), scripter_, worker_))
            {
                // make sure game is not rushed due to delay
                nextTick = graphics_.giveTimer()->getMilliseconds();
            }

            worker_.synchronize();

            nextTick += skipTicks_;

            loops++;
        }

        // draw everything if the game still continues
        if(controller_.stillGoing() && stateManager_.statesLeft())
        {
            if(displayFPS )
            {
                const Ogre::RenderTarget::FrameStats& stats = graphics_.giveRenderWnd()->getStatistics();

                if(lastFPS != stats.lastFPS)
                {
                    lastFPS = stats.lastFPS;
                    std::ostringstream converter;
                    converter << "FPS: " << lastFPS;
                    gui_->giveWindow()->getChild("FPS" )->setText(converter.str());
                }


            }

            gui_->giveSystem().injectTimePulse( float( ( lastRender - graphics_.giveTimer()->getMilliseconds() ) / SECOND ) );
            lastRender = graphics_.giveTimer()->getMilliseconds();

            //how much time has passed since last logic update
            interpolation =  float( graphics_.giveTimer()->getMilliseconds() + skipTicks_ - nextTick )
                             / float( skipTicks_ );

            Ogre::LogManager::getSingleton().logMessage("drawing...");
            stateManager_.draw(interpolation, graphics_, gui_.get(), worker_);
            gui_->draw(interpolation);

            Ogre::LogManager::getSingleton().logMessage("rendering...");
            graphics_.render();
        }
    }

    Ogre::LogManager::getSingleton().logMessage("exiting game");

    cleanup();


}


void GameEngine::init(GameState* firstState)
{
    assert(firstState != 0 && " first state was 0");


    bind(scripter_);
    graphics_.init(scripter_);

    Ogre::LogManager::getSingleton().logMessage("Graphics set up");

    gui_.reset(new Userinterface);
    gui_->init();

    Ogre::LogManager::getSingleton().logMessage("Gui set up");

    // engine settings
    EngineSettings settings = {0,0};

    scripter_.dofile("engine/engine.lua");

    luabind::call_function<void>(scripter_, "engineSettings", boost::ref(settings));

    if(settings.ticksPerSecond == 0)
    {
        throw std::runtime_error("enginesettings not read correctly");
    }

    ticksPerSecond_ = settings.ticksPerSecond;
    maxFrameSkip_ = settings.maxFrameSkip;

    inputManager_.init(graphics_.giveRenderWnd());

    stateManager_.pushState(firstState);
    Ogre::LogManager::getSingleton().logMessage("Engine set up");
}


void GameEngine::cleanup()
{
    graphics_.cleanup();
}

}



