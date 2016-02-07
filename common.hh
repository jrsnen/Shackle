
#ifndef COMMON_HH
#define COMMON_HH

#include <OGRE/OgreLogManager.h>

#include <cassert>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <fstream>


typedef std::ofstream OutFileStream;


namespace Shackle
{
    struct ScreenCoord
    {
        int x;
        int y;
    };


}


#endif
