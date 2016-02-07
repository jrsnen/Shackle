
#include "basiccontext.hh"
#include "callback.hh"
#include "../filesystem/scripter.hh"

#include <iostream>


namespace Shackle
{

BasicContext::BasicContext()
    :
    tempActions_(),
    tempStates_(),
    tempRanges_(),
    actions_(),
    states_(),
    ranges_()
{}

BasicContext::~BasicContext()
{
    //dtor
}

void BasicContext::loadFromFile(Shackle::Scripter& scripter, std::string file)
{

    scripter.dofile(file);


    try
    {
        luabind::call_function<void>(scripter, "loadContext", this);
    }
    catch(luabind::error& e)
    {
        std::cerr << "Error in file: " << file << std::endl;
        throw e;
    }

    setContext(tempActions_, tempStates_, tempRanges_);
}



void BasicContext::setContext(std::vector<ActionMap>& actions,
                              std::vector<StateMap>& states,
                              std::vector<RangeMap>& ranges)
{
    for(unsigned int ui = 0; ui < actions.size(); ++ui)
    {
        ActionInstance a = {actions.at(ui).action, false, false};
        actions_[actions.at(ui).button] = a;
    }

    for(unsigned int ui = 0; ui < states.size(); ++ui)
    {
        states_[states.at(ui).button] = states.at(ui).state;
    }

    for(unsigned int ui = 0; ui < ranges.size(); ++ui)
    {
        ranges_[ranges.at(ui).axis] = ranges.at(ui).range;
    }
}

void BasicContext::interprete(Shackle::RawInputType* input, HighLevelInput* highResult)
{

    // check buttons for actions and states
    std::list<Shackle::Button>::iterator i = input->buttons.begin();
    while( i != input->buttons.end())
    {
        if(actions_.find(*i) != actions_.end())
        {
            ActionInstance a = actions_[*i];
            if(!a.wasPressed)
            {
                highResult->actions.push_back(a.action);
                actions_[*i].nowPressed = true;

            }

            input->buttons.erase(i++);
        }
        else if(states_.find(*i) != states_.end())
        {
            highResult->states.push_back(states_[*i]);
            input->buttons.erase(i++);
        }
        else
        {
            ++i;
        }

    }


    // check axes for ranges
    std::list<Shackle::Axis>::iterator j = input->axes.begin();
    while( j != input->axes.end())
    {
        if(ranges_.find((*j).type) != ranges_.end())
        {
            Range r = {ranges_[(*j).type], (*j).value};
            highResult->ranges.push_back(r);
            input->axes.erase(j++);

        }
        else
        {
            ++j;
        }

    }

    // update what action buttons are down for next call
    for(std::map<Shackle::Button, ActionInstance>::iterator iii = actions_.begin();
            iii != actions_.end(); ++iii)
    {
        (*iii).second.wasPressed = (*iii).second.nowPressed;
        (*iii).second.nowPressed = false;
    }
}


void BasicContext::luaAddActionMap(Shackle::Button button, Action action)
{
    ActionMap a = {button, action};
    tempActions_.push_back(a);
}

void BasicContext::luaAddStateMap(Shackle::Button button, State state)
{
    StateMap a = {button, state};
    tempStates_.push_back(a);
}

void BasicContext::luaAddRangeMap(Shackle::AxisType axis, RangeType range)
{
    RangeMap a = {axis, range};
    tempRanges_.push_back(a);
}



}
