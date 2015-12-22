/*============================================================================
 * @author: Jae Yong Lee
 * @file: eventHandler.cpp
 * @version: 1.0 
 * @summary:
 *      Definition file for event handlers
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "eventHandler.h"
//----------------------------------------------------------------------------//
//                               Class Definitions
//----------------------------------------------------------------------------//
bool EventHandler::OnEvent(const SEvent & event)
{
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    return false;
}

bool EventHandler::IsKeyDown(EKEY_CODE keyCode) const
{
    return KeyIsDown[keyCode];
}

EventHandler::EventHandler()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
}
