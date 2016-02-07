#ifndef BASICCONTEXT_HH
#define BASICCONTEXT_HH


#include <map>
#include <memory>
#include <string>
#include <boost/weak_ptr.hpp>


#include "context.hh"
#include "inputtypes.hh"
#include "rawinputtypes.hh"

namespace Shackle
{


struct ActionMap
{
    Shackle::Button button;
    Action action;
};

struct StateMap
{
    Shackle::Button button;
    State state;
};

struct RangeMap
{
    Shackle::AxisType axis;
    RangeType range;
};

class Scripter;

class BasicContext : public Context
{
public:
    BasicContext();

    virtual ~BasicContext();

    void loadFromFile(Shackle::Scripter& scripter, std::string file);

    // call this or load from file
    void setContext(std::vector<ActionMap>& actions,
                    std::vector<StateMap>& states,
                    std::vector<RangeMap>& ranges);



    void interprete(Shackle::RawInputType* input, HighLevelInput* highResult);

    // to be called from lua
    void luaAddActionMap(Shackle::Button button, Action action);

    void luaAddStateMap(Shackle::Button button, State state);

    void luaAddRangeMap(Shackle::AxisType axis, RangeType range);


private:

    struct ActionInstance
    {
        Action action;
        bool wasPressed;
        bool nowPressed;
    };

    std::vector<ActionMap> tempActions_;
    std::vector<StateMap> tempStates_;
    std::vector<RangeMap> tempRanges_;

    std::map<Shackle::Button, ActionInstance> actions_;
    std::map<Shackle::Button, State> states_;
    std::map<Shackle::AxisType, RangeType> ranges_;

    BasicContext(const BasicContext& other);
    BasicContext& operator=(const BasicContext& other);
};

}


#endif // BASICCONTEXT_HH
