#ifndef REPLAYERTEST_HH
#define REPLAYERTEST_HH



#include "../input/replayer.hh"
#include "../filesystem/bind.hh"
#include "../filesystem/scripter.hh"
#include "../tasks/simpletask.hh"

#include "../tasks/task.hh"

#include <cxxtest/TestSuite.h>

#include <iostream>


class ReplayerTest : public CxxTest::TestSuite
{
    public:

        void setUp()
        {
            testTask. reset(new Shackle::Macrotask);
            boost::shared_ptr<Shackle::Task> testFunction
                (new Shackle::Simpletask<ReplayerTest>(this, &ReplayerTest::task ));
            testTask->addTask(testFunction);

            r = new Shackle::Replayer;

            executed_ = 0;
            Shackle::bind(scripter);
        }

        void tearDown()
        {
            delete r;
            r = 0;
        }
        void testAddInputPoint()
        {
            TS_ASSERT_THROWS_NOTHING(r->addInputPoint(testTask, true));
        }
        void testReplay(void)
        {
            r->addInputPoint(testTask, true);

            TS_ASSERT_THROWS_NOTHING(r->loadReplay("tests/replaytest.lua", scripter));

            r->replay();
            r->replay();
            TS_ASSERT_EQUALS(executed_, 0);

            r->replay();
            r->replay();
            r->replay();
            r->replay();
            TS_ASSERT_EQUALS(executed_, 1);

            r->replay();
            TS_ASSERT_EQUALS(executed_, 1);

            r->replay();

            TS_ASSERT_EQUALS(executed_, 2);

            r->replay();

            TS_ASSERT_EQUALS(executed_, 2);
        }

        void testReplaySaving(void)
        {
            r->addInputPoint(testTask, false);
            r->endRecordRound(); // end 0
            r->endRecordRound(); // end 1
            testTask->run(); // round 2 is taking place
            r->endRecordRound();
            r->endRecordRound();

            TS_ASSERT_EQUALS(executed_, 1);

            OutFileStream stream;
            stream.open("tests/replay.rep");

            TS_ASSERT_THROWS_NOTHING(r->saveReplay(stream));

            TS_ASSERT_THROWS_NOTHING(r->loadReplay("tests/replay.rep", scripter));
            stream.close();

            r->replay();
            r->replay();
            TS_ASSERT_EQUALS(executed_, 1);

            r->replay();
            TS_ASSERT_EQUALS(executed_, 2);

            r->replay();
            TS_ASSERT_EQUALS(executed_, 2);

        }

        void task()
        {
            ++executed_;
        }

    private:
        Shackle::Scripter scripter;

        Shackle::Replayer* r;
        int executed_;

        boost::shared_ptr<Shackle::Macrotask> testTask;
};

#endif // REPLAYERTEST_HH
