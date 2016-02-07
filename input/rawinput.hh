#ifndef RAWINPUT_HH
#define RAWINPUT_HH





#include <string>
#include <list>


#include "rawinputtypes.hh"

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISEvents.h>
#include <OIS/OISMouse.h>
#include <OGRE/OgreRenderWindow.h>

namespace Shackle
{
struct ScreenCoord;


class RawInput : public OIS::MouseListener
{
public:
    RawInput();
    ~RawInput();

    /// \brief Iniates input system
    ///
    /// \param renderWindow Window to which the system is attached
    ///
    void init(Ogre::RenderWindow *renderWindow);

    /// \brief Reads the input from keyboard an mouse and returns all buttons and axes
    /// \return Input of the system
    ///
    RawInputType* getInput();

    virtual bool 	mouseMoved (const OIS::MouseEvent &arg);
    virtual bool 	mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool 	mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id);




private:

    OIS::InputManager*		pInputMgr_;
    OIS::Keyboard*			pKeyboard_;
    OIS::Mouse*			    pMouse_;

    int width_;
    int height_;

    // for temporarily saving captured input
    RawInputType* tempInput_;



    RawInput(const RawInput& other);
    RawInput& operator=(const RawInput& other);
};



}
#endif // RAWINPUT_HH
