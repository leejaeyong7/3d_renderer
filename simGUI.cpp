/*============================================================================
 * @author: Jae Yong Lee
 * @file: simGUI.cpp
 * @version:  
 * @summary:
 *      Definition file for simulator GUI
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "simGUI.h"
//----------------------------------------------------------------------------//
//                               Class Definitions
//----------------------------------------------------------------------------//

SimGUI::SimGUI(SimEngine * eng)
{
    // set engine pointer for engine method handling
    engine = eng;
    // sets IGUIEnvironment object
    // under gui namespace
    // grants access to GUI environment
    guienv = engine->getDevice()->getGUIEnvironment();
    
    // set event handler taking care of all events
    eh = new EventHandler(engine);
    engine->getDevice()->setEventReceiver(eh);
    
    // set GUI skin
    IGUISkin * skin = guienv->getSkin();
    IGUIFont * font = guienv->getFont("font/inconsolata.xml");
    skin->setFont(font);

    for (s32 i=0; i<EGDC_COUNT ; ++i)
    {
        SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }

    // set context menu
    setContextMenu();

}

void SimGUI::draw()
{
    if(!guienv)
        return;

    guienv->drawAll();
}

void SimGUI::promptAddEntity(u32 entityType)
{

}

void SimGUI::promptRemoveEntity(u32 entityType)
{

}

void SimGUI::setContextMenu()
{
    // add context menu(toolbar menu)
    IGUIContextMenu * cM = guienv->addMenu();

    //---------------------------Top Level Menu-------------------------------//
    // menu dropdown for context menu
    u32 engineMenuId = cM->addItem(
        // text of item
        L"Engine",
        // command id
        1,
        //activated
        true,
        // hassubmenu
        true,
        // checked
        false,
        //autochecking
        false
        );

    u32 entityMenuId = cM->addItem(
        // text of item
        L"Entity",
        // command id
        2,
        //activated
        true,
        // hassubmenu
        true,
        // checked
        false,
        //autochecking
        false
        );

    IGUIContextMenu * engineMenu = cM->getSubMenu(engineMenuId);
    IGUIContextMenu * entityMenu = cM->getSubMenu(entityMenuId);
    
    //---------------------------Engine Menu----------------------------------//

	u32 engineQuitId= engineMenu->addItem(
        // text of item
        L"Quit",
        // command id
        GUI_ID_QUIT_BUTTON,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );

    //---------------------------Entity Menu----------------------------------//
	u32 entityAddId = entityMenu->addItem(
        // text of item
        L"Add",
        // command id
        -1,
        // enabled
        true,
        // has sub menu
        true,
        // checked
        false,
        // autoChecking
        false
        );

	u32 entityRemoveId = entityMenu->addItem(
        // text of item
        L"Remove",
        // command id
        -1,
        // enabled
        true,
        // has sub menu
        true,
        // checked
        false,
        // autoChecking
        false
        );

    IGUIContextMenu * entityAdd = entityMenu->getSubMenu(entityAddId);
    IGUIContextMenu * entityRemove= entityMenu->getSubMenu(entityRemoveId);

    //----------------------Entity Add/Remove Sub Menu------------------------//

    u32 entityAddRobotId= entityAdd->addItem(
        // text of item
        L"Robot",
        // command id
        GUI_ID_ADD_ENTITY_ROBOT,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );

    u32 entityAddSensorId= entityAdd->addItem(
        // text of item
        L"Sensor",
        // command id
        GUI_ID_ADD_ENTITY_SENSOR,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );

    u32 entityAddEnvId= entityAdd->addItem(
        // text of item
        L"Environment",
        // command id
        GUI_ID_ADD_ENTITY_ENVIRONMENT,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );
    //---------------------REMOVE-----------------------//

    u32 entityRemoveRobotId= entityRemove->addItem(
        // text of item
        L"Robot",
        // command id
        GUI_ID_REMOVE_ENTITY_ROBOT,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );

    u32 entityRemoveSensorId= entityRemove->addItem(
        // text of item
        L"Sensor",
        // command id
        GUI_ID_REMOVE_ENTITY_SENSOR,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );

    u32 entityRemoveEnvId= entityRemove->addItem(
        // text of item
        L"Environment",
        // command id
        GUI_ID_REMOVE_ENTITY_ENVIRONMENT,
        // enabled
        true,
        // has sub menu
        false,
        // checked
        false,
        // autoChecking
        false
        );
}
