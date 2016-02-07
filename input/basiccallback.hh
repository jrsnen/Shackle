#ifndef BASICCALLBACK_HH
#define BASICCALLBACK_HH

#include "callback.hh"
#include "inputtypes.hh"

#include <boost/shared_ptr.hpp>


#include <map>

namespace Shackle
{
    class Task;


class BasicCallback : public Callback
{
    public:
        BasicCallback();
        virtual ~BasicCallback();

        virtual void handle(HighLevelInput* input);

        void addAction(Action action, boost::shared_ptr<Shackle::Task> task);

        void addState(State state, boost::shared_ptr<Shackle::Task> task);

    private:

        std::map<Action, boost::shared_ptr<Shackle::Task> > actions_;
        std::map<State, boost::shared_ptr<Shackle::Task> > states_;

        BasicCallback(const BasicCallback& other);
        BasicCallback& operator=(const BasicCallback& other);
};

}
#endif // BASICCALLBACK_HH
