
#ifndef RAWINPUTTYPES_HH
#define RAWINPUTTYPES_HH


#include <list>


namespace Shackle
{
    enum Button
    {
		KESCAPE    = 0,
		K1         = 1,
		K2         = 2,
		K3         = 3,
		K4         = 4,
		K5         = 5,
		K6         = 6,
		K7         = 7,
		K8         = 8,
		K9         = 9,
		K0         = 10,
		KMINUS         ,     // - on main keyboard
		KEQUALS        ,
		KBACK          ,    // backspace
		KTAB           ,
		KQ             ,
		KW             ,
		KE           ,
		KR           ,
		KT           ,
		KY           = 20,
		KU           ,
		KI           ,
		KO           ,
        KP           ,
		KLBRACKET    ,
        KRBRACKET    ,
		KRETURN      ,    // Enter on main keyboard
		KLCONTROL    ,
		KA           ,
		KS           = 30,
        KD           ,
		KF           ,
		KG           ,
		KH           ,
		KJ           ,
		KK           ,
		KL           ,
		KSEMICOLON   ,
		KAPOSTROPHE  ,
		KGRAVE       = 40,   // accent
		KLSHIFT      ,
		KBACKSLASH   ,
		KZ           ,
		KX           ,
		KC           ,
		KV           ,
		KB           ,
		KN           ,
		KM           ,
		KCOMMA       = 50,
		KPERIOD      ,  // . on main keyboard
		KSLASH       ,   // / on main keyboard
		KRSHIFT      ,
		KMULTIPLY    ,   // * on numeric keypad
		KLMENU       ,    // left Alt
		KSPACE       ,
        KCAPITAL     ,
		KF1          ,
        KF2          ,
		KF3          = 60,
		KF4          ,
		KF5          ,
		KF6          ,
		KF7          ,
		KF8          ,
		KF9          ,
		KF10         ,
		KNUMLOCK     ,
		KSCROLL      ,    // Scroll Lock
		KNUMPAD7     = 70,
		KNUMPAD8     ,
		KNUMPAD9     ,
		KSUBTRACT    ,    // - on numeric keypad
		KNUMPAD4     ,
		KNUMPAD5     ,
		KNUMPAD6     ,
		KADD         ,    // + on numeric keypad
		KNUMPAD1     ,
		KNUMPAD2     ,
		KNUMPAD3     = 80,
		KNUMPAD0     ,
		KDECIMAL     ,    // . on numeric keypad
		KF11         ,
		KF12         ,
		KNUMPADENTER ,    // Enter on numeric keypad
		KRCONTROL    ,
		KDIVIDE      ,    // / on numeric keypad
		KSYSRQ       ,
		KRMENU       ,    // right Alt
		KPAUSE       = 90,    // Pause
		KHOME        ,    // Home on arrow keypad
		KUP          ,    // UpArrow on arrow keypad
		KPGUP        ,    // PgUp on arrow keypad
		KLEFT        ,    // LeftArrow on arrow keypad
		KRIGHT       ,    // RightArrow on arrow keypad
		KEND         ,    // End on arrow keypad
		KDOWN        ,    // DownArrow on arrow keypad
		KPGDOWN      ,    // PgDn on arrow keypad
		KINSERT      ,    // Insert on arrow keypad
		KDELETE      = 100,    // Delete on arrow keypad
		MOUSELEFTPRESSED    ,
		MOUSELEFTRELEASED   ,
		MOUSERIGHTPRESSED   ,
		MOUSERIGHTRELEASED
	};

	enum AxisType
	{
	    MOUSEXREL      = 0x00,
	    MOUSEYREL      = 0x01,
	    MOUSEWHEELREL  = 0x02,
	    JOYSTICKX      = 0x03,
	    JOYSTICKY      = 0x04
	};

	struct Axis
	{
	    AxisType type;
	    float value;
	};

	struct RawInputType
	{
	    std::list<Button> buttons;
	    std::list<Axis> axes;
	};


}


#endif // RAWINPUTTYPES_HH
