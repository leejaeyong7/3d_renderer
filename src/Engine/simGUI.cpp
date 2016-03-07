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
    device = createDevice(EDT_OPENGL, dimension2d<u32>(m_width,m_height));

    // set window parameters
    width = m_width;
    height = m_height;
    width_r = m_width_r;
    height_r = m_height_r;

    // set window title
    device->setWindowCaption(text);

    // set engine pointer for engine method handling
    engine = eng;

    // sets IGUIEnvironment object
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    
    // set GUI skin
    IGUISkin * skin = guienv->getSkin();
    IGUIFont * font = guienv->getFont("data/font/inconsolata.xml");
    skin->setFont(font);
    for (s32 i=0; i<EGDC_COUNT ; ++i)
    {
        SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }

    // set current object to null;
    currType = 0;
    currPrompt = 0;
    currObj = 0;

    // set initial gui setup
    setup();

    // set context menu
    setContextMenu();

}

void SimGUI::setup()
{
    // set event handler
    EventHandler * eh = new EventHandler(this);
    device->setEventReceiver(eh);

    // declare pointers to engine devices
    IVideoDriver * driver = device->getVideoDriver();
    ISceneManager * smgr = device->getSceneManager();

    // set global lighting (weak gray)
    smgr->setAmbientLight(SColorf(0.1f,0.1f,0.1f,0.1f));

    // setup floor grid
    GridNode * gridNode = new GridNode(smgr->getRootSceneNode(),smgr,-1,1000,1000);
    gridNode->drop();

    // add sun light lighting (diffusive light)
    ILightSceneNode* sun_light = smgr->addLightSceneNode(
        //parent node
        0, 
        //scenemanager
        vector3df(0,10000,0), 
        // sun color
        SColorf(1.0f,0.906f,0.882f,0.488f),
        // sun position
        15000.0f,
        -1 );

//----------------------------------------------------------------------------//
//                             Movement setup
//----------------------------------------------------------------------------//

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

            // make background black for 3D rendering area
            //
            driver->draw2DRectangle(SColor(255,0,0,0), 
                    rect<s32>(0,20,width_r,height_r));
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
void SimGUI::update()
{
    vector<EntityMesh>::iterator it;
    for(it = entityMeshVector.begin(); it != entityMeshVector.end(); it++)
    {
        if(((*it).obj == 0) || ((*it).mesh == 0))
            continue;
        Position pos = (*it).obj->getPosition();
        Rotation rot = (*it).obj->getRotation();
        vector3df mpos(pos.X,pos.Y,pos.Z);
        (*it).mesh->setPosition(mpos);
        vector3df mrot(rot.Roll,rot.Pitch,rot.Yaw);
        (*it).mesh->setRotation(mrot);
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
    vector<EntityMesh>::iterator it =
        std::find_if(entityMeshVector.begin(),
                     entityMeshVector.end(),
                     checkEntityPointer(obj));

    while(it != entityMeshVector.end())
    {
        if(it->mesh)
            it->mesh->drop();
        it = std::find_if(std::next(it),
                          entityMeshVector.end(),
                          checkEntityPointer(obj));
    }
    entityMeshVector.erase(
        std::remove_if(entityMeshVector.begin(),
                       entityMeshVector.end(),
                       checkEntityPointer(obj)),
        entityMeshVector.end());
}

void SimGUI::attachEntityMesh(SimRobot * robot, SimSensor * sensor)
{
    if(!robot)
        return;
    if(!sensor)
        return;
    vector<EntityMesh>::iterator it =
        std::find_if(entityMeshVector.begin(),
                     entityMeshVector.end(),
                     checkEntityPointer(robot));
    EntityMesh robotMesh = (*it);
    
    it = std::find_if(entityMeshVector.begin(),
                      entityMeshVector.end(),
                      checkEntityPointer(sensor));
    EntityMesh sensorMesh = (*it);

    if(robotMesh.mesh != 0 && sensorMesh.mesh != 0)
        sensorMesh.mesh->setParent(robotMesh.mesh);
}

void SimGUI::detachEntityMesh(SimRobot * robot, SimSensor * sensor)
{
    if(!robot)
        return;
    if(!sensor)
        return;
    vector<EntityMesh>::iterator it =
        std::find_if(entityMeshVector.begin(),
                     entityMeshVector.end(),
                     checkEntityPointer(robot));
    EntityMesh robotMesh = (*it);
    
    it = std::find_if(entityMeshVector.begin(),
                      entityMeshVector.end(),
                      checkEntityPointer(sensor));
    EntityMesh sensorMesh = (*it);

    if(robotMesh.mesh != 0 && sensorMesh.mesh != 0)
    {
        ISceneManager * smgr = device->getSceneManager();
        sensorMesh.mesh->setParent(smgr->getRootSceneNode());
    }
}
void SimGUI::entityAttachWindow()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    vector<SimEntity*>* eVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;
    s32 wx,wy,ww,wh;
    wx = 100;
    wy = 100;
    ww = 400;
    wh = 300;
    IGUIWindow* window= guienv->addWindow(
        rect<s32>(wx, wy, wx+ww, wy+wh), true, L"", 0, ATTACH_WINDOW);
    switch(currPrompt)
    {
    case ATTACH_ENTITY_PROMPT:
        window->setText(L"Attach Sensor To Robot");
        break;
    case DETACH_ENTITY_PROMPT:
        window->setText(L"Detach Sensor from Robot");
        break;
    default:
        break;
    }

    s32 cx,cy1,cy2,cw,ch;
    s32 cbx;
    cx = 10;
    cw = ww - 2*cx;
    cy1 = 30;
    ch = 40;
    cy2 = cy1 + ch + 10;
    
    // set text
    IGUIStaticText* crt = guienv->addStaticText(
        L"", rect<s32>(cx, cy1, cx+cw, cy1+20), false, true, window);

    IGUIStaticText* cst = guienv->addStaticText(
        L"", rect<s32>(cx, cy2, cx+cw, cy2+20), false, true, window);

    // set dropdown box
    IGUIComboBox *  crcb = guienv->addComboBox(
        rect<s32>(cx, cy1+20, cx+cw, cy1+ch), window, ATTACH_COMBO1);

    IGUIComboBox *  cscb= guienv->addComboBox(
        rect<s32>(cx, cy2+20, cx+cw, cy2+ch), window, ATTACH_COMBO2);

    // add combobox items
    crt->setText(L"Choose Robot: ");
    cst->setText(L"Choose Sensor: ");
    int index = 0;
    for(it = eVector->begin(); it!= eVector->end(); ++it)
    {
        SimRobot* s = dynamic_cast<SimRobot*>(*it);
        if(s)
        {
            std::string name = (*it)->getName();
            std::wstring wname(name.length(), L' '); 
            std::copy(name.begin(), name.end(), wname.begin());
            crcb->addItem(wname.c_str(),index);
        }
        index++;
    }
    index = 0;
    if(currPrompt == ATTACH_ENTITY_PROMPT)
    {
        for(it = eVector->begin(); it!= eVector->end(); ++it)
        {
            SimSensor* s = dynamic_cast<SimSensor*>(*it);
            if(s)
            {
                std::string name = (*it)->getName();
                std::wstring wname(name.length(), L' '); 
                std::copy(name.begin(), name.end(), wname.begin());
                cscb->addItem(wname.c_str(),index);
            }
            index++;
        }
    }
    else
    {
        setDetachData(0);
    }
    s32 bx,by,bw,bh;
    by = cy2+ch + 10;
    bw = 80;
    bx = ww - 10 - bw*2 - 10;
    bh = 40;

    guienv->addButton(rect<s32>(bx,by,bx+bw*1,by+bh), window,
                      CONFIRM_BUTTON,
                      L"Confirm",
                      L"Set with current settings");

    guienv->addButton(rect<s32>(bx+bw*1+10,by,bx+bw*2+10,by+bh), window,
                      CLOSE_BUTTON,
                      L"Close",
                      L"Cancel and close window");
}

void SimGUI::promptEntityWindow()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    // set common prompt window theme
    s32 wx, wy, ww, wh;
    wx = 150;
    wy = 100;
    ww = 600;
    wh = 410;
    setPromptWindow(wx,wy,ww,wh);

    s32 cx,cy,cw,ch,cm;
    cx = 10;
    cy = 30;
    cw = ww - 20;
    ch = 40;
    cm = 70;
    setPromptComboBox(cx,cy,cw,ch);

    s32 nx,ny,nw,nh,nm;
    nx = 10;
    ny = cy + ch + 10;
    nw = ww - 20;
    nh = 20;
    nm = 70;
    setNameBox(nx,ny,nw,nh,nm);

    s32 dx,dy,dw,dh,dm;
    dx = 10;
    dy = ny + nh + 10;
    dw = ww - 20;
    dh = 20*2 + 30;
    dm = 70;
    setDofBox(dx,dy,dw,dh,dm);

    s32 ax,ay,aw,ah;
    ax = 10;
    ay = dy + dh + 10;
    aw = ww - 20;
    ah = 20*6 + 20 + 20;
    setAdvancedSetting(ax,ay,aw,ah);

    s32 bx,by,bw,bh;
    bx = ww - (80+10)*3 - 10;
    by = ay + ah + 10;
    bw = 80;
    bh = 40;
    setButtons(bx,by,bw,bh);


    switch(currPrompt)
    {
    case EDIT_ENTITY_PROMPT:
    {
        setEditPromptData(0);
        break;
    }
    case ADD_ENTITY_PROMPT:
    {
        setAddPromptData(0);
        break;
    }
    default:
        break;
    }
}

