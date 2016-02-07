#ifndef UINTTASK_HH
#define UINTTASK_HH

#include "../tasks/task.hh"

namespace Shackle
{
template <class Receiver>
class UIntTask : public Task
{
    public:
        typedef void (Receiver::* Action)(unsigned int);
        UIntTask(Receiver* r, Action a, unsigned int number);
        virtual ~UIntTask();

        virtual void run();
    private:


    Action action_;
    Receiver* receiver_;
    unsigned int number_;
    UIntTask(const UIntTask& other);
    UIntTask& operator=(const UIntTask& other);
};

template<class Receiver>
UIntTask<Receiver>::UIntTask(Receiver* r, Action a, unsigned int number)
    :
    action_(a),
    receiver_(r),
    number_(number)
{
    //ctor
}

template<class Receiver>
UIntTask<Receiver>::~UIntTask()
{
    //dtor
}

template<class Receiver>
void UIntTask<Receiver>::run()
{
    (receiver_->*action_)(number_);
}

}
#endif // UINTTASK_HH

