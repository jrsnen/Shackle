#ifndef INPUTTYPES_HH
#define INPUTTYPES_HH

#include <list>
#include <string>

namespace Shackle
{
/*
enum Action
{
    QUIT = 0,
    START = 1,
    LEFTMOUSEDOWN = 2,
    LEFTMOUSEUP = 3,
    RIGHTMOUSEDOWN = 4,
    RIGHTMOUSEUP = 5
};


enum State
{
    SCROLLUP = 0
};

enum RangeType
{
    MOUSEX = 0,
    MOUSEY
};
*/

typedef std::string Action;
typedef std::string State;
typedef std::string RangeType;

struct Range
{
    RangeType type;
    float value;
};

struct HighLevelInput
{
    std::list<Action> actions;
    std::list<State> states;
    std::list<Range> ranges;
};

}

#endif