//----------------------------------------------------------------------------//
//                          Private Helper Functions
//----------------------------------------------------------------------------//
//                           Handles GUI positioning
//----------------------------------------------------------------------------//
void SimGUI::attachEntityObject()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIComboBox* crcb=
        (IGUIComboBox*)(rootelem->getElementFromId(ATTACH_COMBO1, true));

    IGUIComboBox* cscb=
        (IGUIComboBox*)(rootelem->getElementFromId(ATTACH_COMBO2, true));

    vector<SimEntity*>* vec= engine->getEntityVector();
    int i = crcb->getItemData(crcb->getSelected());
    SimRobot* robj= (SimRobot*)(vec->at(i));
    i = cscb->getItemData(cscb->getSelected());
    if(currPrompt == ATTACH_ENTITY_PROMPT)
    {
        SimSensor* sobj= (SimSensor*)(vec->at(i));
        engine->attachEntity(robj,sobj);
    }
    else if(currPrompt == DETACH_ENTITY_PROMPT)
    {
        vector<SimSensor*>* svec= robj->getSensorVector();
        SimSensor* sobj= (SimSensor*)(svec->at(i));
        engine->detachEntity(robj,sobj);
    }
}

void SimGUI::setDetachData(s32 index)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIComboBox* crcb=
        (IGUIComboBox*)(rootelem->getElementFromId(ATTACH_COMBO1, true));
    IGUIComboBox* cscb=
        (IGUIComboBox*)(rootelem->getElementFromId(ATTACH_COMBO2, true));
    for(int i = 0; i < cscb->getItemCount(); i++)
    {
        cscb->removeItem(i);
    }
    vector<SimEntity*>* vec= engine->getEntityVector();
    int i = crcb->getItemData(index);
    SimRobot* robj= (SimRobot*)vec->at(i);
    vector<SimSensor*>* sv = robj->getSensorVector();
    vector<SimSensor*>::iterator it;
    int c = 0;
    for(it = sv->begin(); it != sv->end(); it++)
    {
        std::string name = (*it)->getName();
        std::wstring wname(name.length(), L' '); 
        std::copy(name.begin(), name.end(), wname.begin());
        cscb->addItem(wname.c_str(),c);
        c++;
    }

}
void SimGUI::createEntityObject()
{
    engine->addEntity((EntityType)currType, currObj);
    currType = 0;
    currObj = 0;
}

