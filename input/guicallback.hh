#ifndef GUICALLBACK_HH
#define GUICALLBACK_HH

#include "callback.hh"

#include <boost/shared_ptr.hpp>

namespace Shackle
{
    class Userinterface;



class GuiCallback : public Callback
{
    public:
        GuiCallback(Shackle::Userinterface* gui);
        virtual ~GuiCallback();

        void handle(HighLevelInput* input);

    private:

        Shackle::Userinterface* gui_;



        GuiCallback(const GuiCallback& other);
        GuiCallback& operator=(const GuiCallback& other);
};

}

#endif // GUICALLBACK_HH
