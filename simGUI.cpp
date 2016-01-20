/*============================================================================
 * @author: Jae Yong Lee
 * @file: simGUI.cpp
 * @version:  
 * @summary:
 *      Definition file for simulator GUI
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "simGUI.h"
//----------------------------------------------------------------------------//
//                               Class Definitions
//----------------------------------------------------------------------------//

SimGUI::SimGUI(SimEngine * eng)
{
    // set engine pointer for engine method handling
    engine = eng;
    // sets IGUIEnvironment object
    // under gui namespace
    // grants access to GUI environment
    guienv = engine->getDevice()->getGUIEnvironment();
    eh = new eventHandler(engine->getDevice());
    engine->getDevice()->setEventReceiver(eh);
    
    // quit button
    guienv->addButton(
        // position
        // x, y, x2, y2
        rect<s32>(10, 10, 10, 10),

        // parent (default null)
        0,
        // ID
        GUI_ID_QUIT_BUTTON, 
        // Button Caption
        L"Quit",
        // hover caption
        L"Exits Program");
    
}

void SimGUI::draw()
{
    if(!guienv)
        return;

    guienv->drawall();
}
