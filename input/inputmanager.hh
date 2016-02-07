

#ifndef INPUTMANAGER_HH
#define INPUTMANAGER_HH


#include "../input/rawinput.hh"
#include "../input/callback.hh"
#include "../input/context.hh"
#include "../input/replayer.hh"
#include "../tasks/task.hh"

#include <OGRE/OgreRenderWindow.h>

#include <string>
#include <vector>


namespace Shackle
{

struct KeyTask
{
    Task* task;
};



//class Context;
class GameEngine;
class GuiCallback;
class Macrotask;


class InputManager
{
public:
    InputManager();
    ~InputManager();

    //! \brief Initiates the class an its parts to be ready for operations
    void init(Ogre::RenderWindow* window);

    //! \brief cleans class for destruction
    void cleanup();

    //! \brief add an input task that can be executed in replay
    void addinputPoint(boost::shared_ptr<Macrotask> task);

    //! \brief loads a replay
    //! \param replayFile File in which the wanted replay resides
    void replay(std::string replayFile, Scripter& scripter);

    //void record();

    /// \brief Saves the replay so far to file
    void saveReplay(std::string fileName);

    //! \brief Adds a context  to list of rawinput handlers
    void pushContext(boost::shared_ptr<Context> context);

    //! \brief Adds caalback to a list of high level input handlers
    void pushCallback(boost::shared_ptr<Callback> callback);

    void clear();

    //! \brief Handels input of this round
    void handleInput();




private:



    bool replay_;
    //bool recording_;

    RawInput rInput_;
    Replayer replayer_;



    std::vector<boost::shared_ptr<Context> > contexts_;
    std::vector<boost::shared_ptr<Callback> > callbacks_;

    InputManager(const InputManager& other);
    InputManager& operator=(const InputManager& other);
};

}

#endif // INPUTMANAGER_HH
