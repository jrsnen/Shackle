


#ifndef BIND_HH
#define BIND_HH


namespace Shackle
{
    class Scripter;
/// \brief Binds all necessary classes so they can be used in scripts
///
/// \param scripter Scripting system to which classes are binded
///
///

void bind(Scripter& scripter);
}

#endif
