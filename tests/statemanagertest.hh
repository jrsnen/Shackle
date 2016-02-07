#ifndef STATEMANAGERTEST_HH
#define STATEMANAGERTEST_HH



#include "../core/statemanager.hh"
#include "../core/gameengine.hh"
#include <cxxtest/TestSuite.h>


class TesterState : public Shackle::GameState
{
    public:

    TesterState(){}


    virtual void init( Shackle::InputManager& input,
                      Shackle::GraphicsEngine& graphics, Shackle::Userinterface* gui,
                      Shackle::Scripter& scripter)
    {}

    virtual void cleanup()
    {}

    virtual void pause()
    {}

    virtual void resume()
    {}

    virtual bool update(Shackle::EngineController& controller,Shackle::StateManager* states,
                        Shackle::InputManager& input, Shackle::Scripter& scripter, Shackle::Worker& worker)
    {
        return false;
    }

    virtual void draw(float interpolation, Shackle::GraphicsEngine& graphics,
                      Shackle::Userinterface* gui, Shackle::Worker& worker)
    {}

    private:

};


class StateManagerTest : public CxxTest::TestSuite
{
    public:

    void setUp()
    {
        stateA = new TesterState;
        stateB = new TesterState;
    }

    void tearDown()
    {
        delete stateA;
        delete stateB;
    }


    void testpush(void)
    {
        Shackle::StateManager manager;

        TS_ASSERT(!manager.statesLeft());
        TS_ASSERT_THROWS_NOTHING(manager.pushState(stateA));

        TS_ASSERT(manager.statesLeft());

        TS_ASSERT_THROWS_NOTHING(manager.pushState(stateB));
    }


    private:

    TesterState* stateA;
    TesterState* stateB;
};

#endif // STATEMANAGERTEST_HH
