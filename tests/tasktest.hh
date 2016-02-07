#ifndef TASKTEST_HH
#define TASKTEST_HH

#include <cxxtest/TestSuite.h>
#include "boost/shared_ptr.hpp"


#include "../tasks/simpletask.hh"
#include "../tasks/macrotask.hh"
#include "../tasks/stringtask.hh"
#include "../tasks/uinttask.hh"
//#include "../tasks/guitask.hh"

class Tasktest : public CxxTest::TestSuite
{
public:

    void setUp()
    {
        sum_ = 0;

        task.reset(new Shackle::Simpletask<Tasktest>(this, &Tasktest::adding));

    }

    void tearDown()
    {

    }


    void testOneTask(void)
    {
        task->run();

        TS_ASSERT_EQUALS(sum_, 1);
    }

    void testMacrotask(void)
    {
        unsigned int target = 5;

        Shackle::Macrotask* m = new Shackle::Macrotask;

        for(unsigned int i = 0; i < target; ++i)
        {
            m->addTask(task);
        }

        m->run();
        TS_ASSERT_EQUALS(sum_, target);

        delete m;

    }

    void testStringTask(void)
    {
        std::string message = "test message";
        answer = "wrong";
        Shackle::StringTask<Tasktest> sTask(this, &Tasktest::answerString, message);
        sTask.run();
        TS_ASSERT_EQUALS(message, answer);
    }


    void adding()
    {
        ++sum_;
    }

    bool eventExecute(const CEGUI::EventArgs& /*e*/)
    {
        ++executed;
        return true;
    }

    void answerString(std::string a)
    {
        answer = a;
    }

private:
    int sum_;
    int executed;
    std::string answer;

    boost::shared_ptr<Shackle::Task> task;
};

#endif // TASKTEST_HH
