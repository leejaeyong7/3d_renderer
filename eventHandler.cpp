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
    return false;
}

bool EventHandler::IsKeyDown(EKEY_CODE keyCode) const
{
    return KeyIsDown[keyCode];
}

void EventHandler::setActiveCamera(ICameraSceneNode* newActive)
{
    return;
}

bool EventHandler::isFPS()
{
    return false;
}

EventHandler::EventHandler()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    device = NULL;
}

EventHandler::EventHandler(IrrlichtDevice* dev)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    device = dev;
}
