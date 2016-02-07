#ifndef USERINTERFACETEST_HH
#define USERINTERFACETEST_HH

#include <cxxtest/TestSuite.h>

#include "../output/graphicengine.hh"
#include "../output/userinterface.hh"
#include "../filesystem/scripter.hh"

class Userinterfacetest : public CxxTest::TestSuite
{
public:

    void setUp(void)
    {
        Shackle::Scripter s;
        Shackle::bind(s);
        engine = new Shackle::GraphicsEngine;
        engine->init(s);
    }

    void tearDown(void)
    {
        delete engine;
    }

    void testConstructor(void)
    {
        Shackle::Userinterface gui;
    }

    void testInit(void)
    {

        Shackle::Userinterface gui;
        gui.init();
    }


private:
    Shackle::GraphicsEngine* engine;


};

#endif // USERINTERFACETEST_HH
