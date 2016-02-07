#ifndef GAMEENGINETEST_HH
#define GAMEENGINETEST_HH


#include <cxxtest/TestSuite.h>
#include <cassert>

#include "core/gameengine.hh"
#include "core/enginecontroller.hh"
#include "core/gamestate.hh"
#include "core/statemanager.hh"
#include "input/rawinputtypes.hh"


namespace Shackle
{

//class EngineController;
class InputManager;
class GraphicsEngine;
class Userinterface;
class Scripter;
class Worker;
}

class TestState : public Shackle::GameState
{
public:
    virtual void init( Shackle::InputManager& input,
                      Shackle::GraphicsEngine& graphics, Shackle::Userinterface* gui,
                      Shackle::Scripter& scripter)
    {}

    virtual void cleanup()
    {}

    virtual void pause()
    {

    }

    virtual void resume()
    {

    }


    virtual bool update(Shackle::EngineController& controller,Shackle::StateManager* states, Shackle::InputManager& input,
                        Shackle::Scripter& scripter, Shackle::Worker& worker)
    {
        ++updated;
        controller.setSpeed(2);

        if(updated == 2)
        {
            controller.quit();
            return false;
        }
        return true;
    }

    virtual void draw(float interpolation, Shackle::GraphicsEngine& graphics,
                      Shackle::Userinterface* gui, Shackle::Worker& worker)
    {

    }

    void setUp()
    {
        updated = 0;
    }

    void getValues(int& updatedp)
    {
        updatedp = updated;
    }

private:
    int updated;
};


class GameEnginetest : public CxxTest::TestSuite
{
public:

    void setUp()
    {

        t = new TestState;
        t->setUp();


        game = new Shackle::GameEngine;



    }

    void tearDown()
    {
        delete game;
    }


    void testStart(void)
    {

        TS_ASSERT_THROWS_NOTHING(game->start(t));

        int updated;

        t->getValues(updated);

        TS_ASSERT_EQUALS(updated, 2);
    }


private:
    Shackle::GameEngine* game;
    TestState* t;


};


#endif
