#ifndef SCRIPTER_HH
#define SCRIPTER_HH

extern "C"
{
   #include "lua.h"
   #include "lualib.h"
   #include "lauxlib.h"
}

#include <luabind/luabind.hpp>

#include <iostream>
#include <exception>

namespace Shackle
{


class Scripter
{
public:
    Scripter(): L_(lua_open())
    {
        luaL_openlibs(L_);
        luabind::open(L_);
    }

    ~Scripter()
    {
        lua_close(L_);
    }

    /// \brief Executes script file
    ///
    /// \param file A lua script file
    ///

    void dofile(std::string file)
    {
            luaL_dofile(L_, file.c_str());
    }

   inline operator lua_State*() const
   {
      return L_;
   }

private:

    lua_State* L_;


    Scripter(const Scripter& other);
    Scripter& operator=(const Scripter& other);
};

}
#endif // SCRIPTER_HH
