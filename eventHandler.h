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
#include <irrlicht.h>
#include <iostream>
#include "simEngine.h"
#include "simGUI.h"

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
// enum for GUI elements
enum
{
    GUI_ID_QUIT_BUTTON = 0x10000,
        


    GUI_ID_ADD_ENTITY_ROBOT,
    GUI_ID_ADD_ENTITY_SENSOR,
    GUI_ID_ADD_ENTITY_ENVIRONMENT,

    GUI_ID_REMOVE_ENTITY_ROBOT,
    GUI_ID_REMOVE_ENTITY_SENSOR,
    GUI_ID_REMOVE_ENTITY_ENVIRONMENT
};

enum
{
    ENTITY_TYPE_ROBOT = 1,
    ENTITY_TYPE_SENSOR,
    ENTITY_TYPE_ENVIRONMENT
};

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimEngine;
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
     * Constructor
     * Initializes KeyIsDown array to all false
     */
    EventHandler();

    /**
     * Overloaded Constructor
     * Initializes KeyIsDown array to all false
     * Sets private device pointer
     */
    EventHandler(SimEngine * eng);
private:
    SimEngine * engine;
    // private array that holds all key pressed data
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
    
};
#endif
