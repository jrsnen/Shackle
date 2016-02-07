#ifndef WORKER_HH
#define WORKER_HH


#include <vector>

#include <boost/threadpool.hpp>


#include "../tasks/macrotask.hh"

namespace Shackle
{
    class Task;

    class Worker
    {
    public:
        Worker();
        ~Worker();

        /// \brief Executes task immediately
        ///
        /// \param task The task that is executed
        ///
        void executeTask(boost::shared_ptr<Task> task);


        /// \brief Adds task to queue to be executed in sequence with other tasks with same id
        ///
        /// \param task Task that is executed later
        /// \param id Tells which tasks cannot be executed at the same time
        ///
        void addToQueue(boost::shared_ptr<Task> task, unsigned int const id);

        /// \brief Executes all tasks in queue and waits untill the all tasks have been executed.
        ///
        void synchronize();



    private:

        unsigned int poolsize_;

        boost::threadpool::pool pool_;

        std::vector< boost::shared_ptr<Macrotask> > queues_;
    };


}
#endif // WORKER_HH
