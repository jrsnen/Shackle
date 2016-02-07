#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH


#include "../core/worker.hh"
#include "../core/statemanager.hh"
#include "../core/enginecontroller.hh"

#include "../output/graphicengine.hh"
#include "../output/userinterface.hh"
#include "../filesystem/scripter.hh"
#include "../input/inputmanager.hh"
#include "../common.hh"


#include <memory>

namespace Shackle
{

struct EngineSettings
{
    unsigned int ticksPerSecond;
    unsigned int maxFrameSkip;
};


class GameEngine
{
public:

    //takes ownership of base
    GameEngine();
    ~GameEngine();

    //void loading(); // maybe return bool if multiple are starting loading at the same cycle(or just counter should also work)
    //void loadingFinished(); //same as above

    //! \brief Starts the game. Runs at rate defined in engine.lua file
    void start(GameState* firstState);


private:

    //! \brief Initiates the engine and its subsystems. Call before using
    void init(GameState* firstState);

    //! \brief Shuts systems down. Call after using
    void cleanup();

    //bool running_;

    EngineController controller_;
    StateManager stateManager_;

    Scripter scripter_;
    Worker worker_;
    GraphicsEngine graphics_;
    std::auto_ptr<Userinterface> gui_;

    InputManager inputManager_;

    unsigned int ticksPerSecond_;
    unsigned int skipTicks_;
    unsigned int maxFrameSkip_;

    // forbid copying and assigment
    GameEngine(GameEngine const& old );
    GameEngine& operator =(GameEngine const& old);
};
}

#endif // GAMEENGINE_HH
