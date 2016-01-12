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
    
    if(event.EventType == EET_KEY_INPUT_EVENT)
    {
        if (event.KeyInput.Key== KEY_KEY_T &&
            event.KeyInput.PressedDown == false)
        {
            if(context.device->getSceneManager()->getActiveCamera()
               == context.camera[0])
            {
                setActiveCamera(context.camera[1]);
                context.device->getCursorControl()->setVisible(true);
            }
            else
            {
                setActiveCamera(context.camera[0]);
                context.device->getCursorControl()->setVisible(false);
            }
            
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
        else
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    }
    else if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        IGUIEnvironment* env = context.device->getGUIEnvironment();

        switch(event.GUIEvent.EventType)
        {
        case EGET_BUTTON_CLICKED:
            switch(id)
            {
            case GUI_ID_QUIT_BUTTON:
                context.device->closeDevice();
                return true;
            default:
                return false;
            }
        default:
            break;

        }
    }
    return false;
}

bool EventHandler::IsKeyDown(EKEY_CODE keyCode) const
{
    return KeyIsDown[keyCode];
}

void EventHandler::setActiveCamera(ICameraSceneNode* newActive)
{
    if (0 == context.device)
        return;

    ICameraSceneNode * active = context.device->getSceneManager()->getActiveCamera();
    active->setInputReceiverEnabled(false);

    newActive->setInputReceiverEnabled(true);
    context.device->getSceneManager()->setActiveCamera(newActive);
}

EventHandler::EventHandler()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    context.device = NULL;
    context.camera[0] =0;
    context.camera[1] =0;
}

EventHandler::EventHandler(RenderingDevice & dev)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    context = dev;
}
