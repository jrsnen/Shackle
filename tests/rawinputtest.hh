#ifndef RAWINPUTTEST_HH
#define RAWINPUTTEST_HH

#include <cxxtest/TestSuite.h>

#include "../common.hh"
#include "../filesystem/scripter.hh"
#include "../output/graphicengine.hh"
#include "../filesystem/bind.hh"
#include "../input/rawinput.hh"
#include "../input/rawinputtypes.hh"


class RawInputTest : public CxxTest::TestSuite
{
public:

    void testEmpty(void)
    {
        Shackle::RawInput* r = new Shackle::RawInput;
        Shackle::Scripter scripter;
        Shackle::bind(scripter);
        Shackle::GraphicsEngine* graphics = new Shackle::GraphicsEngine;
        graphics->init(scripter);

        r->init(graphics->giveRenderWnd());
        Shackle::RawInputType* input;

        input = r->getInput();

        TS_ASSERT_EQUALS(input->buttons.size(), 0);
        //TS_ASSERT_EQUALS(input->axes.size(), 2);

        //order of deleting matters
        delete r;
        delete graphics;
    }
private:
};

#endif // RAWINPUTTEST_HH
