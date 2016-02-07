#ifndef STRINGTASK_HH
#define STRINGTASK_HH

#include "task.hh"

namespace Shackle
{
template <class Receiver>
class StringTask : public Task
{
public:

    typedef void (Receiver::* Action)(std::string);


    StringTask(Receiver* r, Action a, std::string message);
    virtual ~StringTask();

    virtual void run();


private:


    Action action_;
    Receiver* receiver_;
    std::string message_;


    StringTask(const StringTask& other);
    StringTask& operator=(const StringTask& other);
};



template<class Receiver>
StringTask<Receiver>::StringTask(Receiver* r, Action a, std::string message)
    :
    action_(a),
    receiver_(r),
    message_(message)
{
    //ctor
}

template<class Receiver>
StringTask<Receiver>::~StringTask()
{
    //dtor
}

template<class Receiver>
void StringTask<Receiver>::run()
{
    (receiver_->*action_)(message_);
}



}
#endif // STRINGTASK_HH
