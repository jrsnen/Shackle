#ifndef SCRIPTERTEST_HH
#define SCRIPTERTEST_HH

#include <cxxtest/TestSuite.h>


#include <luabind/luabind.hpp>
#include <luabind/function.hpp>

#include "../filesystem/scripter.hh"

class Scriptertest : public CxxTest::TestSuite
{
    public:
        void testDoFile(void)
        {
            Shackle::Scripter l;

            int toDouble = 2;

            l.dofile("tests/scriptertest.lua");

            int answer = luabind::call_function<int>(l, "d", toDouble);

            TS_ASSERT_EQUALS(2*toDouble, answer);

            toDouble = 5;

            answer = luabind::call_function<int>(l, "d", toDouble);

            TS_ASSERT_EQUALS(2*toDouble, answer);
        }
};

#endif // SCRIPTERTEST_HH
