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
                showing = false;
            }
            else
            {
                device->getCursorControl()->setVisible(false);
                camera->setInputReceiverEnabled(true);
                showing = true;
            }
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
        if(showing)
        {
            // on pressing Q, quit
            if(event.KeyInput.Key == KEY_KEY_Q &&
               event.KeyInput.PressedDown == false)
            {
                if(camera->isInputReceiverEnabled())
                {
                    device->closeDevice();
                }
            }
            else if(event.KeyInput.Key == KEY_SPACE &&
                    event.KeyInput.PressedDown == false)
            {
                gui->addPathNode();
            }
            else
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
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
            {
                caller->getParent()->remove();
                showing = true;
                return false;
            }
            case RUN_BUTTON:
                gui->execPath();
                return false;
            case APPLY_BUTTON:
                gui->savePathData();
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
                showing = true;
                // update
                break;
            case PATH_REMOVE_BUTTON:
            {
                IGUIComboBox* cb = 
                    (IGUIComboBox*)(rootelem->getElementFromId(PATH_COMBO,true));
                gui->paths-> removePathNode(cb->getSelected());

                cb->clear();
                std::list<PathNode>* plist = gui->paths->getPathList();
                std::list<PathNode>::iterator it;
                int index = 0;
                for(it = plist->begin(); it != plist->end() ; it++)
                {
                    wstring p = L"Path ";
                    p += std::to_wstring(index);
                    cb->addItem(p.c_str(),index);
                    index++;
                }
                gui->setPathData(0);
                
            }
                return false;
            case REMOVE_BUTTON:
                gui->engine->removeEntity(gui->currObj);
                caller->getParent()->remove();
                showing = true;
                return false;
            case CAPTURE_BUTTON:
                gui->capture();
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
            case FEATURE_BUTTON:
            {
            IGUIContextMenu* cm = ((IGUIContextMenu*)(caller));
            if(cm->isItemChecked(sid))
            {
                cm->setItemChecked(sid,false);
                
                vector<SimSceneNode*>* v = &(gui->entityMeshVector);
                for(int i = 0; i < v->size(); i++)
                {
                    v->at(i)->setDrawFeature(false);
                }
            }
            else
            {
                cm->setItemChecked(sid,true);
                vector<SimSceneNode*>* v = &(gui->entityMeshVector);
                for(int i = 0; i < v->size(); i++)
                {
                    v->at(i)->setDrawFeature(true);
                }
            }
            }
                    return false;
            case QUIT_BUTTON:
                device->closeDevice();
                return true;
            case ADD_ROBOT:
                gui->currPrompt= ADD_ENTITY_PROMPT;
                gui->currType= ENTITY_TYPE_ROBOT;
                gui->promptEntityWindow();
                showing = false;
                return true;
            case ADD_SENSOR:
                gui->currPrompt= ADD_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_SENSOR;
                gui->promptEntityWindow();
                showing = false;
                return true;
            case ADD_ENVIRONMENT:
                gui->currPrompt= ADD_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_ENVIRONMENT;
                gui->promptEntityWindow();
                showing = false;
                return true;
            case EDIT_ROBOT:
                gui->currPrompt= EDIT_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_ROBOT;
                gui->promptEntityWindow();
                showing = false;
                return true;
            case EDIT_SENSOR:
                gui->currPrompt= EDIT_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_SENSOR;
                gui->promptEntityWindow();
                showing = false;
                return true;
            case EDIT_ENVIRONMENT:
                gui->currPrompt= EDIT_ENTITY_PROMPT;
                gui->currType = ENTITY_TYPE_ENVIRONMENT;
                gui->promptEntityWindow();
                showing = false;
                return true;
            case ATTACH_ENTITY:
                gui->currPrompt = ATTACH_ENTITY_PROMPT;
                gui->entityAttachWindow();
                showing = false;
                return true;
            case DETACH_ENTITY:
                gui->currPrompt = DETACH_ENTITY_PROMPT;
                gui->entityAttachWindow();
                showing = false;
                return true;
            case EDIT_PATH:
                gui->editPathWindow();
                showing = false;
            default:
                return false;
            }
            return false;
        }
        case EGET_COMBO_BOX_CHANGED:
        {
            s32 sid;
            sid = ((IGUIComboBox*)(caller))->getSelected();
            switch(id)
            {
            case CAMERA_COMBO:
            {

                u32 d;
                if(sid != -1)
                    d = ((IGUIComboBox*)(caller))->getItemData(sid);
                if(d == -1)
                {
                    if(gui->sc == camera)
                        gui->sc->setInputReceiverEnabled(false);
                    gui->wc->setInputReceiverEnabled(true);
                    gui->device->getSceneManager()->setActiveCamera(gui->wc);
                    ((CameraSceneNode*)gui->sc)->detachCamera();
                }
                else
                {
                vector<SimEntity*>* ev= gui->engine->getEntityVector();
                SimCamera* s = dynamic_cast<SimCamera*>(ev->at(d));
                if(s)
                {
                    if(gui->wc == camera)
                        gui->wc->setInputReceiverEnabled(false);
                    ((CameraSceneNode*)gui->sc)->attachCamera(s);
                    gui->sc->setInputReceiverEnabled(true);
                    gui->device->getSceneManager()->setActiveCamera(gui->sc);
                }

                }
            }
                break;
                
            case PROMPT_COMBO:
                if(gui->currPrompt == EDIT_ENTITY_PROMPT)
                    gui->setEditPromptData(sid);
                else if(gui->currPrompt == ADD_ENTITY_PROMPT)
                    gui->setAddPromptData(sid);
                break;
            case ATTACH_COMBO1:
                if(id == ATTACH_COMBO1)
                    gui->setDetachData(sid);
                break;
            case PATH_COMBO:
                gui->setPathData(sid);
                break;
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
        case EGET_SCROLL_BAR_CHANGED:
        {
            switch(id)
            {
            case PATH_FPS_SCROLL:
                gui->setPathFPS();
                return false;
            default:
                break;
            }
        }
            break;
        default:

            break;
        }
    }// end gui event
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
