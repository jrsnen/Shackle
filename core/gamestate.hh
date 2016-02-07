#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "gameengine.hh"

namespace Shackle
{

class GameState
{
public:

    GameState(){}
    virtual ~GameState(){}


    virtual void init(InputManager& input, GraphicsEngine& graphics,
                      Userinterface* gui, Scripter& scripter) = 0;


    virtual void cleanup() = 0;

    /// \brief Updates this states logic and creates new states if necessary
    ///
    /// \param Engine from wich functions gets systems prime components
    /// \return bool Whether this state still needs to exist

    virtual void pause() = 0;

    virtual void resume() = 0;

    /// \return false if wants to be removed
    virtual bool update(EngineController& controller,StateManager* states, InputManager& input,
                        Scripter& scripter, Worker& worker) = 0;

    virtual void draw(float interpolation, GraphicsEngine& graphics,
                      Userinterface* gui, Worker& worker) = 0;



private:

    GameState(const GameState& other);
    GameState& operator=(const GameState& other);
};

}
#endif // GAMESTATE_HH
