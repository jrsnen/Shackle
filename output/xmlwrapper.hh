#ifndef XMLWRAPPER_HH
#define XMLWRAPPER_HH

#include "rapidxml.hpp"


#include <list>
#include <string>

#include <OgreString.h>

namespace Shackle
{


class XMLWrapper
{
    public:
        /** Default constructor */
        XMLWrapper();
        /** Default destructor */
        ~XMLWrapper();

        void init();
        void cleanup();

        void parse(char* scene);

        //bool nextNode(std::string& name);

        bool nextNode(std::string name);;

        /// \brief Tells whether next nodes is given name
        //bool nodeName(std::string name);

        //bool isChild();

        Ogre::String getAttrib(const Ogre::String &attrib,
                               const Ogre::String &defaultValue = "");

        Ogre::Real getAttribReal(const Ogre::String &attrib,
                                 Ogre::Real defaultValue = 0);

        int getAttribInt(const Ogre::String &attrib,
                        int defaultValue = 0);

        bool getAttribBool(const Ogre::String &parameter,
                           bool defaultValue = false);

        Ogre::Vector3 parseVector3();
        //Ogre::Vector3 parseRotation();

        Ogre::Quaternion parseQuaternion();
        Ogre::ColourValue parseColour();

    private:

        void updateFollowing();

        rapidxml::xml_node<>* followingNode_;
        bool child_;
        std::list<rapidxml::xml_node<>*> nodeStack_;
        rapidxml::xml_document<> xmlDoc_;    // character type defaults to char


        /// Here so they can't be used
        XMLWrapper(const XMLWrapper& other);
        XMLWrapper& operator=(const XMLWrapper& other);
};

}
#endif // XMLWRAPPER_HH
