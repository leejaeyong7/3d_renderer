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

SimGUI::SimGUI(SimEngine * eng, const wchar_t * text,
               u32 m_width, u32 m_height,
               u32 m_width_r, u32 m_height_r,
               bool fullscreen)
{
    device = createDevice(
        // device type : opengl 
        // using video namespace
        // is able to choose between EDT_OPENGL, EDT_DIRECT3D8/9, etc
        EDT_OPENGL,

        // window size
        // dimension2d from core namespace
        // using macro values for window size
        dimension2d<u32>(m_width,m_height),

        // bits per pixel : 16 (default 16)
        // if not specified, set to 16
        16,

        // full screen mode : false (default false)
        false,

        // stencil buffer(??) : false (default false)
        false,

        // vsync : false (default false)
        false,

        // custom event handler
        0
        );
    width = m_width;
    height = m_height;
    width_r = m_width_r;
    height_r = m_height_r;

    // set window title
    device->setWindowCaption(text);

    // set engine pointer for engine method handling
    engine = eng;

    // sets IGUIEnvironment object
    // under gui namespace
    // grants access to GUI environment
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    
    // set event handler taking care of all events
    
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
    setup();

    // set context menu
    setContextMenu();

}

void SimGUI::setup()
{
    // set event handler
    EventHandler * eh = new EventHandler(this);
    device->setEventReceiver(eh);
    IVideoDriver * driver = device->getVideoDriver();
    ISceneManager * smgr = device->getSceneManager();

//----------------------------------------------------------------------------//
//                             Sky setup
//----------------------------------------------------------------------------//
    ITexture* skyTexture  = driver->getTexture("Textures/sky.png");
    smgr->addSkyDomeSceneNode(
        skyTexture);
        
//----------------------------------------------------------------------------//
//                             Floor setup
//----------------------------------------------------------------------------//
    SMaterial * floor_material = new SMaterial();
    ITexture* floorTexture= driver->getTexture("Textures/floor.png");
    if(floor_material)
    {
        // floor will be affected by ambient light
        floor_material->AmbientColor  = SColor(255, 110, 110, 110);
        floor_material->DiffuseColor  = SColor(255, 0, 0, 0);
        floor_material->SpecularColor = SColor(255, 0, 0, 0);
        // and emit gray-ish color
        floor_material->EmissiveColor = SColor(255, 110, 110, 110);
        floor_material->ColorMaterial = ECM_NONE;
        floor_material->setTexture(0,floorTexture);
    }
    // add floor for FPS like rendering
    // for basic testing only
    IAnimatedMesh * movingplane= smgr->addHillPlaneMesh(
        // mesh name
        "floor", 
        // tileSize
        dimension2d<f32>(15, 15),
        // tilecount
        dimension2d<u32>(40, 40),
        // material
        floor_material,
        // hillheight
        0.0f,
        // hill count
        dimension2df(0.0f,0.0f),
        // texture repeat count
        dimension2df(40.0f,40.0f)
        );
    IAnimatedMeshSceneNode *floor=smgr->addAnimatedMeshSceneNode(movingplane);

    // set global lighting (weak gray)
    smgr->setAmbientLight(SColorf(0.1f,0.1f,0.1f,0.1f));

    // add sun light lighting (diffusive light)
    ILightSceneNode* sun_light = smgr->addLightSceneNode(
        //parent node
        0, 
        //scenemanager
        vector3df(0,10000,0), 
        // sun color
        SColorf(0.906f,0.882f,0.488f),
        // sun position
        15000.0f,

        -1 );
    // add sun sphere for view
    ISceneNode * sun_node = smgr->addSphereSceneNode();
    if(sun_node)
    {
        // put sun up high
        sun_node->setPosition(vector3df(0,10000,0));
        // scale sun large
        sun_node->setScale(vector3df(100,100,100));
        // sun is not affected by any lighting
        sun_node->setMaterialFlag(EMF_LIGHTING,false);
    }


    SKeyMap keyMap[8];

    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    ICameraSceneNode* rc = smgr->addCameraSceneNodeFPS(
        // parent node : default null
        0, 
        // rotation speed : default 100
        30,
        // movement speed : default 0.5
        0.03, 
        // id
        -1,
        // key mapping input
        keyMap,
        // key mapping size
        8
        );

    // set camera position
    rc->setPosition(vector3df(0,5,0));
    // sets far value of camera
    // extended for sun support
    rc->setFarValue(20000.0f);
    // hides mouse
    device->getCursorControl()->setVisible(false);

    // set prompt window params
    // window pos / size
    px = 150;
    py = 100;
    pw = 600;
    window_offset = 20;

    // margin for all box(borders)
    boxMargin = 10;

    combo_box_x  = boxMargin;
    combo_box_y = window_offset+boxMargin;
    combo_box_w = pw - 2*boxMargin;
    combo_box_h = 20;

    combo_x = boxMargin;
    combo_y = combo_box_y + combo_box_h;
    combo_w = pw - 2*boxMargin;
    combo_h = 20;

    name_x = boxMargin;
    name_y = combo_y+combo_h+boxMargin;
    name_w = pw - 2*boxMargin; 
    name_h = 20;

    nameInputMargin = 5;
    nameText_w_offset = 80;

    boxInputMargin = 5;
    boxText_h_margin = 10;
    boxText_w_margin = 100;
    boxText_w_offset = 10;
    boxInputText_h = 20;
    boxInputText_w = pw - 2*boxMargin - boxText_w_margin;
    boxInputText_w /= 3;

    dof_x = boxMargin;
    dof_y = name_y + name_h + boxMargin;
    dof_w = pw - 2*boxMargin;
    dof_h = boxInputText_h * 2 + 
        boxText_h_margin + boxMargin*2;
    

    boxWidth = pw - 2*(boxInputMargin + boxMargin);
    boxInputText_w_offset = 40;

    advSetting_y = dof_y + dof_h + boxMargin; 
    advSetting_h = 200;
    buttons_y = advSetting_y + advSetting_h + boxMargin;
    buttons_h = 40;

    ph = buttons_y + buttons_h;
}

