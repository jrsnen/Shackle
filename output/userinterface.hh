#ifndef USERINTERFACE_HH
#define USERINTERFACE_HH

#include "../filesystem/scripter.hh"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "boost/shared_ptr.hpp"

namespace Shackle
{
class GraphicsEngine;
class Scripter;
class Task;

class Userinterface
{
public:
    Userinterface();
    ~Userinterface();

    /// \brief Loads all user interface stuff expect layout
    ///
    void init();

    /// \brief Makes userinterface layout to match the layout specified in file
    ///
    /// \param file Layout file to be loaded
    void loadLayout(std::string file);

    /// \brief Updates mouse location on screen
    ///
    /// \param xRel How much mouse has moved in vertical direction since last update.
    /// \param uRel How much mouse has moved in horizontal direction since last update.
    void injectMouse(float xRel, float yRel);

    /// \brief Currently does nothing, may in future update mouse to make it smoother
    ///
    /// \param how much time has passed since last update
    void draw(float interpolation);


    CEGUI::System& giveSystem() const
    {
        return CEGUI::System::getSingleton();
    }

    CEGUI::Window* giveWindow() const
    {
        return window_;
    }

private:

    CEGUI::OgreRenderer& renderer_;
    CEGUI::WindowManager& wmgr_;
    CEGUI::Window* window_;

    float lastMouseX_;
    float lastMouseY_;

    float lastInterpolation_;



    Userinterface(const Userinterface& other);
    Userinterface& operator=(const Userinterface& other);
};


}
#endif // USERINTERFACE_HH
