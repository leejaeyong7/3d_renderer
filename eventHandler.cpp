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
    IrrlichtDevice * device = gui->device;
    ICameraSceneNode* camera = device->getSceneManager()->getActiveCamera();
    // KEYBOARD EVENT
    if(event.EventType == EET_KEY_INPUT_EVENT)
    {
        // on pressing T, toggle cursor and camera input
        if (event.KeyInput.Key== KEY_KEY_T &&
            event.KeyInput.PressedDown == false)
        {
            if(camera->isInputReceiverEnabled())
            {
                device->getCursorControl()->setVisible(true);
                camera->setInputReceiverEnabled(false);
            }
            else
            {
                device->getCursorControl()->setVisible(false);
                camera->setInputReceiverEnabled(true);
            }
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
        // on pressing Q, quit
        else if(event.KeyInput.Key == KEY_KEY_Q &&
                event.KeyInput.PressedDown == false)
        {
            device->closeDevice();
        }
        else
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    }
    // GUI EVENT
    else if (event.EventType == EET_GUI_EVENT)
    {

        IGUIElement * caller = event.GUIEvent.Caller;
        s32 id = caller->getID();
        IGUIEnvironment* env = device->getGUIEnvironment();

        switch(event.GUIEvent.EventType)
        {
            // button click event
        case EGET_BUTTON_CLICKED:
            switch(id)
            {
                // when close button is clicked, close it's parent
            case GUI_ID_CLOSE_BUTTON:
                caller->getParent()->remove();
                return false;
            case GUI_ID_ENTITY_PROMPT_CONFIRM_BUTTON:
                break;
            case GUI_ID_ENTITY_PROMPT_REMOVE_BUTTON:
                return false;
            default:
                return false;
            }
        case EGET_MENU_ITEM_SELECTED:
        {
            s32 sid;
            sid = ((IGUIContextMenu*)(caller))->getSelectedItem();

            if(sid != -1)
                id =((IGUIContextMenu*)(caller))->getItemCommandId(sid);

            switch(id)
            {
            case GUI_ID_QUIT_BUTTON:
                device->closeDevice();
                return true;
            case GUI_ID_ADD_ENTITY_ROBOT:
                gui->promptWindow(ADD_ENTITY_PROMPT,ENTITY_TYPE_ROBOT);
                return true;
            case GUI_ID_ADD_ENTITY_SENSOR:
                gui->promptWindow(ADD_ENTITY_PROMPT,ENTITY_TYPE_SENSOR);
                return true;
            case GUI_ID_ADD_ENTITY_ENVIRONMENT:
                gui->promptWindow(EDIT_ENTITY_PROMPT,ENTITY_TYPE_ENVIRONMENT);
                return true;
            case GUI_ID_EDIT_ENTITY_ROBOT:
                gui->promptWindow(EDIT_ENTITY_PROMPT,ENTITY_TYPE_ROBOT);
                return true;
            case GUI_ID_EDIT_ENTITY_SENSOR:
                gui->promptWindow(EDIT_ENTITY_PROMPT,ENTITY_TYPE_SENSOR);
                return true;
            case GUI_ID_EDIT_ENTITY_ENVIRONMENT:
                gui->promptWindow(EDIT_ENTITY_PROMPT,ENTITY_TYPE_ENVIRONMENT);
                return true;
            default:
                return false;
            }
            return false;
        }
        case EGET_COMBO_BOX_CHANGED:
            s32 sid;
            sid = ((IGUIComboBox*)(caller))->getSelected();
            switch(caller->getID())
            {
            default:
                break;
            }
            break;
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

EventHandler::EventHandler(SimGUI * _gui)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    gui = _gui;
}
