/*============================================================================
 * @author: Jae Yong Lee
 * @file: eventHandler.h
 * @version: 1.0 
 * @summary:
 *      Declaration file for event handlers
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                               INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "simGUI.h"
#include "enumerations.h"
#include <iostream>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//----------------------------------------------------------------------------//
//                                 Global Variables 
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimGUI;
class EventHandler : public IEventReceiver
{
public:
    /**
     * Main Event Handler function for all events
     * @param SEvent - event struct that holds event information
     * @see SEvent
     * @return false
     */
    virtual bool OnEvent(const SEvent & event);

    /**
     * Checks current KeyIsDown array and returns keystate
     * @param EKEY_CODE - enumeration of key code(pressable keys)
     * @see EKEY_CODE
     * @return KeyIsDown value of given keycode
     */
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

    /**
     * Overloaded Constructor
     * Initializes KeyIsDown array to all false
     * Sets private device pointer
     */
    EventHandler(SimGUI* _gui);
private:
    // simGUI object
    SimGUI * gui;
    // private array that holds all key pressed data
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
    bool showing = true;
    
};
#endif
