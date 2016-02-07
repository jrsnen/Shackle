//creator: Joni Räsänen



#include "statemanager.hh"
#include "gamestate.hh"

#include <cassert>


namespace Shackle
{

StateManager::StateManager():iniated_(false), states_()
{
    //ctor
}

StateManager::~StateManager()
{
    //dtor
}

void StateManager::pushState(GameState* state)
{

    if(!states_.empty())
    {
        states_.at(states_.size() - 1)->pause();
    }

    states_.push_back(state);
    iniated_ = false;
}


bool StateManager::update(EngineController& controller, InputManager& input,
                          GraphicsEngine& graphics, Userinterface* gui,
                          Scripter& scripter, Worker& worker)
{
    if(states_.size() == 0)
    {
        controller.quit();
        return false;
    }

    bool iniation = !iniated_;

    if(!iniated_)
    {
        states_.at(states_.size() - 1)->init(input, graphics, gui, scripter);
        iniated_ = true;
    }

    if(!states_.at(states_.size() - 1)->update(controller, this, input, scripter, worker))
    {
        popState();
    }

    return iniation;
}

void StateManager::draw(float interpolation, GraphicsEngine& graphics,
                      Userinterface* gui, Worker& worker)
{
    assert(!states_.empty() && "No states in state manager, while calling draw");
    states_.at(states_.size() - 1)->draw(interpolation, graphics, gui, worker);
}

bool StateManager::statesLeft() const
{
    return !states_.empty();
}

void StateManager::popState()
{
    assert(!states_.empty() && "tried to pop empty queue");

    states_.at(states_.size() - 1)->cleanup();
    states_.pop_back();

    if(!states_.empty())
    {
        states_.at(states_.size() - 1)->resume();
    }
}


}
