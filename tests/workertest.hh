
#ifndef WORKERTEST_HH
#define WORKERTEST_HH


#include <cxxtest/TestSuite.h>
#include "boost/shared_ptr.hpp"


#include "../core/worker.hh"
#include "../tasks/task.hh"
#include "../tasks/simpletask.hh"
#include "../tasks/macrotask.hh"


class Workertest : public CxxTest::TestSuite
{
public:

    void testOneTask(void)
    {
        number_ = 0;

        boost::shared_ptr<Shackle::Task> task(new Shackle::Simpletask<Workertest>(this, &Workertest::function));
        Shackle::Worker worker;

        worker.executeTask(task);

        worker.synchronize();

        TS_ASSERT_EQUALS(number_, 1);

    }
    void testCriticalArea(void)
    {

        //tasks in this test should not be executed at the same time
        number_ = 0;
        inCritical_ = false;
        unsigned int target = 10;

        Shackle::Worker worker;

        //add tasks so that they together raise the number_ to target
        for(unsigned int i = 0; i < target; ++i)
        {
            boost::shared_ptr<Shackle::Task> task(new Shackle::Simpletask<Workertest>(this, &Workertest::critical));
            worker.addToQueue(task, 0);
        }

        worker.synchronize();

        TS_ASSERT_EQUALS(number_, target);

    }
    void function()
    {
        ++number_;
    }

    void critical()
    {
        TS_ASSERT(!inCritical_);
        inCritical_ = true;

        //should add some waiting here

        ++number_;

        inCritical_ = false;
    }

private:
    int number_;
    bool inCritical_;


};


#endif

