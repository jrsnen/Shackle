

#include "userinterface.hh"
#include "graphicengine.hh"

#include "../tasks/task.hh"


namespace Shackle
{


Userinterface::Userinterface()
    :
    renderer_(CEGUI::OgreRenderer::bootstrapSystem()),
    wmgr_(CEGUI::WindowManager::getSingleton()),
    lastMouseX_(0.0),
    lastMouseY_(0.0),
    lastInterpolation_(0.0)
{}

Userinterface::~Userinterface()
{
    //dtor
    CEGUI::System::getSingleton().destroy();
}


void Userinterface::init()
{
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
    CEGUI::System::getSingleton().setDefaultMouseCursor("WindowsLook", "MouseArrow");

    CEGUI::System::getSingleton().injectMousePosition(0,0);
    CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());
}



void Userinterface::loadLayout(std::string file)
{
    window_ = wmgr_.loadWindowLayout(file);
    CEGUI::System::getSingleton().setGUISheet(window_);
}


void Userinterface::injectMouse(float xRel, float yRel)
{
    giveSystem().injectMouseMove (xRel, yRel);   //( xRel - lastInterpolation_*lastMouseX_, yRel - lastInterpolation_*lastMouseY_ );

    //lastMouseX_ = xRel;
    //lastMouseY_ = yRel;

    //lastInterpolation_ = 0.0;
}

void Userinterface::draw(float interpolation)
{
    //giveSystem().injectMouseMove( lastMouseX_ * (interpolation - lastInterpolation_),
     //                            lastMouseY_ * (interpolation - lastInterpolation_));

    //lastInterpolation_ = interpolation;
}

}
