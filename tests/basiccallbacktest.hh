#ifndef BASICCALLBACKTEST_HH
#define BASICCALLBACKTEST_HH


#include "../input/basiccallback.hh"

#include "../input/inputtypes.hh"

#include "../tasks/task.hh"
#include "../tasks/simpletask.hh"



#include <cxxtest/TestSuite.h>




class BasicCallbackTest : public CxxTest::TestSuite
{
    public:

        void setUp()
        {

            task.reset(new Shackle::Simpletask<BasicCallbackTest>
                        (this, &BasicCallbackTest::function));

            c.addAction("QUIT", task);
            c.addState("SCROLLUP", task);

            input = new Shackle::HighLevelInput;
            i = 0;

        }

        void teardown()
        {
            delete input;
        }

        void testHandleEmpty(void)
        {
            TS_ASSERT_THROWS_NOTHING(c.handle(input));

            TS_ASSERT_EQUALS(i,0);
        }

        void testHandleOne(void)
        {
            input->actions.push_back("QUIT");
            TS_ASSERT_THROWS_NOTHING(c.handle(input));

            TS_ASSERT_EQUALS(i,1);

            TS_ASSERT_EQUALS(input->actions.size() ,0);
        }

        void testHandleSeveral(void)
        {
            input->actions.push_back("QUIT");
            c.handle(input);

            input->actions.push_back("QUIT");
            TS_ASSERT_THROWS_NOTHING(c.handle(input));

            input->actions.push_back("QUIT");
            c.handle(input);

            TS_ASSERT_EQUALS(i,3);
            TS_ASSERT_EQUALS(input->actions.size() ,0);
        }

        void testState(void)
        {
            input->states.push_back("SCROLLUP");
            TS_ASSERT_THROWS_NOTHING(c.handle(input));
            TS_ASSERT_EQUALS(i,1);
            TS_ASSERT_EQUALS(input->states.size() ,0);

            i = 0;
            input->states.push_back("SCROLLUP");
            input->actions.push_back("QUIT");
            c.handle(input);
            TS_ASSERT_EQUALS(i,2);
            TS_ASSERT_EQUALS(input->states.size() ,0);


        }

        void testHandleNone(void)
        {
            input->actions.push_back("START");
            TS_ASSERT_THROWS_NOTHING(c.handle(input));
        }


        void function()
        {
            ++i;
        }

    private:
        Shackle::BasicCallback c;

        boost::shared_ptr<Shackle::Task> task;

        Shackle::HighLevelInput* input;


        int i;
};

#endif // BASICCALLBACKTEST_HH