void SimGUI::draw()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    ISceneManager * smgr = device->getSceneManager();
    IVideoDriver * driver = device->getVideoDriver();
    if(device->run())
    {
        if(device->isWindowActive())
        {
            driver->beginScene(true,true,SColor(255,200,200,200));
            driver->setViewPort(rect<s32>(0,20,width_r,height_r));
            smgr->drawAll();
            driver->setViewPort(rect<s32>(0,0,width,height));
            guienv->drawAll();
            driver->endScene();
        }
        else
            device->yield();
    }
}

bool SimGUI::isRunning()
{
    return device->run();
}

void SimGUI::end()
{
    device->drop();
}

void SimGUI::addEntitySceneNode(EntityType type, SimEntity * obj)
{
    if(!obj)
        return;
    EntityMesh newEntityMesh;
    ISceneManager* smgr = device->getSceneManager();
    vector3df pos;
    vector3df rot;
    Position ePos = obj->getPosition();
    Rotation eRot = obj->getRotation();
    pos.X = ePos.X;
    pos.Y = ePos.Y;
    pos.Z = ePos.Z;
    
    rot.X = eRot.Pitch;
    rot.Y = eRot.Roll;
    rot.Z = eRot.Yaw;

    newEntityMesh.obj = obj;
    std::string pathname = obj->getMeshPath();
    irr::core::string<fschar_t> pname = pathname.c_str();
    if(pathname != "")
    {
        IAnimatedMesh* mesh = smgr->getMesh(pname);
        newEntityMesh.mesh = smgr->addMeshSceneNode(
            // mesh
            mesh->getMesh(0),
            //parent scene node
            0,
            // id
            -1,
            // position
            pos,
            // rotation
            rot
            // scale by default
            );
    }
    else
        newEntityMesh.mesh = 0;
    entityMeshVector.push_back(newEntityMesh);
}
void SimGUI::removeEntitySceneNode(SimEntity * obj)
{
    if(!obj)
        return;
    entityMeshVector.erase(
        std::remove_if(entityMeshVector.begin(),
                       entityMeshVector.end(),
                       checkEntityPointer(obj)),
        entityMeshVector.end());
}


