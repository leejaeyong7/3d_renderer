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
    // creates GUI element of prompt window
    setAddPromptWindow(240,100,360,360);

    IGUIComboBox* combo = (IGUIComboBox*)(guienv->getRootGUIElement()
        ->getElementFromId(
            // id of gui element
            GUI_ID_ADD_ENTITY_WINDOW_COMBO,
            // search children
            true));
    // append sub entity for each entity type
    switch(entityType)
    {
    case ENTITY_TYPE_ROBOT:
        combo->addItem(L"-",entityType);
        combo->addItem(L"Quadrotor");
        combo->addItem(L"Ground Robot");
        break;
    case ENTITY_TYPE_SENSOR:
        combo->addItem(L"-",entityType);
        combo->addItem(L"Monocular Camera");
        combo->addItem(L"Depth Camera");
        break;
    case ENTITY_TYPE_ENVIRONMENT:
        combo->addItem(L"-",entityType);
        combo->addItem(L"April Tag");
        combo->addItem(L"Cube");
        combo->addItem(L"Sphere");
        break;
    default:
        break;
    }

}

void SimGUI::promptEditEntity(u32 entityType)
{
    // creates GUI element of prompt window
    setEditPromptWindow(240,100,360,360);
    vector<SimEntity*>* entityVector = engine->getEntityVector();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    // clear combo box and re fill them with entity names
    IGUIElement * cbb = rootelem->getElementFromId(
        GUI_ID_EDIT_ENTITY_WINDOW_COMBO,true);
    IGUIComboBox * cb = (IGUIComboBox*) cbb;
    cb->clear();
    cb->addItem(L"-",entityType);

    vector<SimEntity*>::iterator it;
    int counter = 0;
    for(it = entityVector->begin(); it!= entityVector->end(); ++it)
    {

    switch(entityType)
    {
    case ENTITY_TYPE_ROBOT:
    {
        //cout<< typeid(*it).name()<<endl;
        std::wstring eName;
        std::string eN;
        eN= typeid(*it).name();
        eName = std::wstring(eN.begin(),eN.end());
        rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_WINDOW,true)->setText(eName.c_str());
        if(eName.compare(L"P9SimEntity") == 0)
            cb->addItem((*it)->getName().c_str(),counter);
        break;
    }
    case ENTITY_TYPE_SENSOR:
        cb->addItem((*it)->getName().c_str(),counter);
        break;
    case ENTITY_TYPE_ENVIRONMENT:
        cb->addItem((*it)->getName().c_str(),counter);
        break;
    default:
        break;
    }
        counter++;
    }


}

SimEntity* SimGUI::createEntityObject()
{
    IGUIElement * rootelem = guienv->getRootGUIElement();
    if(!rootelem->getElementFromId(GUI_ID_ADD_ENTITY_WINDOW,true))
        return 0;
    stringc str;    
    stringw name;
    f32 x,y,z,a,b,c;
    str = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_POS_X,true)
        ->getText();
    x = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_POS_Y,true)
        ->getText();
    y = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_POS_Z,true)
        ->getText();
    z = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_ROT_A,true)
        ->getText();
    a = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_ROT_B,true)
        ->getText();
    b = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_ROT_C,true)
        ->getText();
    c = (f32)atof(str.c_str());
    name = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_NAME,true)
        ->getText();
    IGUIComboBox * cb = (IGUIComboBox*)(rootelem->getElementFromId(
                                            GUI_ID_ADD_ENTITY_WINDOW_COMBO,
                                            true));
    u32 entityType = cb->getItemData(0);
    u32 subEntityType = cb->getItemData(cb->getSelected());
    switch(entityType)
    {
    case ENTITY_TYPE_ROBOT:
    {
        SimEntity * newSimEntity = new SimEntity(x,y,z,a,b,c,name);
        return newSimEntity;
        // SimEntity * newSimEntity = new SimEntity(x,y,z,a,b,c,name);
        // return newSimEntity;
    }
    case ENTITY_TYPE_SENSOR:
    default:
    {
        return 0;
    }
    
    }
    
}

SimEntity* SimGUI::editEntityObject(SimEntity * obj)
{
    
    return obj;
}

