#ifndef CONTEXT_HH
#define CONTEXT_HH

#include <list>
#include <vector>


#include "../input/rawinputtypes.hh"
#include "../input/inputtypes.hh"
#include "../input/callback.hh"

namespace Shackle
{



class Context
{
    public:
        Context(){}
        virtual ~Context(){}

        virtual void interprete(Shackle::RawInputType* input, HighLevelInput* highResult) = 0;


    private:

        Context(const Context& other);
        Context& operator=(const Context& other);
};

}

#endif // CONTEXT_HH
