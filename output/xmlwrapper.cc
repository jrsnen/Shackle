#include "xmlwrapper.hh"

#include <OgreStringConverter.h>
#include <OgreLogManager.h>

#include <cassert>

namespace Shackle
{

XMLWrapper::XMLWrapper():
    followingNode_(0),
    child_(true),
    nodeStack_(),
    xmlDoc_()   // character type defaults to char
{
    //ctor
}

XMLWrapper::~XMLWrapper()
{
    //dtor
}

void XMLWrapper::init()
{}

void XMLWrapper::cleanup()
{
    nodeStack_.clear();
}

void XMLWrapper::parse(char* scene)
{
    xmlDoc_.parse<0>(scene);
}
/*
bool XMLWrapper::nextNode(std::string& name)
{
    if(nodeStack_.empty())
    {
        followingNode_ = xmlDoc_.first_node();
    }

    if(!followingNode_)
    {
        return false;
    }

    nodeStack_.push_back(followingNode_);
    name = followingNode_->name();

    updateFollowing();
    return true;
}
*/


bool XMLWrapper::nextNode(std::string name)
{
    if(followingNode_ && followingNode_->name() == name)
    {

        if(nodeStack_.empty())
        {
            followingNode_ = xmlDoc_.first_node();
        }

        nodeStack_.push_back(followingNode_);

        updateFollowing();

        return true;
    }
    return false;
}
/*
bool XMLWrapper::nodeName(std::string name)
{
    return followingNode_ != 0 && name == followingNode_->name();
}

bool XMLWrapper::isChild()
{
    return child_;
}
*/
Ogre::String XMLWrapper::getAttrib(const Ogre::String &attrib,
                                   const Ogre::String &defaultValue)
{
    if(nodeStack_.back()->first_attribute(attrib.c_str()))
        return nodeStack_.back()->first_attribute(attrib.c_str())->value();

    return defaultValue;
}

Ogre::Real XMLWrapper::getAttribReal(const Ogre::String &attrib,
                                     Ogre::Real defaultValue)
{
    if(nodeStack_.back()->first_attribute(attrib.c_str()))
        return Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute
                                                (attrib.c_str())->value());
    return defaultValue;
}

int XMLWrapper::getAttribInt(const Ogre::String &attrib,
                             int defaultValue)
{
    if(nodeStack_.back()->first_attribute(attrib.c_str()))
    {
        return Ogre::StringConverter::parseInt(nodeStack_.back()->first_attribute
                                               (attrib.c_str())->value());
    }
    return defaultValue;
}

bool XMLWrapper::getAttribBool(const Ogre::String &attrib, bool defaultValue)
{
    if(!nodeStack_.back()->first_attribute(attrib.c_str()))
        return defaultValue;

    return Ogre::String(nodeStack_.back()->first_attribute(attrib.c_str())->value()) == "true";
}


Ogre::Vector3 XMLWrapper::parseVector3()
{
    return Ogre::Vector3(
               Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("x")->value()),
               Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("y")->value()),
               Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("z")->value())
           );
}


Ogre::Quaternion XMLWrapper::parseQuaternion()
{
    //! @todo Fix this

    Ogre::Quaternion orientation;

    if(nodeStack_.back()->first_attribute("qx"))
    {
        orientation.x = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qz")->value());
        orientation.w = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qw")->value());
    }
    else if(nodeStack_.back()->first_attribute("qw"))
    {
        orientation.w = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qw")->value());
        orientation.x = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("qz")->value());
    }
    else if(nodeStack_.back()->first_attribute("axisX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("axisX")->value());
        axis.y = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("axisY")->value());
        axis.z = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("axisZ")->value());
        Ogre::Real angle = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("angle")->value());;
        orientation.FromAngleAxis(Ogre::Angle(angle), axis);
    }
    else if(nodeStack_.back()->first_attribute("angleX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("angleX")->value());
        axis.y = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("angleY")->value());
        axis.z = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("angleZ")->value());
        //orientation.FromAxes(&axis);
        //orientation.F
    }
    else if(nodeStack_.back()->first_attribute("x"))
    {
        orientation.x = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("z")->value());
        orientation.w = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("w")->value());
    }
    else if(nodeStack_.back()->first_attribute("w"))
    {
        orientation.w = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("w")->value());
        orientation.x = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("z")->value());
    }

    return orientation;
}

Ogre::ColourValue XMLWrapper::parseColour()
{
    return Ogre::ColourValue(
               Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("r")->value()),
               Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("g")->value()),
               Ogre::StringConverter::parseReal(nodeStack_.back()->first_attribute("b")->value()),
               nodeStack_.back()->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal
               (nodeStack_.back()->first_attribute("a")->value()) : 1
           );
}

void XMLWrapper::updateFollowing()
{
    followingNode_ = 0;

    // find the next following Node
    followingNode_ = nodeStack_.back()->first_node();
    child_ = true;
    while(!followingNode_)
    {
        child_= false;
        followingNode_ = nodeStack_.back()->next_sibling();
        if(!followingNode_)
        {
            nodeStack_.pop_back();
        }
    }
}

}
