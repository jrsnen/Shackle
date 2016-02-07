
#include "guicallback.hh"
#include "inputtypes.hh"


#include "../output/userinterface.hh"

#include <OGRE/OgreLogManager.h>

namespace Shackle
{


GuiCallback::GuiCallback(Shackle::Userinterface* gui)
    :
    gui_(gui)
{
    //ctor
}

GuiCallback::~GuiCallback()
{
    //dtor
}


void GuiCallback::handle(HighLevelInput* input)
{
    bool x = false;
    bool y = false;
    float xValue = 0.0;
    float yValue = 0.0;


    std::list<Range>::iterator i = input->ranges.end();
    while( i != input->ranges.begin())
    {
        --i;
        if((*i).type == "MOUSEX")
        {
            x = true;
            xValue = (*i).value;

        }
        else if((*i).type == "MOUSEY")
        {
            y = true;
            yValue = (*i).value;
        }
    }

    if(x && y)
    {
        Ogre::LogManager::getSingleton().logMessage("injecting mouse movement for gui");
        gui_->injectMouse(xValue, yValue);
    }

    std::list<Action>::iterator j = input->actions.end();
    while( j != input->actions.begin())
    {
        --j;

        if(*j == "LEFTMOUSEDOWN" && gui_->giveSystem().injectMouseButtonDown(CEGUI::LeftButton))
        {
            input->actions.erase(j);
        }
        else if(*j == "LEFTMOUSEUP" && gui_->giveSystem().injectMouseButtonUp(CEGUI::LeftButton))
        {
            input->actions.erase(j);
        }
    }




}

}
