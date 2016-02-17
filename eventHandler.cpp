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
            if(camera->isInputReceiverEnabled())
            {
                device->closeDevice();
            }
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
        IGUIElement* rootelem = env->getRootGUIElement();

        switch(event.GUIEvent.EventType)
        {
        // button click event
        case EGET_BUTTON_CLICKED:
            switch(id)
            {
            // when close button is clicked, close it's parent
            case CLOSE_BUTTON:
                caller->getParent()->remove();
                return false;
            case CONFIRM_BUTTON:
                // sets parameter for entity
                if(gui->currPrompt == EDIT_ENTITY_PROMPT)
                {
                    gui->editEntityObject();
                }
                // create new object if add window
                else if(gui->currPrompt == ADD_ENTITY_PROMPT)
                {
                    gui->editEntityObject();
                    gui->createEntityObject();
                }
                else if(gui->currPrompt == ATTACH_ENTITY_PROMPT ||
                    gui->currPrompt == DETACH_ENTITY_PROMPT)
                {
                    gui->attachEntityObject();
                }
                gui->update();
                caller->getParent()->remove();
                // update
                break;
            case REMOVE_BUTTON:
                gui->engine->removeEntity(gui->currObj);
                caller->getParent()->remove();
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
            case QUIT_BUTTON:
                device->closeDevice();
                return true;
            case ADD_ROBOT:
                gui->currPrompt= ADD_ENTITY_PROMPT;
                gui->currType= ENTITY_TYPE_ROBOT;
                gui->promptEntityWindow();
                return true;
            case ADD_SENSOR:
                gui->currPrompt= ADD_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_SENSOR;
                gui->promptEntityWindow();
                return true;
            case ADD_ENVIRONMENT:
                gui->currPrompt= ADD_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_ENVIRONMENT;
                gui->promptEntityWindow();
                return true;
            case EDIT_ROBOT:
                gui->currPrompt= EDIT_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_ROBOT;
                gui->promptEntityWindow();
                return true;
            case EDIT_SENSOR:
                gui->currPrompt= EDIT_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_SENSOR;
                gui->promptEntityWindow();
                return true;
            case EDIT_ENVIRONMENT:
                gui->currPrompt= EDIT_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_ENVIRONMENT;
                gui->promptEntityWindow();
                return true;
            case ATTACH_ENTITY:
                gui->currPrompt = ATTACH_ENTITY_PROMPT;
                gui->entityAttachWindow();
                return true;
            case DETACH_ENTITY:
                gui->currPrompt = DETACH_ENTITY_PROMPT;
                gui->entityAttachWindow();
                return true;
            default:
                return false;
            }
            return false;
        }
        case EGET_COMBO_BOX_CHANGED:
        {
            s32 sid;
            sid = ((IGUIComboBox*)(caller))->getSelected();
            if(gui->currPrompt == EDIT_ENTITY_PROMPT)
            {
                gui->setEditPromptData(sid);
            }
            else if(gui->currPrompt == ADD_ENTITY_PROMPT)
            {
                gui->setAddPromptData(sid);
            }
            else if(gui->currPrompt == DETACH_ENTITY)
            {
                if(id == ATTACH_COMBO1)
                    gui->setDetachData(sid);
            }
            break;
        }
        case EGET_ELEMENT_CLOSED:
        {
            switch(id)
            {
            case PROMPT_ADD_WINDOW:
            {
                if(gui->currObj)
                {
                    delete gui->currObj;
                    gui->currObj = 0;
                }
                gui->currType = 0;
                gui->currPrompt = 0;
                break;
            }
            case PROMPT_EDIT_WINDOW:
            {
                gui->currObj = 0;
                gui->currType = 0;
                gui->currPrompt = 0;
                break;
            }
            case ATTACH_WINDOW:
            {
                gui->currPrompt = 0;
            }
            default:
                break;
            }
            break;
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

EventHandler::EventHandler(SimGUI * _gui)
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
    gui = _gui;
}