SimEntity* SimGUI::createEntityObject()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    ISceneManager* smgr = device->getSceneManager();
    if(!rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_WINDOW,true))
        return 0;
    stringc str;    
    stringw name;
    f32 x,y,z,a,b,c;
    str = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_POS_X,true)
        ->getText();
    x = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_POS_Y,true)
        ->getText();
    y = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_POS_Z,true)
        ->getText();
    z = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_ROT_A,true)
        ->getText();
    a = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_ROT_B,true)
        ->getText();
    b = (f32)atof(str.c_str());
    str = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_ROT_C,true)
        ->getText();
    c = (f32)atof(str.c_str());
    name = rootelem->getElementFromId(GUI_ID_ENTITY_PROMPT_NAME,true)
        ->getText();
    IGUIComboBox * cb = (IGUIComboBox*)(rootelem->getElementFromId(
                                            GUI_ID_ENTITY_PROMPT_COMBO,
                                            true));
    u32 entityType = cb->getItemData(0);
    u32 subEntityType = cb->getItemData(cb->getSelected());
    switch(entityType)
    {
    case ENTITY_TYPE_ROBOT:
    {
        switch(subEntityType)
        {
        case SUB_ENTITY_ROBOT_QUAD:
        {
            return 0;
        }
        case SUB_ENTITY_ROBOT_GROUND:
        {
            return 0;
        }
        default:
            return 0;
        }
    }
    case ENTITY_TYPE_SENSOR:
    {
        switch(subEntityType)
        {
        case SUB_ENTITY_SENSOR_CAM:
        {
            return 0;
        }
        default:
            return 0;
        }
    }
    case ENTITY_TYPE_ENVIRONMENT:
    {
        return 0;
    }
    default:
    {
        return 0;
    }
    
    }
    return 0;
    
}

SimEntity* SimGUI::editEntityObject(SimEntity * obj)
{
    return obj;
}

bool SimGUI::checkEntityValid()
{
    // IGUIEnvironment* guienv = device->getGUIEnvironment();
    // IGUIElement * rootelem = guienv->getRootGUIElement();
    // wstring name;
    // if(rootelem->getElementFromId(GUI_ID_ADD_ENTITY_WINDOW,true))
    // {
    //     name = rootelem->getElementFromId(GUI_ID_ADD_ENTITY_NAME,true)
    //         ->getText();
    // }
    // else if(rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_WINDOW,true))
    // {
    //     name = rootelem->getElementFromId(GUI_ID_EDIT_ENTITY_NAME,true)
    //         ->getText();
    // }
    // else
    // {
    //     return false;
    // }
    // // check name exists already
    // bool nameMatch = true;
    // vector<SimEntity*> * entityVector = engine->getEntityVector();
    // vector<SimEntity*>::iterator it;
    // for(it = entityVector->begin(); it != entityVector->end(); ++it)
    // {
    //     wstring eName;
    //     std::string entityName;
    //     eName = wstring(entityName.begin(),entityName.end()); 
    //     if(eName == name)
    //         nameMatch = false;
    //     nameMatch = nameMatch && true;
    // }
    // return nameMatch;
    return false;
}

void SimGUI::alertCreationFailure(const wchar_t* message)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
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
        GUI_ID_ENTITY_PROMPT_WINDOW);
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

