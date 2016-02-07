//made by Joni Räsänen
#include "worker.hh"
#include "tasks/task.hh"

#include "boost/shared_ptr.hpp"

#include "common.hh"

namespace Shackle
{
Worker::Worker():
    poolsize_(boost::thread::hardware_concurrency() -1), pool_(poolsize_), queues_()
{
    //ctor
}

Worker::~Worker()
{
    //dtor
}

void Worker::executeTask(boost::shared_ptr<Task> task)
{

    assert(task != 0);


    boost::threadpool::pool* ptr = &pool_;
    if(!boost::threadpool::schedule(ptr, task))
    {
        Ogre::LogManager::getSingleton().logMessage("ERROR: Failed to schedule a task");
    }
}

void Worker::addToQueue(boost::shared_ptr<Task> task, unsigned int id)
{


    if(id >= queues_.size())
    {
        boost::shared_ptr<Macrotask> macro(new Macrotask());
        for(unsigned int queue = id + 1 - queues_.size(); queue != 0; --queue)
        {
            queues_.push_back(macro);
        }
    }

    queues_.at(id)->addTask(task);



}
void Worker::synchronize()
{
    for(unsigned int task = 0; task < queues_.size(); ++task)
    {
        boost::threadpool::pool* ptr = &pool_;
        if(boost::threadpool::schedule(ptr, queues_.at(task)))
        {

        }

    }

    if(pool_.size_controller().resize(poolsize_ + 1))
    {
        pool_.wait(0);
        pool_.size_controller().resize(poolsize_ );
    }
    else if(poolsize_ > 0)
    {
        pool_.wait(0);
        Ogre::LogManager::getSingleton().logMessage("WARNING: Failed to resize pool");
    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage("ERROR: Failed to execute pool");
    }

    for(unsigned int task = 0; task < queues_.size(); ++task)
    {
        queues_.at(task)->clear();
    }
}

}
