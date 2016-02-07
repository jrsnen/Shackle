#ifndef GRAPHICSENGINETEST_HH
#define GRAPHICSENGINETEST_HH

#include <cxxtest/TestSuite.h>

#include "../filesystem/scripter.hh"
#include "../output/graphicengine.hh"
#include "../filesystem/bind.hh"

#include <iostream>


class Graphicsenginetest : public CxxTest::TestSuite
{
public:

    void testWindow(void)
    {

        Shackle::Scripter scripter;
        Shackle::bind(scripter);

        Shackle::GraphicsEngine g;

        g.init(scripter);

        g.render();


    }

};

#endif // GRAPHICSENGINETEST_HH