//----------------------------------------------------------------------------//
//                          Private Helper Functions
//----------------------------------------------------------------------------//
//                           Handles GUI positioning
//----------------------------------------------------------------------------//
void SimGUI::promptWindow(s32 prompt, u32 entityType)
{
    setPromptWindow();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    IGUIElement * comboText = rootelem->getElementFromId(
        GUI_ID_ENTITY_PROMPT_COMBO_TEXT,true);
    IGUIComboBox* combo =
        (IGUIComboBox*)(rootelem->getElementFromId(
                            GUI_ID_ENTITY_PROMPT_COMBO,
                            true));
    vector<SimEntity*>* eVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;
    int counter = 0;

    switch(prompt)
    {
    case ADD_ENTITY_PROMPT:
    {
        // append sub entity for each entity type
        switch(entityType)
        {
        case ENTITY_TYPE_ROBOT:
            comboText->setText(L"Choose Robot");
            combo->addItem(L"Quadrotor",SUB_ENTITY_ROBOT_QUAD);
            combo->addItem(L"Ground Robot",SUB_ENTITY_ROBOT_GROUND);
            break;
        case ENTITY_TYPE_SENSOR:
            comboText->setText(L"Choose Sensor");
            combo->addItem(L"Camera",SUB_ENTITY_SENSOR_CAM);
            break;
        case ENTITY_TYPE_ENVIRONMENT:
            comboText->setText(L"Choose Entity");
            combo->addItem(L"April Tag",SUB_ENTITY_ENVIRONMENT_APRIL);
            combo->addItem(L"Cube",SUB_ENTITY_ENVIRONMENT_CUBE);
            combo->addItem(L"Sphere",SUB_ENTITY_ENVIRONMENT_SPHERE);
            break;
        default:
            break;
        }
    }
    case EDIT_ENTITY_PROMPT:
    {
        switch(entityType)
        {
        case ENTITY_TYPE_ROBOT:
        {
            comboText->setText(L"Choose Robot by name to edit");

            for(it = eVector->begin(); it!= eVector->end(); ++it)
            {
                std::wstring eName;
                std::string eN;
                eN= typeid(*it).name();
                eName = std::wstring(eN.begin(),eN.end());
                if(eName.compare(L"P8SimRobot") == 0)
                {
                    std::wstring enName;
                    enName = std::wstring((*it)->getName().begin(),
                                          (*it)->getName().end());
                    combo->addItem(enName.c_str(),counter);
                }
            }
        }
            break;
        case ENTITY_TYPE_SENSOR:
        {
            comboText->setText(L"Choose Sensor by name to edit");
            for(it = eVector->begin(); it!= eVector->end(); ++it)
            {
                SimSensor* s = dynamic_cast<SimSensor*>(*it);
                if(s)
                {

                    comboText->setText(L"Choose Sensor by name to ");
                    std::wstring name;
                    name = std::wstring((*it)->getName().begin(),
                                        (*it)->getName().end());
                    // combo->addItem(name.c_str(),counter);
                    counter++;
                }
            }

        }
            break;
        case ENTITY_TYPE_ENVIRONMENT:
        {

            comboText->setText(L"Choose Entity by name to edit");
            for(it = eVector->begin(); it!= eVector->end(); ++it)
            {
                std::wstring eName;
                std::string eN;
                eN= typeid(*it).name();
                eName = std::wstring(eN.begin(),eN.end());
                if(eName.compare(L"P14SimEnvironment") == 0)
                {
                    std::wstring enName;
                    enName = std::wstring((*it)->getName().begin(),
                                          (*it)->getName().end());
                    combo->addItem(enName.c_str(),counter);
                }

            }
        }
            break;
        default:
            break;
        }
    }
    default:
        break;
    }

//-------------------------------Buttons--------------------------------------//

    //setup buttons create/close
    // IGUIButton * createButton = guienv->addButton(
    //     rect<s32>(

    //         window_width/2 + 10,
    //         310,
    //         window_width/2+window_width/4-5,
    //         350
    //         ),
    //     window,
    //     GUI_ID_ADD_ENTITY_CREATE_BUTTON,
    //     L"Create",
    //     L"Creates entity with above parameters"
    //     );
    // IGUIButton * closeButton = guienv->addButton(
    //     rect<s32>(
    //         window_width/2+window_width/4+5,
    //         310,
    //         window_width-10,
    //         350
    //         ),
    //     window,
    //     GUI_ID_CLOSE_BUTTON,
    //     L"Close",
    //     L"Cancel and close window"
    //     );
}

void SimGUI::setPromptWindow()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    s32 x = px;
    s32 y = py;
    s32 w = pw;
    s32 h = ph;
    // creates GUI element of prompt window
    IGUIWindow* window= guienv->addWindow(
        // window rectangle
        rect<s32>(
            x,
            y,
            x+w,
            y+h),
        // Modality
        true,
        // text
        L"",
        // parent
        0,
        // id
        GUI_ID_ENTITY_PROMPT_WINDOW);

//-----------------------------Combo box--------------------------------------//
    guienv->addStaticText(
        L"Text",
        rect<s32>(
            combo_box_x,
            combo_box_y,
            combo_box_x+ combo_box_w,
            combo_box_y+combo_box_h),
        false, true, window,
        GUI_ID_ENTITY_PROMPT_COMBO_TEXT);

    IGUIComboBox *  cb = guienv->addComboBox(
        rect<s32>(
            combo_x,
            combo_y,
            combo_x+combo_w,
            combo_y+combo_h),
        window,
        GUI_ID_ENTITY_PROMPT_COMBO);
//-----------------------------Entity Name------------------------------------//
    // 
    IGUIStaticText * en = guienv->addStaticText(
        L"Name:",
        rect<s32>(
            boxMargin,
            name_y,
            boxMargin + name_w,
            name_y+name_h),
        false, true, window);
    guienv->addEditBox(
        // text
        L"",
        // rectangle
        rect<s32>(nameText_w_offset + nameInputMargin,
                  0,
                  name_w - nameInputMargin,
                  name_h),
        //border
        true,
        // parent
        en,
        // ID
        GUI_ID_ENTITY_PROMPT_NAME);