void SimGUI::editEntityObject()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PROMPT_COMBO, true));

    IGUIEditBox * en =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_NAME, true));

    IGUIEditBox * ex =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_POS_X, true));

    IGUIEditBox * ey =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_POS_Y, true));

    IGUIEditBox * ez =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_POS_Z, true));

    IGUIEditBox * ea =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_ROT_A, true));

    IGUIEditBox * eb =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_ROT_B, true));

    IGUIEditBox * ec =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_ROT_C, true));

    IGUIStaticText * adv_box =
        (IGUIStaticText*)(rootelem->getElementFromId(PROMPT_ADVANCED_BOX,true));

    // set name
    const wchar_t * wname = en->getText();
    wstring ws(wname);
    std::string str(ws.begin(),ws.end());
    currObj->setName(str);

    stringc cstr;
    // set Position
    cstr = ex->getText();
    double x = (double)atof(cstr.c_str());
    cstr = ey->getText();
    double y = (double)atof(cstr.c_str());
    cstr = ez->getText();
    double z = (double)atof(cstr.c_str());
    currObj->setPosition(x,y,z);

    cstr = ea->getText();
    double a = (double)atof(cstr.c_str());
    cstr = eb->getText();
    double b = (double)atof(cstr.c_str());
    cstr = ec->getText();
    double c = (double)atof(cstr.c_str());
    currObj->setRotation(a,b,c);

    // advanced settings
    vector<AdvancedOption*>* advOV = currObj->getAdvancedOption();
    vector<AdvancedOption*>::iterator it = advOV->begin();

    list<IGUIElement*> advChildren = adv_box->getChildren();
    list<IGUIElement*>::Iterator lit = advChildren.begin();;
    for(int i = 0; i < advChildren.size(); i++)
    {
        cstr = (*lit)->getText();
        switch((*it)->type)
        {
        case INTEGER:
            ((AdvancedOption_Int*)*it)->value = (int)atof(cstr.c_str());
            break;
        case DOUBLE:
            ((AdvancedOption_Double*)*it)->value = (double)atof(cstr.c_str());
            break;
        case STRING:
        {
            std::string advstr(ws.begin(),ws.end());
            ((AdvancedOption_String*)*it)->value = advstr;
            break;
        }
        default:
            break;
        }
    }
}
void SimGUI::setEditPromptData(s32 index)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PROMPT_COMBO, true));

    vector<SimEntity*>* vec= engine->getEntityVector();
    int i = cb->getItemData(index);
    SimEntity* obj= vec->at(i);
    currObj = obj;
    setPromptData(obj);
}
void SimGUI::setAddPromptData(s32 index)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PROMPT_COMBO, true));
    int i = cb->getItemData(index);
    SimEntity* obj = 0;
    switch(currType)
    {
    case ENTITY_TYPE_ROBOT:
    {
        switch(i)
        {
        case SUB_ENTITY_ROBOT_QUAD:
        {
            break;
        }
        case SUB_ENTITY_ROBOT_GROUND:
        {
            obj = new SimGroundRobot("",0,0,0,0,0,0);
            currObj = obj;
            break;
        }
        default:
            break;
        }
        break;
    }
    case ENTITY_TYPE_SENSOR:
    {
        switch(i)
        {
        case SUB_ENTITY_SENSOR_CAM:
        {
            obj = new SimCamera("",0,0,0,0,0,0,0,0,0);
            currObj = obj;
            break;
        }
        default:
            break;
        }
        break;
    }
    case ENTITY_TYPE_ENVIRONMENT:
    {
        switch(i)
        {
        case SUB_ENTITY_ENVIRONMENT_APRIL:
        {
            break;
        }
        case SUB_ENTITY_ENVIRONMENT_CUBE:
        {
            break;
        }
        case SUB_ENTITY_ENVIRONMENT_SPHERE:
        {
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    setPromptData(obj);
}
void SimGUI::setPromptData(SimEntity * obj)
{
    // checks whether prompt is properly called
    if(obj == 0)
        return;

    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    IGUIWindow* window;
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_ADD_WINDOW,true));
        break;
    case EDIT_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_EDIT_WINDOW,true));
        break;
    }
    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PROMPT_COMBO, true));

    IGUIEditBox * en =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_NAME, true));

    IGUIEditBox * ex =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_POS_X, true));

    IGUIEditBox * ey =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_POS_Y, true));

    IGUIEditBox * ez =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_POS_Z, true));

    IGUIEditBox * ea =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_ROT_A, true));

    IGUIEditBox * eb =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_ROT_B, true));

    IGUIEditBox * ec =
        (IGUIEditBox*)(rootelem->getElementFromId(PROMPT_ROT_C, true));

    IGUIStaticText * adv_box =
        (IGUIStaticText*)(rootelem->getElementFromId(PROMPT_ADVANCED_BOX,true));

    // set name
    std::string name = obj->getName();
    std::wstring wname(name.length(), L' '); 
    std::copy(name.begin(), name.end(), wname.begin());
    en->setText(wname.c_str());

    // set Position
    Position pos = obj->getPosition();
    ex->setText(to_wstring(pos.X).c_str());
    ey->setText(to_wstring(pos.Y).c_str());
    ez->setText(to_wstring(pos.Z).c_str());

    Rotation rot = obj->getRotation();
    ea->setText(to_wstring(rot.Roll).c_str());
    eb->setText(to_wstring(rot.Pitch).c_str());
    ec->setText(to_wstring(rot.Yaw).c_str());

    // advanced settings
    vector<AdvancedOption*>* advOV = obj->getAdvancedOption();
    vector<AdvancedOption*>::iterator it;

    const rect<s32> advB = adv_box->getAbsolutePosition();
    s32 advW = advB.getWidth();
    s32 advIW = advW/3;
    int count = 0;
    list<IGUIElement*> advChildren = adv_box->getChildren();
    list<IGUIElement*>::Iterator lit;
    for(lit = advChildren.begin(); lit != advChildren.end(); ++lit)
    {
        adv_box->removeChild(*lit);
    }

    for(it = advOV->begin(); it!= advOV->end(); ++it)
    {
        std::string label = (*it)->label;
        label.append(":");
        std::wstring wname(label.length(),L' ');
        std::copy(label.begin(), label.end(), wname.begin());

        IGUIStaticText* text = guienv->addStaticText(
            wname.c_str(),
            rect<s32>(
                advIW*(count%4) + 10,
                20 + 20*((int)count/4),
                advIW*((count%4) +1),
                20*((int)count/4) + 40
                ),
            false, true, adv_box);

        IGUIEditBox* input =  guienv->addEditBox(
            L"0.0",
            rect<s32>(100,0,advIW-20,20),
            true,
            text,
            PROMPT_ADV_INPUT
            );

        switch((*it)->type)
        {
        case INTEGER:
        {
            int val = ((AdvancedOption_Int*)(*it))->value;
            input->setText(to_wstring(val).c_str());
            break;
        }
        case DOUBLE:
        {
            double val = ((AdvancedOption_Double*)(*it))->value;
            input->setText(to_wstring(val).c_str());
            break;
        }
        case STRING:
        {
            std::string val = ((AdvancedOption_String*)(*it))->value;
            std::wstring wval(label.length(),L' ');
            std::copy(val.begin(), val.end(), wval.begin());
            input->setText(wval.c_str());
            break;
        }
        }
        count++;
        if(count == 24)
            break;
    }
}
void SimGUI::setPromptWindow(s32 wx, s32 wy, s32 ww, s32 wh)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    // creates GUI element of prompt window
    IGUIWindow* window;
    // set window title selectively
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
    {
        window= guienv->addWindow(
            rect<s32>(wx, wy, wx+ww, wy+wh), true, L"", 0, PROMPT_ADD_WINDOW);

        switch(currType)
        {
        case ENTITY_TYPE_ROBOT:
            window->setText(L"Add Robot");
            break;
        case ENTITY_TYPE_SENSOR:
            window->setText(L"Add Sensor");
            break;
        case ENTITY_TYPE_ENVIRONMENT:
            window->setText(L"Add Environment");
            break;
        }
        break;
    }
    case EDIT_ENTITY_PROMPT:
    {
        window= guienv->addWindow(
            rect<s32>(wx, wy, wx+ww, wy+wh), true, L"", 0, PROMPT_EDIT_WINDOW);
        switch(currType)
        {
        case ENTITY_TYPE_ROBOT:
            window->setText(L"Edit Robot");
            break;
        case ENTITY_TYPE_SENSOR:
            window->setText(L"Edit Sensor");
            break;
        case ENTITY_TYPE_ENVIRONMENT:
            window->setText(L"Edit Environment");
            break;
        }
        break;
    }
    }
    
}
void SimGUI::setPromptComboBox(s32 cx, s32 cy, s32 cw, s32 ch)
{
    // get device object pointers
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    vector<SimEntity*>* eVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;
    IGUIWindow* window;
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_ADD_WINDOW,true));
        break;
    case EDIT_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_EDIT_WINDOW,true));
        break;
    }

    // set text
    IGUIStaticText* text = guienv->addStaticText(
        L"", rect<s32>(cx, cy, cx+cw, cy+ch), false, true, window);

    // set dropdown box
    IGUIComboBox *  cb = guienv->addComboBox(
        rect<s32>(cx, cy+20, cx+cw, cy+ch), window, PROMPT_COMBO);

    // add combobox items
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
    {
        switch(currType)
        {
        case ENTITY_TYPE_ROBOT:
            text->setText(L"Choose Robot");
            cb->addItem(L"Quadrotor",SUB_ENTITY_ROBOT_QUAD);
            cb->addItem(L"Ground Robot",SUB_ENTITY_ROBOT_GROUND);
            break;
        case ENTITY_TYPE_SENSOR:
            text->setText(L"Choose Sensor");
            cb->addItem(L"Camera",SUB_ENTITY_SENSOR_CAM);
            break;
        case ENTITY_TYPE_ENVIRONMENT:
            text->setText(L"Choose Entity");
            cb->addItem(L"April Tag",SUB_ENTITY_ENVIRONMENT_APRIL);
            cb->addItem(L"Cube",SUB_ENTITY_ENVIRONMENT_CUBE);
            cb->addItem(L"Sphere",SUB_ENTITY_ENVIRONMENT_SPHERE);
            break;
        default:
            break;
        }
    }
    break;
    case EDIT_ENTITY_PROMPT:
    {
        // index of entity from vector
        int index = 0;
        switch(currType)
        {
        case ENTITY_TYPE_ROBOT:
        {
            text->setText(L"Choose Robot by name to edit");
            for(it = eVector->begin(); it!= eVector->end(); ++it)
            {
                SimRobot* s = dynamic_cast<SimRobot*>(*it);
                if(s)
                {
                    std::string name = (*it)->getName();
                    std::wstring wname(name.length(), L' '); 
                    std::copy(name.begin(), name.end(), wname.begin());
                    cb->addItem(wname.c_str(),index);
                }
                index++;
            }
        }
        break;
        case ENTITY_TYPE_SENSOR:
        {
            text->setText(L"Choose Sensor by name to edit");
            for(it = eVector->begin(); it!= eVector->end(); ++it)
            {
                SimSensor* s = dynamic_cast<SimSensor*>(*it);
                if(s)
                {
                    std::string name = (*it)->getName();
                    std::wstring wname(name.length(), L' '); 
                    std::copy(name.begin(), name.end(), wname.begin());
                    cb->addItem(wname.c_str(),index);
                }
                index++;
            }
        }
        break;
        case ENTITY_TYPE_ENVIRONMENT:
        {
            text->setText(L"Choose Entity by name to edit");
            for(it = eVector->begin(); it!= eVector->end(); ++it)
            {
                SimEntity* s = dynamic_cast<SimEntity*>(*it);
                if(s)
                {
                    std::string name = (*it)->getName();
                    std::wstring wname(name.length(), L' '); 
                    std::copy(name.begin(), name.end(), wname.begin());
                    cb->addItem(wname.c_str(),index);
                }
                index++;
            }
        }
        break;
        default:
            break;
        }
    }
    break;
    default:
        break;
    }
}
void SimGUI::setNameBox(s32 nx, s32 ny, s32 nw, s32 nh, s32 nm)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIWindow* window;
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_ADD_WINDOW,true));
        break;
    case EDIT_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_EDIT_WINDOW,true));
        break;
    }
    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PROMPT_COMBO, true));

    // create textbox and name input
    guienv->addStaticText(
        L"Name:", rect<s32>(nx, ny,nx+nw,ny+nh), false, true, window);

    IGUIEditBox* eb = guienv->addEditBox(
        L"", rect<s32>(nx+nm, ny,nx+nw,ny+nh), true, window, PROMPT_NAME);

}
void SimGUI::setDofBox(s32 dx, s32 dy, s32 dw, s32 dh, s32 dm)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIWindow* window;
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_ADD_WINDOW,true));
        break;
    case EDIT_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_EDIT_WINDOW,true));
        break;
    }
    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PROMPT_COMBO, true));

    // add text for coordinate/rotation inputs
    IGUIStaticText* dof_box = guienv->addStaticText(
        L"", rect<s32>(dx,dy,dx+dw,dy+dh), true, true, window, -1, false);

    // set pos/rotation with margin 10
    s32 ddw = dw-20;
    
    IGUIStaticText* position =
        guienv->addStaticText(
            L"Position", rect<s32>(10,10,ddw,30), false, true, dof_box);

    IGUIStaticText* rotation=
        guienv->addStaticText(
            L"Rotation", rect<s32>(10,40,ddw,60), false, true, dof_box);

    // set textwidth as 1/3 each
    s32 tw = (ddw - dm + 10) / 3;

    // add static text
    IGUIStaticText* sx =
        guienv->addStaticText(
            L"x:", rect<s32>(dm,0,tw+dm,20), false, true, position);

    IGUIStaticText* sy =
        guienv->addStaticText(
            L"y:",rect<s32>(tw+dm,0,2*tw+dm,20),false,true,position);

    IGUIStaticText* sz =
        guienv->addStaticText(
            L"z:",rect<s32>(2*tw+dm,0,3*tw+dm,20),false,true,position);

    IGUIStaticText* sa =
        guienv->addStaticText(
            L"Roll:", rect<s32>(dm,0,tw+dm,20), false, true, rotation);

    IGUIStaticText* sb =
        guienv->addStaticText(
            L"Pitch:",rect<s32>(tw+dm,0,2*tw+dm,20),false,true,rotation);

    IGUIStaticText* sc =
        guienv->addStaticText(
            L"Yaw:",rect<s32>(2*tw+dm,0,3*tw+dm,20),false,true,rotation);

    s32 tm = 50;
    // add edit box
    IGUIEditBox* ex =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sx, PROMPT_POS_X);

    IGUIEditBox* ey =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sy, PROMPT_POS_Y);

    IGUIEditBox* ez =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sz, PROMPT_POS_Z);

    IGUIEditBox* ea =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sa, PROMPT_ROT_A);

    IGUIEditBox* eb =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sb, PROMPT_ROT_B);

    IGUIEditBox* ec =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sc, PROMPT_ROT_C);

}
void SimGUI::setAdvancedSetting(s32 ax, s32 ay, s32 aw, s32 ah)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIWindow* window;
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_ADD_WINDOW,true));
        break;
    case EDIT_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_EDIT_WINDOW,true));
        break;
    }
    IGUIStaticText * adv_box =
        guienv->addStaticText(
            L"Advanced Options",
            rect<s32>(ax,ay,ax+aw,ay+ah),true,true,window,PROMPT_ADVANCED_BOX);

    s32 aaw = aw - 20;
    s32 iw = aaw/4;
}
void SimGUI::setButtons(s32 bx, s32 by, s32 bw, s32 bh)
{
    // call pointers to engine device objects
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIWindow* window;
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_ADD_WINDOW,true));
        break;
    case EDIT_ENTITY_PROMPT:
        window =
            (IGUIWindow*)(rootelem-> getElementFromId(PROMPT_EDIT_WINDOW,true));
        break;
    }
    switch(currPrompt)
    {
    case ADD_ENTITY_PROMPT:
    {
        // add create / close buttons
        //setup buttons create/close
        guienv->addButton(rect<s32>(bx+bw+10,by,bx+bw*2+10,by+bh), window,
                          CONFIRM_BUTTON,
                          L"Create",
                          L"Creates entity with above parameters");
        guienv->addButton(rect<s32>(bx+bw*2+20,by,bx+bw*3+20,by+bh), window,
                          CLOSE_BUTTON,
                          L"Close",
                          L"Cancel and close window");
        break;
    }
    case EDIT_ENTITY_PROMPT:
    {
        guienv->addButton(rect<s32>(bx,by,bx+bw,by+bh), window,
                          REMOVE_BUTTON,
                          L"Delete",
                          L"Delete this entity");

        guienv->addButton(rect<s32>(bx+bw+10,by,bx+bw*2+10,by+bh), window,
                          CONFIRM_BUTTON,
                          L"Confirm",
                          L"Set entity with above parameters");

        guienv->addButton(rect<s32>(bx+bw*2+20,by,bx+bw*3+20,by+bh), window,
                          CLOSE_BUTTON,
                          L"Close",
                          L"Cancel and close window");
        break;
    }
    }
}


