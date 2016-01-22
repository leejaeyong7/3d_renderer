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
    IrrlichtDevice * device = engine->getDevice();
    SimGUI* simGUI = engine->getSimGUI(); 
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
        case EGET_BUTTON_CLICKED:
            switch(id)
            {
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
                simGUI->promptAddEntity(ENTITY_TYPE_ROBOT);
                return true;
            case GUI_ID_ADD_ENTITY_SENSOR:
                simGUI->promptAddEntity(ENTITY_TYPE_SENSOR);
                return true;
            case GUI_ID_ADD_ENTITY_ENVIRONMENT:
                simGUI->promptAddEntity(ENTITY_TYPE_ENVIRONMENT);
                return true;
            case GUI_ID_EDIT_ENTITY_ROBOT:
                simGUI->promptEditEntity(ENTITY_TYPE_ROBOT);
                return true;
            case GUI_ID_EDIT_ENTITY_SENSOR:
                simGUI->promptEditEntity(ENTITY_TYPE_SENSOR);
                return true;
            case GUI_ID_EDIT_ENTITY_ENVIRONMENT:
                simGUI->promptEditEntity(ENTITY_TYPE_ENVIRONMENT);
                return true;
            default:
                return false;
            }
            return false;
        }
        case EGET_COMBO_BOX_CHANGED:
            s32 sid;
            sid = ((IGUIComboBox*)(caller))->getSelected();
            if(sid == 0)
            {
                switch(caller->getID())
                {
                case GUI_ID_ADD_ENTITY_WINDOW_COMBO:
                    simGUI->setAddPromptWindowEnabled(false);
                    break;
                case GUI_ID_EDIT_ENTITY_WINDOW_COMBO:
                    simGUI->setEditPromptWindowEnabled(false);
                    break;
                }
            }
            else
            {
                switch(caller->getID())
                {
                case GUI_ID_ADD_ENTITY_WINDOW_COMBO:
                    simGUI->setAddPromptWindowEnabled(true);
                    break;
                case GUI_ID_EDIT_ENTITY_WINDOW_COMBO:
                    simGUI->setEditPromptWindowEnabled(true);
                    break;
                }
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

EventHandler::EventHandler(SimEngine * eng)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    engine = eng;
}
