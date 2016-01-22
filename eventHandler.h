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

    // used for add prompt
    GUI_ID_ADD_ENTITY_WINDOW,

    GUI_ID_ADD_ENTITY_WINDOW_COMBO,
    GUI_ID_ADD_ENTITY_CREATE_BUTTON,
    GUI_ID_ADD_ENTITY_CLOSE_BUTTON,

    GUI_ID_ADD_ENTITY_POS_X,
    GUI_ID_ADD_ENTITY_POS_Y,
    GUI_ID_ADD_ENTITY_POS_Z,
    GUI_ID_ADD_ENTITY_ROT_A,
    GUI_ID_ADD_ENTITY_ROT_B,
    GUI_ID_ADD_ENTITY_ROT_C,

    GUI_ID_ADD_ENTITY_NAME,

    GUI_ID_ADD_ENTITY_ROBOT,
    GUI_ID_ADD_ENTITY_SENSOR,
    GUI_ID_ADD_ENTITY_ENVIRONMENT,

    // used for edit prompt
    GUI_ID_EDIT_ENTITY_WINDOW,

    GUI_ID_EDIT_ENTITY_WINDOW_COMBO,
    GUI_ID_EDIT_ENTITY_REMOVE_BUTTON,
    GUI_ID_EDIT_ENTITY_CREATE_BUTTON,
    GUI_ID_EDIT_ENTITY_CLOSE_BUTTON,

    GUI_ID_EDIT_ENTITY_POS_X,
    GUI_ID_EDIT_ENTITY_POS_Y,
    GUI_ID_EDIT_ENTITY_POS_Z,
    GUI_ID_EDIT_ENTITY_ROT_A,
    GUI_ID_EDIT_ENTITY_ROT_B,
    GUI_ID_EDIT_ENTITY_ROT_C,

    GUI_ID_EDIT_ENTITY_NAME,

    GUI_ID_EDIT_ENTITY_ROBOT,
    GUI_ID_EDIT_ENTITY_SENSOR,
    GUI_ID_EDIT_ENTITY_ENVIRONMENT
};

enum
{
    ENTITY_TYPE_ROBOT = 0,
    ENTITY_TYPE_SENSOR = 1,
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