void SimGUI::setContextMenu()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    // add context menu(toolbar menu)
    IGUIContextMenu * cM = guienv->addMenu();

    //---------------------------Top Level Menu-------------------------------//
    // menu dropdown for context menu
    u32 engineMenuId = cM->addItem(L"Engine", 1, true, true, false, false);

    u32 entityMenuId = cM->addItem(L"Entity", 2, true, true, false, false);

    IGUIContextMenu * engineMenu = cM->getSubMenu(engineMenuId);
    IGUIContextMenu * entityMenu = cM->getSubMenu(entityMenuId);
    
    //---------------------------Engine Menu----------------------------------//

	engineMenu->addItem(L"Quit", QUIT_BUTTON, true, false, false, false);

    //---------------------------Entity Menu----------------------------------//
	u32 eA= entityMenu->addItem(L"Add", -1, true, true, false, false);

	u32 eR= entityMenu->addItem(L"Edit", -1, true, true, false, false);

	u32 eAt= entityMenu->addItem(
        L"Attach", ATTACH_ENTITY, true, false, false, false);
	u32 eDt= entityMenu->addItem(
        L"Detach", DETACH_ENTITY, true, false, false, false);

    IGUIContextMenu * entityAdd = entityMenu->getSubMenu(eA);
    IGUIContextMenu * entityRemove= entityMenu->getSubMenu(eR);

    //----------------------Entity Add/Remove Sub Menu------------------------//
    u32 entityAddRobotId= entityAdd->addItem(
        L"Robot", ADD_ROBOT, true, false, false, false);

    u32 entityAddSensorId= entityAdd->addItem(
        L"Sensor", ADD_SENSOR, true, false, false, false);

    u32 entityAddEnvId= entityAdd->addItem(
        L"Environment", ADD_ENVIRONMENT, true, false, false, false);

    //---------------------REMOVE-----------------------//
    u32 entityRemoveRobotId= entityRemove->addItem(
        L"Robot", EDIT_ROBOT, true, false, false, false);

    u32 entityRemoveSensorId= entityRemove->addItem(
        L"Sensor", EDIT_SENSOR, true, false, false, false);

    u32 entityRemoveEnvId= entityRemove->addItem(
        L"Environment", EDIT_ENVIRONMENT, true, false, false, false);
}
