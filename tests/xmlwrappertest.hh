#ifndef XMLWRAPPERTEST_HH
#define XMLWRAPPERTEST_HH



#include "output/xmlwrapper.hh"
#include "output/graphicengine.hh"
#include "filesystem/bind.hh"
#include "filesystem/scripter.hh"

#include <cxxtest/TestSuite.h>



#include <OgreResourceGroupManager.h>




class XMLWrapperTest : public CxxTest::TestSuite
{
public:

    void setUp(void)
    {
        std::cout << "Wrapper test setup" << std::endl;
        Ogre::String groupName = "General";
        Ogre::String sceneName = "basic.scene";

        Shackle::Scripter s;
        Shackle::bind (s);
        graphics_ = new Shackle::GraphicsEngine;
        graphics_->init(s);

        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().
                                     openResource( sceneName, groupName );

        char* scene = strdup(stream->getAsString().c_str());

        TS_ASSERT_THROWS_NOTHING(wrapper_.init());
        TS_ASSERT_THROWS_NOTHING(wrapper_.parse(scene));

        std::cout << "Setup finished" << std::endl;
    }

    void tearDown(void)
    {
        TS_ASSERT_THROWS_NOTHING(wrapper_.cleanup());
        delete graphics_;

    }


    void testPushPop(void)
    {
        std::cout << "PushPop test start" << std::endl;

        TS_ASSERT(wrapper_.pushChild("scene"));
        TS_ASSERT(wrapper_.pushChild("nodes"));
        TS_ASSERT(wrapper_.pushChild("node"));
        TS_ASSERT(!wrapper_.pushChild("notreal"));
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());

        TS_ASSERT(!wrapper_.pushChild("notreal"));

        std::cout << "PushPop test finish" << std::endl;
    }

    void testAttrib(void)
    {
        wrapper_.pushChild("scene");
        wrapper_.pushChild("nodes");
        wrapper_.pushChild("node");
        wrapper_.pushChild("rotation");

        TS_ASSERT_EQUALS(wrapper_.getAttrib("qx"), "0.412257" );

        Ogre::Real real = 0.412257;
        TS_ASSERT_EQUALS(wrapper_.getAttribReal("qx"),  real);

        wrapper_.popNode();

        wrapper_.pushChild("light");
        wrapper_.pushChild("lightAttenuation");

        TS_ASSERT_EQUALS(wrapper_.getAttribReal("range"), 5000);

        wrapper_.popNode();
        wrapper_.popNode();
        wrapper_.pushChild("node");
        wrapper_.pushChild("entity");

        TS_ASSERT(!wrapper_.getAttribBool("anisotropic_friction"));
    }

    void testParse(void)
    {
        wrapper_.pushChild("scene");
        wrapper_.pushChild("nodes");
        wrapper_.pushChild("node");
        wrapper_.pushChild("position");

        Ogre::Vector3 vector;
        vector = wrapper_.parseVector3();

        Ogre::Real answer = 46.864777;
        TS_ASSERT_EQUALS(vector.x, answer);
        answer = 11.420194;
        TS_ASSERT_EQUALS(vector.y, answer);
        answer = 20.551025;
        TS_ASSERT_EQUALS(vector.z, answer);

        wrapper_.popNode();
        wrapper_.pushChild("rotation");

        Ogre::Quaternion q;
        q = wrapper_.parseQuaternion();

        answer = 0.412257;
        TS_ASSERT_EQUALS(q.x, answer);

        wrapper_.popNode();
        wrapper_.popNode();
        wrapper_.popNode();
        wrapper_.pushChild("environment");
        wrapper_.pushChild("colourBackground");

        Ogre::ColourValue c;
        c = wrapper_.parseColour();

        TS_ASSERT_EQUALS(c.b, 0.05087608844041824);
    }

    void testSibling(void)
    {
        std::cout << "Sibling test" << std::endl;

        wrapper_.pushChild("scene");
        wrapper_.pushChild("nodes");
        wrapper_.pushChild("node");

        TS_ASSERT(wrapper_.pushSibling("node"));
        TS_ASSERT(wrapper_.pushChild("position"));
        Ogre::Vector3 vector;
        vector = wrapper_.parseVector3();
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());

        TS_ASSERT(wrapper_.pushSibling("node"));
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());
        TS_ASSERT_THROWS_NOTHING(wrapper_.popNode());

    }

private:
    Shackle::XMLWrapper wrapper_;

    Shackle::GraphicsEngine* graphics_;

};

#endif // XMLWRAPPERTEST_HH
