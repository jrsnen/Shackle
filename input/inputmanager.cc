// made by Joni Räsänen


#include "../input/inputmanager.hh"
#include "../input/inputtypes.hh"

#include "../common.hh"

#include <cassert>
#include <iostream>

namespace Shackle
{

InputManager::InputManager():
    replay_(false),
//    recording_(false),
    rInput_(),
    replayer_(),
    contexts_(),
    callbacks_()
{
    //ctor
}

InputManager::~InputManager()
{
    //dtor
}

void InputManager::init(Ogre::RenderWindow* window)
{
    rInput_.init(window);
}

void InputManager::cleanup()
{
    if(replay_)
    {
        replayer_.clearReplay();
    }

}


void InputManager::addinputPoint(boost::shared_ptr<Macrotask> task)
{

    replayer_.addInputPoint(task, true);
}

void InputManager::replay(std::string replayFile, Scripter& scripter)
{
    //assert(!record_ && "inputmanager was already recording when commanded to replay");
    replayer_.loadReplay(replayFile, scripter);
    replay_ = true;
}
/*
void InputManager::record()
{
    assert(!replay_ && "inputmanager was already replaying when commanded to record");

    record_ = true;
}
*/
/// \brief Saves the replay so far to file
void InputManager::saveReplay(std::string fileName)
{
    //assert(record_ && "saving input when not recording");
    OutFileStream stream;

}


void InputManager::pushContext(boost::shared_ptr<Context> context)
{
    assert(context != 0 && " tried to add 0 pointer");
    contexts_.push_back(context);
}


void InputManager::pushCallback(boost::shared_ptr<Callback> callback)
{
    assert(callback != 0 && " tried to add 0 pointer");
    callbacks_.push_back(callback);
}


void InputManager::clear()
{
    contexts_.clear();
    callbacks_.clear();
}


void InputManager::handleInput()
{

    boost::shared_ptr<RawInputType> input(rInput_.getInput());

    assert(input.get() != 0 && " rawinput returned 0 pointer");



    boost::shared_ptr<HighLevelInput> processedInput(new HighLevelInput);
    for(unsigned int i  = 0; i < contexts_.size() &&
                            (input.get()->buttons.begin() != input.get()->buttons.end() ||
                             input.get()->axes.begin() != input.get()->axes.end());
                             ++i)
    {
        contexts_.at(i).get()->interprete(input.get(), processedInput.get());
    }

    //handle input
    for(unsigned int ui = 0; ui < callbacks_.size(); ++ui)
    {
        callbacks_.at(ui).get()->handle(processedInput.get());
    }

    if(replay_)
    {
        replayer_.replay();
    }
    else
    {
        replayer_.endRecordRound();
    }

}

}
