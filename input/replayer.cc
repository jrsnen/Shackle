#include "replayer.hh"


#include "../tasks/uinttask.hh"
#include "../filesystem/scripter.hh"
#include "../common.hh"


#include <iostream>
namespace Shackle
{



Replayer::Replayer()
:
    round_(0),
    nextCommand_(0),
    nextID_(0),
    tasks_(),
    toBeSaved_(),
    toBeExecuted_()

{
    //ctor
}

Replayer::~Replayer()
{
    //dtor
}


void Replayer::loadReplay(std::string initFile, Scripter& scripter, unsigned int startRound)
{
    scripter.dofile(initFile);

    luabind::call_function<void>(scripter, "replay", boost::ref(this));

    round_ = startRound;

}

void Replayer::clearReplay()
{
    toBeExecuted_.clear();
}


void Replayer::addInputPoint(boost::shared_ptr<Shackle::Macrotask> task, bool replay)
{
    assert(task.get() != 0);

    if(!replay)
    {
        boost::shared_ptr<Task> saving(new Shackle::UIntTask<Replayer>(this, &Replayer::recordAction, nextID_));
        task->addTask(saving);
    }
    tasks_[nextID_] = task;
    ++nextID_;
}


void Replayer::replay()
{
    while(nextCommand_ < toBeExecuted_.size() && toBeExecuted_.at(nextCommand_).cycle <= round_ )
    {

        if(toBeExecuted_.at(nextCommand_).cycle == round_)
        {
            assert(toBeExecuted_.at(nextCommand_).task.get() != 0);

            toBeExecuted_.at(nextCommand_).task->run();
        }
        ++nextCommand_;
    }

    ++round_;
}

void Replayer::saveReplay(OutFileStream& saveFile)
{
    saveFile << "function replay(input)" << std::endl;

    for(unsigned int ui = 0; ui < toBeSaved_.size(); ++ui)
    {
        saveFile << "    input:luaAddTask("
                 << toBeSaved_.at(ui).cycle
                 << "," << toBeSaved_.at(ui).ID
                 << ")" << std::endl;
    }


    saveFile << "end" << std::endl;
}

void Replayer::endRecordRound()
{
    ++round_;
}

//for lua
void Replayer::luaAddTask(unsigned int cycle, unsigned int taskID)
{
    assert(tasks_.find(taskID) != tasks_.end() && "Couldn't find saved task.");
    FutureCommand f = {tasks_[taskID], cycle};
    toBeExecuted_.push_back(f);
}

// called from recorded tasks
void Replayer::recordAction(unsigned int taskID)
{
    PastCommand pc = {round_, taskID};
    toBeSaved_.push_back(pc);
}



}