//-----------------------------Degree of Freedoms-----------------------------//
    // add text for coordinate/rotation inputs
    IGUIStaticText* dof_box = guienv->addStaticText(
        // text
        L"",
        //
        rect<s32>(boxMargin, dof_y, w-boxMargin, dof_y+dof_h),
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

    IGUIStaticText* position = guienv->
        addStaticText(L"Position",
                      rect<s32>(
                          boxMargin,
                          boxMargin,
                          boxText_w_margin,
                          boxMargin + boxInputText_h),
                      false, true, dof_box);
    IGUIStaticText* sx = guienv->
        addStaticText(L"x:",
                      rect<s32>(
                          boxMargin + boxText_w_margin,
                          boxMargin,
                          boxMargin + (boxInputText_w) + boxText_w_margin,
                          boxMargin + boxInputText_h),
                      false, true, dof_box);
    IGUIStaticText* sy = guienv->
        addStaticText(L"y:",
                      rect<s32>(
                          boxText_w_margin + boxMargin + (boxInputText_w),
                          boxMargin,
                          boxText_w_margin + boxMargin + 2*(boxInputText_w),
                          boxMargin + boxInputText_h),
                      false, true, dof_box);
    IGUIStaticText* sz = guienv->
        addStaticText(L"z:",
                      rect<s32>(
                          boxText_w_margin + boxMargin + 2*(boxInputText_w),
                          boxMargin,
                          boxText_w_margin + boxMargin + 3*(boxInputText_w),
                          boxMargin + boxInputText_h),
                      false, true, dof_box);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(boxInputText_w_offset,
                  0,
                  (boxInputText_w)-20,
                  20),
        //border
        true,
        // parent
        sx,
        // ID
        GUI_ID_ENTITY_PROMPT_POS_X);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(boxInputText_w_offset,
                  0,
                  (boxInputText_w)-20,
                  20),
        //border
        true,
        // parent
        sy,
        // ID
        GUI_ID_ENTITY_PROMPT_POS_Y);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(boxInputText_w_offset,
                  0,
                  (boxInputText_w)-20,
                  20),
        //border
        true,
        // parent
        sz,
        // ID
        GUI_ID_ENTITY_PROMPT_POS_Z);

    IGUIStaticText* rotation= guienv->
        addStaticText(L"Rotation",
                      rect<s32>(
                          boxMargin,
                          boxMargin+ boxText_h_margin + boxInputText_h,
                          boxText_w_margin,
                          boxMargin + boxText_h_margin + boxInputText_h*2),
                      false, true, dof_box);
    IGUIStaticText* sa = guienv->
        addStaticText(L"pitch:",
                      rect<s32>(
                          boxText_w_margin + boxMargin,
                          boxMargin+ boxText_h_margin + boxInputText_h,
                          boxText_w_margin + boxMargin + (boxInputText_w),
                          boxMargin + boxText_h_margin + boxInputText_h*2),
                      false, true, dof_box);
    IGUIStaticText* sb = guienv->
        addStaticText(L"roll:",
                      rect<s32>(
                          boxText_w_margin + boxMargin + (boxInputText_w),
                          boxMargin + boxText_h_margin + boxInputText_h,
                          boxText_w_margin + boxMargin + 2*(boxInputText_w),
                          boxMargin + boxText_h_margin + boxInputText_h*2),
                      false, true, dof_box);
    IGUIStaticText* sc = guienv->
        addStaticText(L"yaw:",
                      rect<s32>(
                          boxText_w_margin + boxMargin + 2*(boxInputText_w),
                          boxMargin + boxText_h_margin + boxInputText_h,
                          boxText_w_margin + boxMargin + 3*(boxInputText_w),
                          boxMargin+ boxText_h_margin + boxInputText_h*2),
                      false, true, dof_box);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(boxInputText_w_offset,
                  0,
                  (boxInputText_w)-20,
                  20),
        //border
        true,
        // parent
        sa,
        // ID
        GUI_ID_ENTITY_PROMPT_ROT_A);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(boxInputText_w_offset,
                  0,
                  (boxInputText_w)-20,
                  20),
        //border
        true,
        // parent
        sb,
        // ID
        GUI_ID_ENTITY_PROMPT_ROT_B);

    guienv->addEditBox(
        // text
        L"0.0",
        // rectangle
        rect<s32>(boxInputText_w_offset,
                  0,
                  (boxInputText_w)-20,
                  20),
        //border
        true,
        // parent
        sc,
        // ID
        GUI_ID_ENTITY_PROMPT_ROT_C);

//-------------------------------Advanced Options-----------------------------//
    IGUIStaticText * adv_box = guienv->
        addStaticText(L"Advanced Options",
                      rect<s32>(
                          boxMargin,
                          advSetting_y,
                          w - boxMargin,
                          advSetting_y + advSetting_h),
                      true, true, window,
                      GUI_ID_ENTITY_PROMPT_ADVANCED_BOX);
}

void SimGUI::setContextMenu()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
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
