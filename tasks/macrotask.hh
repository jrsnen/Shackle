#ifndef MACROTASK_HH
#define MACROTASK_HH

#include <vector>



#include "task.hh"

namespace boost
{
    template <class T>
    class shared_ptr;
}
namespace Shackle
{

class Macrotask : public Task
{
public:
    Macrotask();
    Macrotask(boost::shared_ptr<Task> task);

    virtual ~Macrotask();

    virtual void run();

    //! \brief  Add task to be executed when this task is run
    void addTask(boost::shared_ptr<Task> task);

    //! \brief  Removes all tasks that have been added
    void clear();

private:
    std::vector<boost::shared_ptr<Task> > parts_;
};
}
#endif // MACROTASK_HH
