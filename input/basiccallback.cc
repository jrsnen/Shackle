

#include "basiccallback.hh"

#include "../tasks/task.hh"

namespace Shackle
{

BasicCallback::BasicCallback()
{
    //ctor
}

BasicCallback::~BasicCallback()
{
    //dtor
}

void BasicCallback::handle(HighLevelInput* input)
{
    // check buttons for actions and states
    std::list<Action>::iterator i = input->actions.end();
    while( i != input->actions.begin())
    {
        --i;
        if(actions_.find(*i) != actions_.end())
        {
            assert(actions_[*i].get() != 0);
            actions_[*i]->run();
            input->actions.erase(i);
        }

    }

    std::list<State>::iterator ii = input->states.end();
    while( ii != input->states.begin())
    {
        --ii;
        if(states_.find(*ii) != states_.end())
        {
            states_[*ii]->run();
            input->states.erase(ii);
        }
    }
}


void BasicCallback::addAction(Action action, boost::shared_ptr<Shackle::Task> task)
{
    actions_[action] = task;
}

void BasicCallback::addState(State state, boost::shared_ptr<Shackle::Task> task)
{
    states_[state] = task;
}

}
