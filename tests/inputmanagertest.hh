#ifndef INPUTMANAGERTEST_HH
#define INPUTMANAGERTEST_HH


#include "../input/inputmanager.hh"
#include "../output/graphicengine.hh"

#include "../input/context.hh"
#include "../input/callback.hh"
#include "../input/basiccontext.hh"
#include "../input/basiccallback.hh"

#include "../filesystem/scripter.hh"


#include <cxxtest/TestSuite.h>








class InputManagerTest : public CxxTest::TestSuite
{
public:

    void setUp()
    {
        TS_TRACE("InputManagertest setUp");
        Shackle::Scripter scripter;
        Shackle::bind(scripter);
        graphics_ = new Shackle::GraphicsEngine;
        graphics_->init(scripter);

        co1_.reset(new Shackle::BasicContext);
        co2_.reset(new Shackle::BasicContext);

        co1_->loadFromFile(scripter, "tests/basiccontexttest.lua");

        ca1_.reset(new Shackle::BasicCallback);
        ca2_.reset(new Shackle::BasicCallback);

        task_.reset(new Shackle::Simpletask<InputManagerTest>
                    (this, &InputManagerTest::nothing));
        ca1_->addAction("QUIT", task_);
        ca2_->addState("SCROLLUP", task_);

        i = 0;
    }

    void tearDown()
    {
        input_.clear();
        delete graphics_;
    }

    void testInit(void)
    {
        TS_ASSERT_THROWS_NOTHING(input_.init(graphics_->giveRenderWnd()));
    }

    void testPushContext(void)
    {
        input_.init(graphics_->giveRenderWnd());

        TS_ASSERT_THROWS_NOTHING(input_.pushContext(co1_));
        TS_ASSERT_THROWS_NOTHING(input_.pushContext(co2_));

    }

    void testPushCallback(void)
    {
        input_.init(graphics_->giveRenderWnd());

        TS_ASSERT_THROWS_NOTHING(input_.pushCallback(ca1_));
        TS_ASSERT_THROWS_NOTHING(input_.pushCallback(ca2_));

    }

    void testHandle(void)
    {
        input_.init(graphics_->giveRenderWnd());

        TS_ASSERT_THROWS_NOTHING(input_.handleInput());

        input_.pushContext(co1_);
        input_.pushContext(co2_);

        TS_ASSERT_THROWS_NOTHING(input_.handleInput());

        input_.pushCallback(ca1_);
        input_.pushCallback(ca2_);

        TS_ASSERT_THROWS_NOTHING(input_.handleInput());
    }

    void testReplay(void)
    {
        boost::shared_ptr<Shackle::Macrotask> testTask(new Shackle::Macrotask);
        boost::shared_ptr<Shackle::Task> testFunction
                (new Shackle::Simpletask<InputManagerTest>(this, &InputManagerTest::nothing ));
        testTask->addTask(testFunction);

        Shackle::Scripter scripter;

        Shackle::bind(scripter);

        input_.init(graphics_->giveRenderWnd());

        TS_ASSERT_THROWS_NOTHING(input_.addinputPoint(testTask));
        TS_ASSERT_THROWS_NOTHING(input_.replay("tests/replaytest.lua", scripter));

        TS_ASSERT_THROWS_NOTHING(input_.handleInput());
    }

    void nothing()
    {
        ++i;;
    }

private:

    Shackle::InputManager input_;
    Shackle::GraphicsEngine* graphics_;


    boost::shared_ptr<Shackle::BasicContext> co1_;
    boost::shared_ptr<Shackle::BasicContext> co2_;

    boost::shared_ptr<Shackle::BasicCallback> ca1_;
    boost::shared_ptr<Shackle::BasicCallback> ca2_;

    boost::shared_ptr<Shackle::Task> task_;

    int i;

};

#endif // INPUTMANAGERTEST_HH
