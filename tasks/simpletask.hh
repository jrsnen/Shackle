#ifndef SIMPLETASK_HH
#define SIMPLETASK_HH

#include "../tasks/task.hh"

namespace Shackle
{
    template <class Receiver>
    class Simpletask : public Task
    {
    public:
        typedef void (Receiver::* Action)();

        Simpletask(Receiver* r, Action a);
        virtual ~Simpletask();

        virtual void run();

    private:
    Action action_;
    Receiver* receiver_;

        Simpletask(const Simpletask& other);
        Simpletask& operator=(const Simpletask& other);
    };



template<class Receiver>
Simpletask<Receiver>::Simpletask(Receiver* r, Action a):action_(a), receiver_(r)
{
    //ctor
}

template<class Receiver>
Simpletask<Receiver>::~Simpletask()
{
    //dtor
}

template<class Receiver>
void Simpletask<Receiver>::run()
{
    (receiver_->*action_)();
}
}



#endif // SIMPLETASK_HH
