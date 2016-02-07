#include "rawinput.hh"
#include "rawinputtypes.hh"


#include "../common.hh"


#include <sstream>



namespace Shackle
{

struct RawMap
{
    Button button;
    OIS::KeyCode key;
};

const unsigned int SIZE = 101;
const RawMap map[] =
{
    {KESCAPE      , OIS::KC_ESCAPE},
    {K1           , OIS::KC_1},
    {K2           , OIS::KC_2},
    {K3           , OIS::KC_3},
    {K4           , OIS::KC_4},
    {K5           , OIS::KC_5},
    {K6           , OIS::KC_6},
    {K7           , OIS::KC_7},
    {K8           , OIS::KC_8},
    {K9           , OIS::KC_9},
    {K0           , OIS::KC_0},
    {KMINUS       , OIS::KC_MINUS },   // - on main keyboard
    {KEQUALS      , OIS::KC_EQUALS},
    {KBACK        , OIS::KC_BACK},   // backspace
    {KTAB         , OIS::KC_TAB},
    {KQ           , OIS::KC_Q},
    {KW           , OIS::KC_W},
    {KE           , OIS::KC_E},
    {KR           , OIS::KC_R},
    {KT           , OIS::KC_T},
    {KY           , OIS::KC_Y},
    {KU           , OIS::KC_U},
    {KI           , OIS::KC_I},
    {KO           , OIS::KC_O},
    {KP           , OIS::KC_P},
    {KLBRACKET    , OIS::KC_LBRACKET},
    {KRBRACKET    , OIS::KC_RBRACKET},
    {KRETURN      , OIS::KC_RETURN},    // Enter on main keyboard
    {KLCONTROL    , OIS::KC_LCONTROL},
    {KA           , OIS::KC_A},
    {KS           , OIS::KC_S},
    {KD           , OIS::KC_D},
    {KF           , OIS::KC_F},
    {KG           , OIS::KC_G},
    {KH           , OIS::KC_H},
    {KJ           , OIS::KC_J},
    {KK           , OIS::KC_K},
    {KL           , OIS::KC_L},
    {KSEMICOLON   , OIS::KC_SEMICOLON},
    {	KAPOSTROPHE , OIS::KC_APOSTROPHE},
    {	KGRAVE      ,  OIS::KC_GRAVE },   // accent
    {	KLSHIFT     ,  OIS::KC_LSHIFT},
    {	KBACKSLASH  ,  OIS::KC_BACKSLASH},
    {	KZ          ,  OIS::KC_Z},
    {	KX          ,  OIS::KC_X},
    {	KC          ,  OIS::KC_C},
    {	KV          ,  OIS::KC_V},
    {	KB          ,  OIS::KC_B},
    {	KN          ,  OIS::KC_N},
    {	KM          ,  OIS::KC_M},
    {	KCOMMA      ,  OIS::KC_COMMA},
    {	KPERIOD     ,  OIS::KC_PERIOD},   // . on main keyboard
    {	KSLASH      ,  OIS::KC_SLASH},  // / on main keyboard
    {	KRSHIFT     ,  OIS::KC_RSHIFT},
    {	KMULTIPLY   ,  OIS::KC_MULTIPLY },   // * on numeric keypad
    {	KLMENU      ,  OIS::KC_LMENU  },  // left Alt
    {	KSPACE      ,  OIS::KC_SPACE},
    {	KCAPITAL    ,  OIS::KC_CAPITAL},
    {	KF1          , OIS::KC_F1},
    {	KF2          , OIS::KC_F2},
    {	KF3         ,  OIS::KC_F3},
    {	KF4         ,  OIS::KC_F4},
    {	KF5         ,  OIS::KC_F5},
    {	KF6         ,  OIS::KC_F6},
    {	KF7         ,  OIS::KC_F7},
    {	KF8         ,  OIS::KC_F8},
    {	KF9         ,  OIS::KC_F9},
    {	KF10        , OIS::KC_F10},
    {	KNUMLOCK    ,  OIS::KC_NUMLOCK},
    {	KSCROLL     , OIS::KC_SCROLL },   // Scroll Lock
    {	KNUMPAD7    ,  OIS::KC_NUMPAD7},
    {	KNUMPAD8    ,  OIS::KC_NUMPAD8},
    {	KNUMPAD9    ,  OIS::KC_NUMPAD9},
    {	KSUBTRACT   ,  OIS::KC_SUBTRACT },   // - on numeric keypad
    {	KNUMPAD4    ,  OIS::KC_NUMPAD4},
    {	KNUMPAD5     , OIS::KC_NUMPAD5},
    {	KNUMPAD6    ,  OIS::KC_NUMPAD6},
    {	KADD        ,  OIS::KC_ADD },   // + on numeric keypad
    {	KNUMPAD1    ,  OIS::KC_NUMPAD1},
    {	KNUMPAD2    ,  OIS::KC_NUMPAD2},
    {	KNUMPAD3    ,  OIS::KC_NUMPAD3},
    {	KNUMPAD0    ,  OIS::KC_NUMPAD0},
    {	KDECIMAL    ,  OIS::KC_DECIMAL },   // . on numeric keypad
    {	KF11        ,  OIS::KC_F11},
    {	KF12        ,  OIS::KC_F12},
    {	KNUMPADENTER,  OIS::KC_NUMPADENTER },   // Enter on numeric keypad
    {	KRCONTROL   ,  OIS::KC_RCONTROL},
    {	KDIVIDE     ,  OIS::KC_DIVIDE },   // / on numeric keypad
    {	KSYSRQ      ,  OIS::KC_SYSRQ},
    {	KRMENU      ,  OIS::KC_RMENU } ,  // right Alt
    {	KPAUSE      ,  OIS::KC_PAUSE  },  // Pause
    {	KHOME       ,  OIS::KC_HOME   }, // Home on arrow keypad
    {	KUP         ,  OIS::KC_UP  },  // UpArrow on arrow keypad
    {	KPGUP       ,  OIS::KC_PGUP    },// PgUp on arrow keypad
    {	KLEFT       ,  OIS::KC_LEFT   }, // LeftArrow on arrow keypad
    {	KRIGHT      ,  OIS::KC_RIGHT  },  // RightArrow on arrow keypad
    {	KEND        ,  OIS::KC_END  },  // End on arrow keypad
    {	KDOWN       ,  OIS::KC_DOWN}  ,  // DownArrow on arrow keypad
    {	KPGDOWN     ,  OIS::KC_PGDOWN},    // PgDn on arrow keypad
    {	KINSERT     ,  OIS::KC_INSERT},    // Insert on arrow keypad
    {	KDELETE     ,  OIS::KC_DELETE}    // Delete on arrow keypad

};

RawInput::RawInput()
    :
    pInputMgr_(),
    pKeyboard_(),
    pMouse_()
{
    //ctor
}

RawInput::~RawInput()
{
    if(pInputMgr_)
    {
        OIS::InputManager::destroyInputSystem(pInputMgr_);
    }
    //ctor
}


void RawInput::init(Ogre::RenderWindow *renderWindow)
{

    unsigned long windowHandle = 0;
    renderWindow->getCustomAttribute("WINDOW", &windowHandle);


    width_ = renderWindow->getWidth();
    height_ = renderWindow->getHeight();

    OIS::ParamList paramList;

    std::string windowsHandleAsString="";

    std::ostringstream windowHndStr;
    windowHndStr << windowHandle;
    windowsHandleAsString = windowHndStr.str();

    paramList.insert(OIS::ParamList::value_type("WINDOW", windowsHandleAsString));

    pInputMgr_ = OIS::InputManager::createInputSystem(paramList);

#if defined OIS_WIN32_PLATFORM
    paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

    pKeyboard_ =
        static_cast<OIS::Keyboard*>(pInputMgr_->createInputObject(OIS::OISKeyboard, false)); //no buffering

    pMouse_ = static_cast<OIS::Mouse*>(pInputMgr_->createInputObject(OIS::OISMouse, true)); //no buffering

    pMouse_->getMouseState().height = height_;
    pMouse_->getMouseState().width  = width_;

    pMouse_->setEventCallback(this);

}


RawInputType* RawInput::getInput()
{
    tempInput_ = new RawInputType;
    pKeyboard_->capture();

    //compare if any key in actions_ is pressed down and execute those
    for(unsigned int ui = 0; ui < SIZE; ++ui)
    {
        if(pKeyboard_->isKeyDown(map[ui].key))
        {
            tempInput_->buttons.push_back(map[ui].button);
        }
    }

    pMouse_->capture();

    //assert(tempInput_->axes.size() > 0);
    RawInputType* r = tempInput_;
    tempInput_ = 0;
    return r;
}

bool RawInput::mouseMoved (const OIS::MouseEvent &arg)
{
    Ogre::LogManager::getSingleton().logMessage("mouse moved");
    Axis a = {MOUSEXREL, 0.0};
    a.value = float(arg.state.X.rel);
    tempInput_->axes.push_back(a);

    a.type = MOUSEYREL;
    a.value = float(arg.state.Y.rel);
    tempInput_->axes.push_back(a);
    return true;
}

bool RawInput::mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        tempInput_->buttons.push_back(MOUSELEFTPRESSED);
    }
    else if(id ==OIS::MB_Right)
    {
        tempInput_->buttons.push_back(MOUSERIGHTPRESSED);
    }
    return true;
}

bool RawInput::mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        tempInput_->buttons.push_back(MOUSELEFTRELEASED);
    }
    else if(id ==OIS::MB_Right)
    {
        tempInput_->buttons.push_back(MOUSERIGHTRELEASED);
    }
    return true;
}

}