bool SimGUI::checkEntityValid()
{
    IGUIElement * rootelem = guienv->getRootGUIElement();
    stringw name;
    if(rootelem->getElementFromId(GUI_ID_ADD_ENTITY_WINDOW,true))
    {
        name = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_NAME,true)
            ->getText();
    }
    else if(rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_WINDOW,true))
    {
        name = rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_NAME,true)
            ->getText();
    }
    else
    {
        return false;
    }
    // check name exists already
    bool nameMatch = true;
    vector<SimEntity*> * entityVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;
    for(it = entityVector->begin(); it != entityVector->end(); ++it)
    {
        if((*it)->getName() == name)
            nameMatch = false;
        nameMatch = nameMatch && true;
    }
    return nameMatch;
}

void SimGUI::alertCreationFailure(const wchar_t* message)
{
    IGUIWindow* window= guienv->addWindow(
        // window rectangle
        rect<s32>(
            300,
            250,
            540,
            350),
        // Modality
        true,
        // text
        L"Alert!",
        // parent
        0,
        // id
        GUI_ID_ADD_ENTITY_WINDOW);
    guienv->addStaticText(
        // text
        message,
        //
        rect<s32>(10, 20, 230, 50),
        // border
        false,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);
    IGUIButton * closeButton = guienv->addButton(
        rect<s32>(
            240/2+240/4+5,
            60,
            240-10,
            90
            ),
        window,
        GUI_ID_CLOSE_BUTTON,
        L"Close",
        L"Cancel and close window"
        );
}

void SimGUI::setAddPromptWindowEnabled(bool enabled){
    IGUIElement * rootelem = guienv->getRootGUIElement();
    if(!rootelem->getElementFromId(GUI_ID_ADD_ENTITY_WINDOW,true))
        return;
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_CREATE_BUTTON,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_POS_X,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_POS_Y,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_POS_Z,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_ROT_A,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_ROT_B,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_ROT_C,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_ADD_ENTITY_NAME,true)
        ->setEnabled(enabled);
}

void SimGUI::setEditPromptWindowEnabled(bool enabled){
    IGUIElement * rootelem = guienv->getRootGUIElement();
    if(!rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_WINDOW,true))
        return;
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_REMOVE_BUTTON,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_CREATE_BUTTON,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_POS_X,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_POS_Y,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_POS_Z,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_ROT_A,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_ROT_B,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_ROT_C,true)
        ->setEnabled(enabled);
    rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_NAME,true)
        ->setEnabled(enabled);
}
//----------------------------------------------------------------------------//
//                          Private Helper Functions
//----------------------------------------------------------------------------//
//                           Handles GUI positioning
//----------------------------------------------------------------------------//

