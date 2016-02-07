

#include "bind.hh"
#include "scripter.hh"
#include "../core/gameengine.hh"
#include "../output/graphicengine.hh"
#include "../input/replayer.hh"
#include "../input/basiccontext.hh"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>


//#include <OIS/OISKeyboard.h>



namespace Shackle
{

void bind(Scripter& scripter)
{

    luabind::module(scripter)
    [
        luabind::class_<Graphicfiles>("Graphicfiles")
        .def_readwrite("configFileName", &Shackle::Graphicfiles::configFileName)
        .def_readwrite("pluginsFileName", &Shackle::Graphicfiles::pluginsFileName)
        .def_readwrite("logFileName", &Shackle::Graphicfiles::logFileName)
        .def_readwrite("resourceFileName", &Shackle::Graphicfiles::resourceFileName)
    ];

    luabind::module(scripter)
    [
        luabind::class_<Windowsettings>("Windowsettings")
        .def_readwrite("title", &Shackle::Windowsettings::title)
        .def_readwrite("customCapacities", &Shackle::Windowsettings::customCapacities)
    ];

    luabind::module(scripter)
    [
        luabind::class_<EngineSettings>("EngineSettings")
        .def_readwrite("ticksPerSecond", &Shackle::EngineSettings::ticksPerSecond)
        .def_readwrite("maxFrameSkip", &Shackle::EngineSettings::maxFrameSkip)
    ];

    luabind::module(scripter)
    [
        luabind::class_<Replayer>("Replayer")
        .def("luaAddTask", &Shackle::Replayer::luaAddTask)
    ];

    luabind::module(scripter)
    [
        luabind::class_<BasicContext>("BasicContext")
        .def("luaAddActionMap", &Shackle::BasicContext::luaAddActionMap)
        .def("luaAddStateMap", &Shackle::BasicContext::luaAddStateMap)
        .def("luaAddRangeMap", &Shackle::BasicContext::luaAddRangeMap)
    ];


}
}
