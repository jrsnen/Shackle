#ifndef BASICCONTEXTTEST_HH
#define BASICCONTEXTTEST_HH

#include <cxxtest/TestSuite.h>

#include "../input/basiccontext.hh"
#include "../filesystem/bind.hh"

#include "../input/rawinputtypes.hh"
#include "../input/inputtypes.hh"
#include "../filesystem/scripter.hh"



class ContextTest : public CxxTest::TestSuite
{
public:
    void setUp()
    {
        Shackle::Scripter scripter;
        bind(scripter);
        context.loadFromFile(scripter, "tests/basiccontexttest.lua");

        input = new Shackle::RawInputType;
        processedInput = new Shackle::HighLevelInput;

    }

    void tearDown(void)
    {
        delete input;
        input = 0;
        delete processedInput;
        processedInput = 0;
    }

    void testActionMapping(void)
    {

        input->buttons.push_back(Shackle::K1);
        context.interprete(input, processedInput);

        TS_ASSERT_EQUALS(processedInput->actions.size(), 1);
        TS_ASSERT_EQUALS(processedInput->states.size(), 0);

        TS_ASSERT_EQUALS(input->buttons.size(), 0);

    }

    void testStateMapping(void)
    {

        input->buttons.push_back(Shackle::K2);
        context.interprete(input, processedInput);

        TS_ASSERT_EQUALS(processedInput->actions.size(), 0);
        TS_ASSERT_EQUALS(processedInput->states.size(), 1);

        TS_ASSERT_EQUALS(input->buttons.size(), 0);
    }

    void testAxisMapping(void)
    {

        Shackle::Axis a = {Shackle::MOUSEXREL, 0.0};
        input->axes.push_back(a);
        input->axes.push_back(a);

        context.interprete(input, processedInput);

        TS_ASSERT_EQUALS(processedInput->actions.size(), 0);
        TS_ASSERT_EQUALS(processedInput->states.size(), 0);
        TS_ASSERT_EQUALS(input->axes.size(), 0);
    }

    void testNotUsing(void)
    {

        input->buttons.push_back(Shackle::K3);
        Shackle::Axis a = {Shackle::MOUSEYREL, 0.0};
        input->axes.push_back(a);

        context.interprete(input, processedInput);

        TS_ASSERT_EQUALS(processedInput->actions.size(), 0);
        TS_ASSERT_EQUALS(processedInput->states.size(), 0);
        TS_ASSERT_EQUALS(input->buttons.size(), 1);
        TS_ASSERT_EQUALS(input->axes.size(), 1);
    }

    void testNoInput(void)
    {
        TS_ASSERT_THROWS_NOTHING(context.interprete(input, processedInput));

        TS_ASSERT_EQUALS(processedInput->actions.size(), 0);
        TS_ASSERT_EQUALS(processedInput->states.size(), 0);
        TS_ASSERT_EQUALS(input->buttons.size(), 0);
    }

private:

    Shackle::BasicContext context;
    Shackle::RawInputType* input;
    Shackle::HighLevelInput* processedInput;

};

#endif // BASICCONTEXTTEST_HH
