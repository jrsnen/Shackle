#ifndef REPLAYER_HH
#define REPLAYER_HH


#include "../tasks/macrotask.hh"
#include "../common.hh"

#include <boost/shared_ptr.hpp>


#include <string>
#include <vector>
#include <map>


namespace Shackle
{
class Scripter;

class Replayer
{
public:
    Replayer();
    ~Replayer();

    /// \brief Load  a inputs from file.
    ///
    /// \param scripter Used to execute the load script.
    /// \param initFile name of the replay file
    /// \param startRound offset for the starting round compared to ones marked on file
    ///
    void loadReplay(std::string initFile, Scripter& scripter, unsigned int startRound = 0);

    /// \brief Discard rest of the replay so it won't be executed
    ///
    void clearReplay();

    /// \brief Adds a task whose executions are recorded for later saving
    ///        and if replaying then all task that are in replay must be added beforehand.
    /// \param task Task in question.
    /// \param taskID specified name for the task. In future may be automated
    /// \param saveActions Tells wether executing actions will save them
    ///
    void addInputPoint(boost::shared_ptr<Macrotask> task,
                       bool saveActions);

    /// \brief Replay tasks for this round are executed
    ///
    ///
    void replay();

    /// \brief Saves task history up to this moment to stream
    ///
    /// \param saveFile Stream to save file
    ///
    void saveReplay(OutFileStream& saveFile);

    /// \brief Used to mark when round changes during recording
    ///
    void endRecordRound();

    /// \brief Function which lua scripts use to add tasks to replay
    ///
    /// \param cycle Time the task is executed
    /// \param taskID ID for the task to be executed
    ///
    void luaAddTask(unsigned int cycle, unsigned int taskID);

private:

    // to be called from saving tasks
    void recordAction(unsigned int taskID);

    unsigned int round_;
    unsigned int nextCommand_;
    unsigned int nextID_;

    // saved to when added so can be loaded from file
    std::map<unsigned int, boost::shared_ptr<Macrotask> > tasks_;



    struct PastCommand
    {
        unsigned int cycle;
        unsigned int ID;
    };

    std::vector<PastCommand> toBeSaved_;

    struct FutureCommand
    {
        boost::shared_ptr<Macrotask> task;
        unsigned int cycle;
    };

    std::vector<FutureCommand> toBeExecuted_;





    Replayer(const Replayer& other);
    Replayer& operator=(const Replayer& other);
};

}


#endif // REPLAYER_HH
