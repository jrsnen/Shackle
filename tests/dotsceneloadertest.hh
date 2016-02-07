#ifndef DOTSCENELOADERTEST_HH
#define DOTSCENELOADERTEST_HH


#include "output/graphicengine.hh"
#include "output/dotsceneloader.hh"



#include <cxxtest/TestSuite.h>


class DotSceneLoaderTest : public CxxTest::TestSuite
{
    public:

    void setUp(void)
    {
        std::cout << "Iniating DotSceneLoader test" << std::endl;

        graphics_ = new Shackle::GraphicsEngine;

        Shackle::Scripter s;
        Shackle::bind (s);
        graphics_->init(s);


        std::cout << "Iniating loader" << std::endl;
        loader_.init();
        std::cout << "Test iniated" << std::endl;

    }

    void tearDown()
    {
        loader_.cleanup();
        graphics_->cleanup();
        delete graphics_;

    }

    void testParseDotScene()
    {
        std::cout << "ParseDotScene test" << std::endl;
        TS_ASSERT_THROWS_NOTHING(loader_.parseDotScene("basic.scene", "General", graphics_->giveSceneMgr()));
        std::cout << "rendering" << std::endl;
        TS_ASSERT_THROWS_NOTHING(graphics_->render());
        std::cout << "Test finished" << std::endl;
    }

    private:

    Shackle::DotSceneLoader loader_;
    Shackle::GraphicsEngine* graphics_;


};

#endif // DOTSCENELOADERTEST_HH
