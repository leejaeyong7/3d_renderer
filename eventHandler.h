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
using namespace irr;

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
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
private:
    // private array that holds all key pressed data
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
#endif