void SimGUI::setAddPromptWindow(s32 x, s32 y, s32 w, s32 h)
{
    s32 window_pos_x = x;
    s32 window_pos_y = y;
    s32 window_width = w;
    s32 window_height = h;
    // setup window
    IGUIWindow* window= guienv->addWindow(
        // window rectangle
        rect<s32>(
            window_pos_x,
            window_pos_y,
            window_pos_x+window_width,
            window_pos_y+window_height),
        // Modality
        true,
        // text
        L"Add Entity",
        // parent
        0,
        // id
        GUI_ID_ADD_ENTITY_WINDOW);

    guienv->addStaticText(
        // text
        L"Select Object to add",
        //
        rect<s32>(10, 20, window_width-10, 40),
        // border
        false,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);

    // add dropdown for choosing which sub-entity to add
    IGUIComboBox* cb = guienv->addComboBox(
        //rectangle
        rect<s32> (10,40,window_width-10,60),
        //parent
        window,
        //id
        GUI_ID_ADD_ENTITY_WINDOW_COMBO);

    

    s32 boxWidth = (window_width-30);
    // add text for coordinate/rotation inputs
    IGUIStaticText* ic = guienv->addStaticText(
        // text
        L"Input Coordinates",
        //
        rect<s32>(10, 80, window_width-10, 150),
        // border
        true,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);

    // add text for coordinate/rotation inputs
    IGUIStaticText* ir = guienv->addStaticText(
        // text
        L"Input Rotation",
        //
        rect<s32>(10, 160, window_width-10, 230),
        // border
        true,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);


    IGUIStaticText* sx = guienv->addStaticText(L"x:",
                                               rect<s32>(
                                                   5 + 10,
                                                   40,
                                                   5+(boxWidth/3),
                                                   60),
                                               false, true, ic);
    IGUIStaticText* sy = guienv->addStaticText(L"y:",
                                               rect<s32>(
                                                   5+(boxWidth/3)+ 10,
                                                   40,
                                                   5+(boxWidth/3)*2,
                                                   60),
                                               false, true, ic);
    IGUIStaticText* sz = guienv->addStaticText(L"z:",
                                               rect<s32>(
                                                   5+(boxWidth/3)*2 + 10,
                                                   40,
                                                   5+(boxWidth/3)*3,
                                                   60),
                                               false, true, ic);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sx,
        // ID
        GUI_ID_ADD_ENTITY_POS_X);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sy,
        // ID
        GUI_ID_ADD_ENTITY_POS_Y);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sz,
        // ID
        GUI_ID_ADD_ENTITY_POS_Z);

    IGUIStaticText* sa = guienv->addStaticText(L"a:",
                                               rect<s32>(
                                                   5 + 10,
                                                   40,
                                                   5+(boxWidth/3),
                                                   60),
                                               false, true, ir);
    IGUIStaticText* sb = guienv->addStaticText(L"b:",
                                               rect<s32>(
                                                   5+(boxWidth/3)+ 10,
                                                   40,
                                                   5+(boxWidth/3)*2,
                                                   60),
                                               false, true, ir);
    IGUIStaticText* sc = guienv->addStaticText(L"c:",
                                               rect<s32>(
                                                   5+(boxWidth/3)*2 + 10,
                                                   40,
                                                   5+(boxWidth/3)*3,
                                                   60),
                                               false, true, ir);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sa,
        // ID
        GUI_ID_ADD_ENTITY_ROT_A);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sb,
        // ID
        GUI_ID_ADD_ENTITY_ROT_B);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sc,
        // ID
        GUI_ID_ADD_ENTITY_ROT_C);

    IGUIStaticText * en = guienv->addStaticText(
        L"Input Entity Name",
        rect<s32>(
            10,
            240,
            window_width-10,
            290),
        true, true, window);
    guienv->addEditBox(
        // text
        L"-",
        // rectangle
        rect<s32>(20,20,(boxWidth)-20,40),
        //border
        true,
        // parent
        en,
        // ID
        GUI_ID_ADD_ENTITY_NAME);

    //setup buttons create/close
    IGUIButton * createButton = guienv->addButton(
        rect<s32>(
            window_width/2 + 10,
            310,
            window_width/2+window_width/4-5,
            350
            ),
        window,
        GUI_ID_ADD_ENTITY_CREATE_BUTTON,
        L"Create",
        L"Creates entity with above parameters"
        );
    IGUIButton * closeButton = guienv->addButton(
        rect<s32>(
            window_width/2+window_width/4+5,
            310,
            window_width-10,
            350
            ),
        window,
        GUI_ID_CLOSE_BUTTON,
        L"Close",
        L"Cancel and close window"
        );
    // disables all input until user selects sub entity
    setAddPromptWindowEnabled(false);
}
void SimGUI::setEditPromptWindow(s32 x, s32 y, s32 w, s32 h)
{
    s32 window_pos_x = x;
    s32 window_pos_y = y;
    s32 window_width = w;
    s32 window_height = h;

    // setup window
    IGUIWindow* window= guienv->addWindow(
        // window rectangle
        rect<s32>(
            window_pos_x,
            window_pos_y,
            window_pos_x+window_width,
            window_pos_y+window_height),
        // Modality
        true,
        // text
        L"Edit Entity",
        // parent
        0,
        // id
        GUI_ID_EDIT_ENTITY_WINDOW);

    guienv->addStaticText(
        // text
        L"Select Object to Edit",
        //
        rect<s32>(10, 20, window_width-10, 40),
        // border
        false,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);

    // add dropdown for choosing which sub-entity to add
    IGUIComboBox* cb = guienv->addComboBox(
        //rectangle
        rect<s32> (10,40,window_width-10,60),
        //parent
        window,
        //id
        GUI_ID_EDIT_ENTITY_WINDOW_COMBO);

    s32 boxWidth = (window_width-30);

    // add text for coordinate/rotation inputs
    IGUIStaticText* ic = guienv->addStaticText(
        // text
        L"Edit Coordinates",
        //
        rect<s32>(10, 80, window_width-10, 150),
        // border
        true,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);

    // add text for coordinate/rotation inputs
    IGUIStaticText* ir = guienv->addStaticText(
        // text
        L"Edit Rotation",
        //
        rect<s32>(10, 160, window_width-10, 230),
        // border
        true,
        // textwrap to multi lines
        true,
        // parent
        window,
        // id
        -1,
        // background fill
        false);


    IGUIStaticText* sx = guienv->addStaticText(L"x:",
                                               rect<s32>(
                                                   5 + 10,
                                                   40,
                                                   5+(boxWidth/3),
                                                   60),
                                               false, true, ic);
    IGUIStaticText* sy = guienv->addStaticText(L"y:",
                                               rect<s32>(
                                                   5+(boxWidth/3)+ 10,
                                                   40,
                                                   5+(boxWidth/3)*2,
                                                   60),
                                               false, true, ic);
    IGUIStaticText* sz = guienv->addStaticText(L"z:",
                                               rect<s32>(
                                                   5+(boxWidth/3)*2 + 10,
                                                   40,
                                                   5+(boxWidth/3)*3,
                                                   60),
                                               false, true, ic);

    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sx,
        // ID
        GUI_ID_EDIT_ENTITY_POS_X);

    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sy,
        // ID
        GUI_ID_EDIT_ENTITY_POS_Y);

    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sz,
        // ID
        GUI_ID_EDIT_ENTITY_POS_Z);

    IGUIStaticText* sa = guienv->addStaticText(L"a:",
                                               rect<s32>(
                                                   5 + 10,
                                                   40,
                                                   5+(boxWidth/3),
                                                   60),
                                               false, true, ir);
    IGUIStaticText* sb = guienv->addStaticText(L"b:",
                                               rect<s32>(
                                                   5+(boxWidth/3)+ 10,
                                                   40,
                                                   5+(boxWidth/3)*2,
                                                   60),
                                               false, true, ir);
    IGUIStaticText* sc = guienv->addStaticText(L"c:",
                                               rect<s32>(
                                                   5+(boxWidth/3)*2 + 10,
                                                   40,
                                                   5+(boxWidth/3)*3,
                                                   60),
                                               false, true, ir);

    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sa,
        // ID
        GUI_ID_EDIT_ENTITY_ROT_A);

    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sb,
        // ID
        GUI_ID_EDIT_ENTITY_ROT_B);

    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,0,(boxWidth/3)-20,20),
        //border
        true,
        // parent
        sc,
        // ID
        GUI_ID_EDIT_ENTITY_ROT_C);

    IGUIStaticText * en = guienv->addStaticText(
        L"Entity Name",
        rect<s32>(
            10,
            240,
            window_width-10,
            290),
        true, true, window);
    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(20,20,(boxWidth)-20,40),
        //border
        true,
        // parent
        en,
        // ID
        GUI_ID_EDIT_ENTITY_NAME);

    //setup buttons create/close

    IGUIButton * deleteButton = guienv->addButton(
        rect<s32>(
            window_width/2 -window_width/4+15,
            310,
            window_width/2,
            350
            ),
        window,
        GUI_ID_EDIT_ENTITY_REMOVE_BUTTON,
        L"Delete",
        L"Delete selected entity"
        );

    IGUIButton * createButton = guienv->addButton(
        rect<s32>(
            window_width/2 + 10,
            310,
            window_width/2+window_width/4-5,
            350
            ),
        window,
        GUI_ID_EDIT_ENTITY_CREATE_BUTTON,
        L"Set",
        L"Sets selected entity with above parameters"
        );
    IGUIButton * closeButton = guienv->addButton(
        rect<s32>(
            window_width/2+window_width/4+5,
            310,
            window_width-10,
            350
            ),
        window,
        GUI_ID_CLOSE_BUTTON,
        L"Close",
        L"Cancel and close window"
        );
    
    // disable all inputs until user selects object
    setEditPromptWindowEnabled(false);

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
        L"Edit",
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
        GUI_ID_EDIT_ENTITY_ROBOT,
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
        GUI_ID_EDIT_ENTITY_SENSOR,
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
        GUI_ID_EDIT_ENTITY_ENVIRONMENT,
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
