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
    renderView = rect<s32>(0,20,width_r,height_r+20);

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
    placeObj = 0;
    placeMode = 0;

    // set initial gui setup
    setup();

    // set context menu
    setContextMenu();
    setCameraDropdown();
    setCameraCapture();
    setPathExec();
    setPlacerMenu();
    setPlacerSubMenu(0);

    exec = false;
}

void SimGUI::setup()
{
    // set event handler
    EventHandler * eh = new EventHandler(this);
    device->setEventReceiver(eh);

    // declare pointers to engine devices
    IVideoDriver * driver = device->getVideoDriver();
    ISceneManager * smgr = device->getSceneManager();
    ISceneNode * r = smgr->getRootSceneNode();

    // set global lighting (weak gray)
    smgr->setAmbientLight(SColorf(0.1f,0.1f,0.1f,1.0f));

    // setup floor grid
    GridNode * gridNode = new GridNode(smgr->getRootSceneNode(),
                                       smgr,-1,100,100,10);
    gridNode->drop();

    // add 4 sun lights at corners
    ILightSceneNode* sunu = smgr->addLightSceneNode(
        0, vector3df(0,10000,0),
        SColorf(0.906f,0.882f,0.488f,1.0f),
        25000.0f, -1 );

    SColorf side(0.151f,0.146f,0.81f,1.0f);
    ILightSceneNode* sun1 = smgr->addLightSceneNode(
        0, vector3df(5000,0,5000), side, 25000.0f, -1 );
    ILightSceneNode* sun2 = smgr->addLightSceneNode(
        0, vector3df(5000,0,-5000), side, 25000.0f, -1 );
    ILightSceneNode* sun3 = smgr->addLightSceneNode(
        0, vector3df(-5000,0,5000), side, 25000.0f, -1 );
    ILightSceneNode* sun4 = smgr->addLightSceneNode(
        0, vector3df(-5000,0,-5000), side, 25000.0f, -1 );

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

    fc = smgr->addCameraSceneNodeFPS(
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
    fc->setPosition(vector3df(0,5,0));
    // sets far value of camera
    // extended for sun support
    fc->setFarValue(20000.0f);
    // hides mouse
    /* device->getCursorControl()->setVisible(false); */

    sc = new Sim::CameraSceneNode(r,smgr,-1);

    // minimap camera
    mc = smgr->addCameraSceneNode(0, vector3df(0,50,0), vector3df(0,0,0));
    yc = smgr->addCameraSceneNode(0, vector3df(10,10,10), vector3df(0,0,0));
    yc->bindTargetAndRotation(false);
    /* yc = new WorldCameraSceneNode(r,smgr,-1,device); */

    yc->bindTargetAndRotation(false);
    yc->setFOV(fc->getFOV());
    yc->setAspectRatio(fc->getAspectRatio());
    yc->setPosition(vector3df(0,100,0));
    yc->setTarget(vector3df(0,30,0));
    yc->setFarValue(20000.0f);

    // placer camera
    pc = smgr->addCameraSceneNode(0, vector3df(-20,20,-20), vector3df(0,0,0));

    // setup path scenenode
    paths = new Sim::PathSceneNode(r,smgr,-1);

    wc = fc;
    smgr->setActiveCamera(wc);
}

void SimGUI::draw()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    ISceneManager * smgr = device->getSceneManager();
    IVideoDriver * driver = device->getVideoDriver();
    rect<s32> minimap(width_r + 10, 300, width - 10, height - 10);
    // if running, draw
    if(device->run())
    {
        // is window is shown
        if(device->isWindowActive())
        {
            ICameraSceneNode* cc = smgr->getActiveCamera();
            driver->beginScene(true,true,SColor(255,200,200,200));
            // make background black for 3D rendering area
            driver->draw2DRectangle(SColor(255,0,0,0),renderView);
            driver->draw2DRectangle(SColor(255,0,0,0),minimap);
            driver->setViewPort(renderView);
            smgr->drawAll();

            smgr->setActiveCamera(mc);
            driver->setViewPort(minimap);
            //Draw scene
            smgr->drawAll();
            smgr->setActiveCamera(cc);
            driver->setViewPort(rect<s32>(0,0,width,height));
            guienv->drawAll();
            driver->endScene();
            if(exec)
            {
                curr_t= clock();
                if((((double)(curr_t - prev_t))/ CLOCKS_PER_SEC) >
                   (1.0f / cap_fps))
                {
                    execUpdate();
                    prev_t = curr_t;
                }
            }
            if(placeMode)
            {
                if(placeObj && wc == smgr->getActiveCamera())
                {
                    ISceneCollisionManager * cm =
                        smgr->getSceneCollisionManager();
                    position2d<s32> cursor =
                        device->getCursorControl()->getPosition();
                    position2d<s32> trueCursor =
                        position2d<s32>(
                            cursor.X * ((double)width/(double)width_r),
                            cursor.Y * ((double)height/(double)height_r));
                    line3d<f32> raytrace =
                        cm->getRayFromScreenCoordinates(
                            trueCursor, wc);
                    vector3df ratio = raytrace.start - raytrace.end;
                    double dxdy = ratio.X / ratio.Y;
                    double dzdy = ratio.Z / ratio.Y;
                    double dyh = wc->getPosition().Y;
                    vector<Point>* pts = placeMesh->getAllPoints();
                    vector<Point>::iterator itp;
                    double baseHeight = 0;
                    line3d<f32> ray;

                    IMetaTriangleSelector* ts = 
                        smgr->createMetaTriangleSelector();
                    // iterate scene nodes
                    vector<SimSceneNode*>::iterator it;
                    for(it = entityMeshVector.begin();
                        it != entityMeshVector.end();
                        it++)
                    {
                        // check whether entity is environment
                        SimEnvironment* s = 
                            dynamic_cast<SimEnvironment*>(
                                (*it)->getEntity());
                        if(s)
                        {
                            ts->addTriangleSelector(
                                (smgr)->
                                createTriangleSelectorFromBoundingBox(*it));
                        }
                    }
                    vector3df cv;
                    triangle3df ot;
                    ISceneNode * matchingNode = 0;
                    vector3df pp = placeMesh->getPosition();

                    for(itp = pts->begin(); itp != pts->end(); itp++)
                    {
                        ray = line3d<f32>(pp.X + itp->x,999999,pp.Z + itp->z,
                                          pp.X + itp->x,0,pp.Z + itp->z);
                        if(cm->getCollisionPoint(ray,ts,cv,ot,matchingNode))
                        {
                            SimSceneNode * scn = (SimSceneNode*)matchingNode;
                            baseHeight =
                                std::max(
                                    baseHeight,
                                    scn->getPosition().Y +
                                    scn->getHeight()/2);
                            
                            std::cout<<scn->getPosition().Y<<"," <<
                                scn->getHeight()<<","<<
                                baseHeight<<std::endl;
                        }
                    }
                    placeObj->setPosition(
                        wc->getPosition().X + -1*dyh*dxdy,
                        baseHeight + placeMesh->getHeight()/2,
                        wc->getPosition().Z + -1*dyh*dzdy);
                    placeMesh->update();
                }
            }
        }
        else
            device->yield();
    }
}
void SimGUI::update()
{
    // update all scene nodes
    vector<SimSceneNode*>::iterator it;
    for(it = entityMeshVector.begin(); it != entityMeshVector.end(); it++)
    {
        if((*it) == 0)
            continue;
        (*it)->update();
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
/**
 * @brief executes path based on saved points
 */
void SimGUI::execPath()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    ISceneManager * smgr = device->getSceneManager();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    std::list<PathNode>* plist = paths->getPathList();
    std::list<PathNode>::iterator it = plist->begin();
    if(plist->size() < 1)
        return;
    exec = true;
    cap_id = 0;
    struct stat sb;
    IGUIStaticText * fn =
        (IGUIStaticText*)(rootelem->getElementFromId(PATH_FOLDER,true));

    IGUIScrollBar* scr =
        (IGUIScrollBar*)(rootelem->getElementFromId(PATH_FPS_SCROLL,true));

    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PATH_CAMERA_COMBO,true));

    wstring pn = fn->getText();
    std::string oname(pn.begin(),pn.end());
    std::string pathname = oname;
    int i = 1;
    // set name of path result path 
    while((stat(pathname.c_str(),&sb) == 0 && S_ISDIR(sb.st_mode)))
    {
        pathname = oname + " (" + to_string(i) + ")";
        i++;
    }

    cap_path = wstring(pathname.length(), L' ');
    std::copy(pathname.begin(),pathname.end(),cap_path.begin());
    mkdir(pathname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    cap_fps = scr->getPos();
    curr_node = 1;
    cap_node_count = 0;

    s32 sid = cb->getSelected();
    u32 d = cb->getItemData(sid);

    vector<SimEntity*>* ev= engine->getEntityVector();
    SimCamera* s = dynamic_cast<SimCamera*>(ev->at(d));
    if(s)
    {
        if(wc == smgr->getActiveCamera())
            wc->setInputReceiverEnabled(false);
        ((CameraSceneNode*)sc)->attachCamera(s);
        sc->setInputReceiverEnabled(true);
        device->getSceneManager()->setActiveCamera(sc);
    }

    ICameraSceneNode * cc = smgr->getActiveCamera();
    SimCamera* cam = ((Sim::CameraSceneNode*)cc)->getCamera();
    cam->removeAttachedRobot();
    vector3df new_pos = (*it).Pos;
    vector3df new_rot = (*it).Rot;
    cam->setPosition(new_pos.X,new_pos.Y,new_pos.Z);
    cam->setRotation(new_rot.X,new_rot.Y,new_rot.Z);

    ((Sim::CameraSceneNode*)cc)->update();
    curr_t = clock();
}

void SimGUI::execUpdate()
{
    // capture and update
    capture();

    ISceneManager * smgr = device->getSceneManager();
    std::list<PathNode>* plist = paths->getPathList();
    std::list<PathNode>::iterator it = plist->begin();
    std::advance(it,(int)curr_node-1);
    PathNode pN = (*it);
    std::advance(it,1);
    PathNode cN = (*it);


    vector3df pos_update = cN.Pos - pN.Pos;
    vector3df rot_update = cN.Rot - pN.Rot;
    if(rot_update.X > 180)
    {
        rot_update.X -= 360;
    }
    else if(rot_update.X < - 180)
    {
        rot_update.X += 360;
    }
    if(rot_update.Y > 180)
    {
        rot_update.Y -= 360;
    }
    else if(rot_update.Y < - 180)
    {
        rot_update.Y += 360;
    }

    if(rot_update.Z > 180)
    {
        rot_update.Z -= 360;
    }
    else if(rot_update.Z < - 180)
    {
        rot_update.Z += 360;
    }
    pos_update = pos_update / floor(cap_fps*cN.dur);
    rot_update = rot_update / floor(cap_fps*cN.dur);

    ICameraSceneNode * cc = smgr->getActiveCamera();

    SimCamera* cam = ((Sim::CameraSceneNode*)cc)->getCamera();
    Position pos = cam->getPosition();
    Rotation rot = cam->getRotation();
    vector3df new_pos = vector3df(pos.X,pos.Y,pos.Z) + pos_update;
    vector3df new_rot = vector3df(rot.Roll,rot.Pitch,rot.Yaw) +rot_update;

    cam->setPosition(new_pos.X,new_pos.Y,new_pos.Z);
    cam->setRotation(new_rot.X,new_rot.Y,new_rot.Z);

    ((Sim::CameraSceneNode*)cc)->update();

    cap_node_count++;
    if(cap_node_count == floor(cap_fps*cN.dur))
    {
        curr_node++;
        cap_node_count = 0;
    }
    if(curr_node == plist->size())
    {
        exec = false;
        cap_path = L".";
        cap_id = 0;
    }
}

//----------------------------------------------------------------------------//
//                            GUI WINDOW HANDLERS                             //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                           OBJECT PLACER HANDLER                            //
//----------------------------------------------------------------------------//
void SimGUI::setPlacerMenu()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    s32 cx,cy,cw,bw;
    cx = 10;
    cw = width_r - 20;
    cy = height_r + 30;
    bw = 40;

    guienv->addStaticText(
        L"Camera Mode",
        rect<s32>(cx,cy,cx+bw*2+20,cy+70),
        true,false);

    guienv->addButton(rect<s32>(cx+10,cy+20,cx+bw+10,cy+60), 0,
                      FPS_CAMERA_BUTTON,
                      L"Free",
                      L"Use Free World Camera");

    guienv->addButton(rect<s32>(cx+bw+10,cy+20,cx+2*bw+10,cy+60), 0,
                      EDIT_CAMERA_BUTTON,
                      L"Edit",
                      L"Use Edit mode Camera");
}
void SimGUI::setPlacerSubMenu(int type)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    IGUIElement* t;
    if((t = rootelem->getElementFromId(CAMERA_MODE_TEXT,true))){
        t->remove();
    }
    if((t = rootelem->getElementFromId(EDIT_MOVEMENT_TEXT,true))){
        t->remove();
    }
    if((t = rootelem->getElementFromId(ADD_ENTITY_TEXT,true))){
        t->remove();
    }
    if(type == 0)
    {
        s32 px,py,pw,ph,bw;
        px = 120;
        pw = width_r - 20 - px;
        py = height_r + 30;
        bw = 90;

        IGUIStaticText* ct = guienv->addStaticText(
            L"Choose Movement Mode",
            rect<s32>(px,py,px+2*bw+20,py+70),
            true,false,0,CAMERA_MODE_TEXT);

        guienv->addCheckBox(true,rect<s32>(10,20,bw+10,60),
                            ct,
                            FREE_CAMERA_MODE,
                            L"Fly");

        guienv->addCheckBox(false,rect<s32>(bw+10,20,2*bw+10,60), 
                            ct,
                            GROUND_CAMERA_MODE,
                            L"Ground");

    }
    else
    {
        s32 px,py,pw,ph,bw;
        px = 120;
        pw = width_r - 20 - px;
        py = height_r + 30;
        bw = 60;

        IGUIStaticText* ct = guienv->addStaticText(
            L"Move Camera",
            rect<s32>(px,py,px+4*bw+20,py+70),
            true,false,0,EDIT_MOVEMENT_TEXT);

        guienv->addButton(rect<s32>(10,20,bw+10,40), ct,
                          ADD_Y_BUTTON,
                          L"Y+",
                          L"Increase Height");
        guienv->addButton(rect<s32>(10,40,bw+10,60), ct,
                          SUB_Y_BUTTON,
                          L"Y-",
                          L"Decrease Height");
        guienv->addButton(rect<s32>(bw+10,20,2*bw+10,40), ct,
                          ROT_Y_U_BUTTON,
                          L"ROT Y++",
                          L"Look Up");
        guienv->addButton(rect<s32>(bw+10,40,2*bw+10,60), ct,
                          ROT_Y_D_BUTTON,
                          L"ROT Y-",
                          L"Look Down");
        guienv->addButton(rect<s32>(2*bw+10,20,3*bw+10,60),ct,
                          ROT_L_BUTTON,
                          L"Rot L",
                          L"Rotate Counter Clockwise");
        guienv->addButton(rect<s32>(3*bw+10,20,4*bw+10,60), ct,
                          ROT_R_BUTTON,
                          L"Rot R",
                          L"Rotate Clockwise");
        px = 120;
        py = height_r + 30;
        bw = 60;

        ct = guienv->addStaticText(
            L"Add Entity",
            rect<s32>(px+4*bw+30,py,px+7*bw+50,py+70),
            true,false,0,ADD_ENTITY_TEXT);

        guienv->addButton(rect<s32>(10,20,bw+10,60), ct,
                          ADD_PLANE_BUTTON,
                          L"Plane",
                          L"Adds Plane on scene");
        guienv->addButton(rect<s32>(bw+10,20,2*bw+10,60), ct,
                          ADD_CUBE_BUTTON,
                          L"Cube",
                          L"Adds Cube on scene");
        guienv->addButton(rect<s32>(2*bw+10,20,3*bw+10,60),ct,
                          ADD_PYRAMID_BUTTON,
                          L"Pyramid",
                          L"Adds Pyramid on scene");
    }
}
//----------------------------------------------------------------------------//
//                             PATH EXEC HANDLERS                             //
//----------------------------------------------------------------------------//
void SimGUI::setPathExec()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    s32 cx,cy,cw,ch;
    cx = width_r + 10;
    cw = width - width_r -20;
    cy = 130;
    ch = 40;

    IGUIStaticText * ct = guienv->addStaticText(
        L"Choose Camera To Execute Path",
        rect<s32>(cx,cy,cx+cw,cy+20),
        false, true);
    IGUIComboBox* ccb = guienv->addComboBox(
        rect<s32>(cx, cy+20, cx+cw, cy+ch), 0, PATH_CAMERA_COMBO);

    vector<SimEntity*>* eVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;

    // set camera combo box
    int index = 0;
    for(it = eVector->begin(); it!= eVector->end(); ++it)
    {
        SimCamera* s = dynamic_cast<SimCamera*>(*it);
        if(s)
        {
            std::string name = (*it)->getName();
            std::wstring wname(name.length(), L' ');
            std::copy(name.begin(), name.end(), wname.begin());
            ccb->addItem(wname.c_str(),index);
        }
        index++;
    }

    s32 fx,fy,fw,fh;
    fx = width_r + 10;
    fy = cy + ch + 10;
    fw = width - width_r - 20;
    fh = 40;

    IGUIStaticText * ft = guienv->addStaticText(
        L"Folder Path: ",
        rect<s32>(fx,fy,fx+fw,fy+20),
        false, true);

    IGUIEditBox* filename =  guienv->addEditBox(
        L"result", rect<s32>(fx,fy+20,fx+fw,fy+fh), true, 0, PATH_FOLDER);


    s32 bx,by,bw,bh;
    bx = width_r + 10;
    bw = 80;
    by = fy + fh + 10;
    bh = 40;

    guienv->addButton(rect<s32>(bx,by+10,bx+bw,by+bh), 0,
                      RUN_BUTTON,
                      L"Run",
                      L"Run path with given camera");

    s32 sx,sy,sw,sh,ew;
    sx = bx + bw + 10;
    ew = 60;
    sw = width - width_r - 30 - bw - ew;
    sy = by;
    sh = 40;

    IGUIStaticText * st = guienv->addStaticText(
        L"Set FPS",
        rect<s32>(sx,sy,sx+sw,sy+20),
        false, true);

    IGUIScrollBar * scr =
        guienv->addScrollBar(true,
                             rect<s32>(sx,sy+20,sx+sw,sy+sh),
                             0,PATH_FPS_SCROLL);
    scr->setMin(1);
    scr->setMax(60);
    scr->setPos(30);
    scr->setSmallStep(1);
    scr->setLargeStep(5);

    guienv->addStaticText(
        L"",
        rect<s32>(sx + sw + 10, sy+20,sx + sw + ew, sy + sh),
        true,true,
        0, PATH_FPS);

    setPathFPS();

}
void SimGUI::updateCombos()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIComboBox* pc =
        (IGUIComboBox*)(rootelem->getElementFromId(PATH_CAMERA_COMBO, true));
    pc->clear();

    IGUIComboBox* cc =
        (IGUIComboBox*)(rootelem->getElementFromId(CAMERA_COMBO, true));
    cc->clear();

    vector<SimEntity*>* eVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;

    // set camera combo box
    int index = 0;
    cc->addItem(L"world camera",-1);
    for(it = eVector->begin(); it!= eVector->end(); ++it)
    {
        SimCamera* s = dynamic_cast<SimCamera*>(*it);
        if(s)
        {
            std::string name = (*it)->getName();
            std::wstring wname(name.length(), L' ');
            std::copy(name.begin(), name.end(), wname.begin());
            pc->addItem(wname.c_str(),index);
            cc->addItem(wname.c_str(),index);
        }
        index++;
    }
}

void SimGUI::setPathFPS()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    IGUIScrollBar* scr=
        (IGUIScrollBar*)(rootelem->getElementFromId(PATH_FPS_SCROLL, true));
    int val = scr->getPos();

    IGUIStaticText * fps =
        (IGUIStaticText*)(rootelem->getElementFromId(PATH_FPS, true));

    fps->setText(std::to_wstring(val).c_str());
}

void SimGUI::addPathNode()
{
    ISceneManager* smgr = device->getSceneManager();
    ICameraSceneNode * cc = smgr->getActiveCamera();
    paths->addPathNode(cc->getPosition(), cc->getRotation(),1);
}
//----------------------------------------------------------------------------//
//                              PATH EDIT WINDOW                              //
//----------------------------------------------------------------------------//
// sets attach window appropriately.
void SimGUI::editPathWindow()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    s32 wx,wy,ww,wh;
    wx = 100;
    wy = 100;
    ww = 600;
    wh = 240;

    IGUIWindow* window= guienv->addWindow(
        rect<s32>(wx, wy, wx+ww, wy+wh), true, L"Edit Path", 0, PATH_WINDOW);

    s32 cx,cy,cw,ch;
    s32 cbx;
    cx = 10;
    cw = ww - 2*cx;
    ch = 40;
    cy = 30;

    // set text
    IGUIStaticText* crt = guienv->addStaticText(
        L"", rect<s32>(cx, cy, cx+cw, cy+20), false, true, window);

    // set dropdown box
    IGUIComboBox *  pc= guienv->addComboBox(
        rect<s32>(cx, cy+20, cx+cw, cy+ch), window, PATH_COMBO);

    // add combobox items
    crt->setText(L"Choose Path Node to Edit: ");

    std::list<PathNode>* plist = paths->getPathList();
    std::list<PathNode>::iterator it;
    int index = 0;
    for(it = plist->begin(); it != plist->end() ; it++)
    {
        wstring p = L"Path ";
        p += std::to_wstring(index);
        pc->addItem(p.c_str(),index);
        index++;
    }

    s32 dx,dy,dw,dh,dm;
    dx = 10;
    dh = 100;
    dw = ww - 2*dx;
    dy = cy + ch + 10;
    dm = 70;
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

    IGUIStaticText* duration=
        guienv->addStaticText(
            L"Duration", rect<s32>(10,70,ddw,90), false, true, dof_box);

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
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sx, PATH_POS_X);

    IGUIEditBox* ey =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sy, PATH_POS_Y);

    IGUIEditBox* ez =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sz, PATH_POS_Z);

    IGUIEditBox* ea =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sa, PATH_ROT_A);

    IGUIEditBox* eb =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sb, PATH_ROT_B);

    IGUIEditBox* ec =  guienv->addEditBox(
        L"0.0", rect<s32>(tm,0,tw-10,20), true, sc, PATH_ROT_C);

    IGUIEditBox* ed =  guienv->addEditBox(
        L"0", rect<s32>(dm+tm,0,tw+dm-10,20), true, duration, PATH_DUR);

    s32 bx,by,bw,bh;
    by = dy+dh + 10;
    bw = 80;
    bx = ww - 10 - bw*3 - 20;
    bh = 40;

    guienv->addButton(rect<s32>(bx,by,bx+bw*1,by+bh), window,
                      PATH_REMOVE_BUTTON,
                      L"Delete",
                      L"Removes current node");

    guienv->addButton(rect<s32>(bx+bw*1+10,by,bx+bw*2+10,by+bh), window,
                      APPLY_BUTTON,
                      L"Apply",
                      L"Set with current settings");

    guienv->addButton(rect<s32>(bx+bw*2+20,by,bx+bw*3+20,by+bh), window,
                      CLOSE_BUTTON,
                      L"Close",
                      L"Cancel and close window");

    setPathData(0);
}
void SimGUI::setPathData(s32 index)
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    std::list<PathNode>* plist = paths->getPathList();
    std::list<PathNode>::iterator it = plist->begin();
    std::advance(it,(int)index);
    vector3df pos = (*it).Pos;
    vector3df rot = (*it).Rot;

    IGUIEditBox * ex =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_POS_X, true));

    IGUIEditBox * ey =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_POS_Y, true));

    IGUIEditBox * ez =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_POS_Z, true));

    IGUIEditBox * ea =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_ROT_A, true));

    IGUIEditBox * eb =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_ROT_B, true));

    IGUIEditBox * ec =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_ROT_C, true));

    IGUIEditBox * ed =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_DUR, true));

    ex->setText(to_wstring(pos.X).c_str());
    ey->setText(to_wstring(pos.Y).c_str());
    ez->setText(to_wstring(pos.Z).c_str());
    ea->setText(to_wstring(rot.X).c_str());
    eb->setText(to_wstring(rot.Y).c_str());
    ec->setText(to_wstring(rot.Z).c_str());
    ed->setText(to_wstring((*it).dur).c_str());
}
void SimGUI::savePathData()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    std::list<PathNode>* plist = paths->getPathList();
    std::list<PathNode>::iterator it = plist->begin();

    IGUIComboBox* cb=
        (IGUIComboBox*)(rootelem->getElementFromId(PATH_COMBO, true));

    IGUIEditBox * ex =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_POS_X, true));

    IGUIEditBox * ey =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_POS_Y, true));

    IGUIEditBox * ez =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_POS_Z, true));

    IGUIEditBox * ea =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_ROT_A, true));

    IGUIEditBox * eb =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_ROT_B, true));

    IGUIEditBox * ec =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_ROT_C, true));

    IGUIEditBox * ed =
        (IGUIEditBox*)(rootelem->getElementFromId(PATH_DUR, true));


    stringc cstr;
    int index = cb->getSelected();
    std::advance(it,index);

    // set Position
    cstr = ex->getText();
    double x = (double)atof(cstr.c_str());
    cstr = ey->getText();
    double y = (double)atof(cstr.c_str());
    cstr = ez->getText();
    double z = (double)atof(cstr.c_str());
    (*it).Pos = vector3df(x,y,z);

    cstr = ea->getText();
    double a = (double)atof(cstr.c_str());
    cstr = eb->getText();
    double b = (double)atof(cstr.c_str());
    cstr = ec->getText();
    double c = (double)atof(cstr.c_str());
    (*it).Rot = vector3df(a,b,c);

    cstr = ed->getText();
    int d = (int)atof(cstr.c_str());
    (*it).dur = d;
}
//----------------------------------------------------------------------------//
//                        ADD / EDIT ENTITY SCENENODE                         //
//----------------------------------------------------------------------------//
void SimGUI::addEntitySceneNode(EntityType type, SimEntity * obj)
{
    // null check
    if(!obj)
    {
    	cout<<"null pointer..."<<endl;
        return;
    }

    ISceneManager* smgr = device->getSceneManager();
    ISceneNode * r = smgr->getRootSceneNode();
    // create scene node that takes object
    Sim::SimSceneNode * node = new Sim::SimSceneNode(r,smgr,-1,obj);
    // if this is entity, set lighting to false.(to be seen easier)
    if(type == ENTITY_TYPE_ENVIRONMENT)
    {
        node->getMaterial(1).Lighting = false;
    }
    entityMeshVector.push_back(node);
    // reset camera dropdown menu
    updateCombos();
}
void SimGUI::removeEntitySceneNode(SimEntity * obj)
{
    // null check
    if(!obj)
        return;
    // find entity scene node by entity pointer
    vector<SimSceneNode*>::iterator it =
        std::find_if(entityMeshVector.begin(),
                     entityMeshVector.end(),
                     checkEntityPointer(obj));

    // remove all entity scene node that uses given obj as pointer
    while(it != entityMeshVector.end())
    {
        if(*it)
            (*it)->remove();
        it = std::find_if(std::next(it),
                          entityMeshVector.end(),
                          checkEntityPointer(obj));
    }
    entityMeshVector.erase(
        std::remove_if(entityMeshVector.begin(),
                       entityMeshVector.end(),
                       checkEntityPointer(obj)),
        entityMeshVector.end());
    updateCombos();
    /* setCameraDropdown(); */
    /* setPathExec(); */
}

//----------------------------------------------------------------------------//
//                          ATTACH ENTITY GUI WINDOW                          //
//----------------------------------------------------------------------------//
void SimGUI::attachEntityMesh(SimRobot * robot, SimSensor * sensor)
{
    // null check
    if(!robot)
        return;
    if(!sensor)
        return;

    // get first scenenode that uses robot ptr as entity
    vector<SimSceneNode*>::iterator it =
        std::find_if(entityMeshVector.begin(),
                     entityMeshVector.end(),
                     checkEntityPointer(robot));
    SimSceneNode * robotMesh = (*it);

    // same for sensor
    it = std::find_if(entityMeshVector.begin(),
                      entityMeshVector.end(),
                      checkEntityPointer(sensor));
    SimSceneNode * sensorMesh = (*it);

    // set robot as parent of sensor
    if(robotMesh != 0 && sensorMesh != 0)
        sensorMesh->setParent(robotMesh);

    // for possible case where sensor was camera, update sc.
    ((CameraSceneNode*)sc)->update();
}

void SimGUI::detachEntityMesh(SimRobot * robot, SimSensor * sensor)
{
    // null check
    if(!robot)
        return;
    if(!sensor)
        return;

    // get first scene node that uses robot as ptr
    vector<SimSceneNode*>::iterator it =
        std::find_if(entityMeshVector.begin(),
                     entityMeshVector.end(),
                     checkEntityPointer(robot));
    SimSceneNode* robotMesh = (*it);

    // same for sensor
    it = std::find_if(entityMeshVector.begin(),
                      entityMeshVector.end(),
                      checkEntityPointer(sensor));
    SimSceneNode* sensorMesh = (*it);

    // if they are not both null, set sensor's parent to root
    if(robotMesh != 0 && sensorMesh != 0)
    {
        ISceneManager * smgr = device->getSceneManager();
        sensorMesh->setParent(smgr->getRootSceneNode());
    }
    // for possible case where sensor was camera, update
    ((CameraSceneNode*)sc)->update();
}
// sets attach window appropriately.
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

// sets entity add / edit window appropriately.
void SimGUI::promptEntityWindow()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();

    // set common prompt window theme
    s32 wx, wy, ww, wh;
    wx = 150;
    wy = 30;
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
void SimGUI::capture()
{
    IVideoDriver * driver = device->getVideoDriver();
    ISceneManager * smgr = device->getSceneManager();

    ISceneCollisionManager * cm = smgr->getSceneCollisionManager();

    // create image and crop
    IImage *img = driver->createScreenShot();
    IImage * view = driver->createImage(
        ECF_A1R5G5B5,
        dimension2d<u32>(width_r,height_r));
    img->copyTo(view,
                position2d<s32>(0,0),
                rect<s32>(0,20,width_r,height_r+20));
    wstring imagename = cap_path + L"/res_" + std::to_wstring(cap_id)+ L".jpg";
    driver->writeImageToFile(
        view,
        imagename.c_str());
    img->drop();
    view->drop();

    // iterate scene nodes
    vector<SimSceneNode*>::iterator it;

    // if camera is sensor camera output capture data
    ICameraSceneNode * cc;
    if(!sc)
        return;
    cc = sc;

    // meta triangle selector for ray collision detection
    IMetaTriangleSelector* ts = smgr->createMetaTriangleSelector();

    // add all environment's bounding box for collision detection
    for(it = entityMeshVector.begin();
        it != entityMeshVector.end();
        it++)
    {
        // check whether entity is environment
        SimEnvironment* s = dynamic_cast<SimEnvironment*>((*it)->getEntity());
        if(s)
        {
            ts->addTriangleSelector(
                (smgr)->createTriangleSelectorFromBoundingBox(*it));
        }
    }

    // output file
    ofstream myfile;
    wstring resultname= cap_path + L"/res_" + std::to_wstring(cap_id)+ L".txt";
    std::string resname(resultname.begin(),resultname.end());
    myfile.open (resname);

    // get fov, position and rotation of camera
    double fovy = sc->getFOV();
    double fovx = 2.0f*atan(sc->getAspectRatio()*tan(fovy/2.0f));
    double fl = ((CameraSceneNode*)sc)->getFocalLength();

    double w = 2*tan(fovx/2.0f)*fl;
    double h = 2*tan(fovy/2.0f)*fl;

    double cx = (double)width_r / 2.0f;
    double cy = (double)height_r / 2.0f;

    vector3df cp =
        convertPos(((CameraSceneNode*)cc)->getCamera()->getPosition());

    vector3df cr =
        convertRot(((CameraSceneNode*)cc)->getCamera()->getRotation());

    position2d<s32> coord;

    // write camera info on first line
    myfile<<cp.X<<" "<<cp.Y<<" "<<cp.Z<<" "<<cr.X<<" "<<cr.Y<<" "<<cr.Z<<"\n";

    // iterate entity scennodes
    for(it = entityMeshVector.begin();
        it != entityMeshVector.end();
        it++)
    {
        // check whether entity is environment
        SimEnvironment* s = dynamic_cast<SimEnvironment*>((*it)->getEntity());
        if(s)
        {
            // check whether scene node is drawn
            if(!(smgr->isCulled(*it)))
            {
                vector<Point>* kv = s->getKeyPoints();
                vector<Point>::iterator itk;
                for(itk = kv->begin();
                    itk != kv->end();
                    itk++)
                {
                    // calculate keypoint's absolute postion
                    vector3df ip = (*it)->getPosition();
                    vector3df p = ip + convertPoint(*itk);
                    vector3df r = (*it)->getRotation();

                    p.rotateYZBy(r.X,ip);
                    p.rotateXZBy(r.Y,ip);
                    p.rotateXYBy(r.Z,ip);

                    coord = cm->getScreenCoordinatesFrom3DPosition(p,cc,false);
                    coord.X *= ((double)width_r/(double)width);
                    coord.Y *= ((double)height_r/(double)height);

                    // if point is between frustum angles, check for collision
                    if(coord.X>= 0 && coord.X <= width_r &&
                       coord.Y >= 0 && coord.Y <= height_r)
                    {
                        vector3df rp = p;
                        triangle3df rt;
                        ISceneNode * ret = 0;

                        // check for collision. if covered is true, it means
                        // that it may be blocked by some environment entity
                        bool covered =
                            cm->getCollisionPoint(
                                line3d<f32>(cp,rp), ts, p, rt, ret);

                        // if not blocked, add to file
                        if(!covered || rp == p)
                        {
                            myfile<<coord.X<<" "<<coord.Y<<" "<<
                                p.X<<" "<<p.Y<<" "<<p.Z<<"\n";
                        }
                    }
                }
            }
        }
    }
    ts->drop();
    myfile.close();
    cap_id++;
}
// quick little helper function to convert point to vector
vector3df SimGUI::convertPoint(Point p)
{
    return vector3df(p.x,p.y,p.z);
}

vector3df SimGUI::convertPos(Position p)
{
    return vector3df(p.X,p.Y,p.Z);
}

vector3df SimGUI::convertRot(Rotation p)
{
    return vector3df(p.Roll,p.Pitch,p.Yaw);
}

//----------------------------------------------------------------------------//
//                        GUI PRIVATE HELPER FUNCTIONS                        //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                              CONTEXT MENU GUI                              //
//----------------------------------------------------------------------------//
void SimGUI::setContextMenu()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    // add context menu(toolbar menu)
    IGUIContextMenu * cM = guienv->addMenu();

    //---------------------------Top Level Menu-------------------------------//
    // menu dropdown for context menu
    u32 engineMenuId = cM->addItem(L"Engine", 1, true, true, false, false);

    u32 entityMenuId = cM->addItem(L"Entity", 2, true, true, false, false);
    u32 pathMenuId = cM->addItem(L"Path", 3, true, true, false, false);

    IGUIContextMenu * engineMenu = cM->getSubMenu(engineMenuId);
    IGUIContextMenu * entityMenu = cM->getSubMenu(entityMenuId);
    IGUIContextMenu * pathMenu = cM->getSubMenu(pathMenuId);

    //---------------------------Engine Menu----------------------------------//

	engineMenu->addItem(L"Show Features", FEATURE_BUTTON,
                        true, false, true, false);
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

    //----------------------Path Edit menu------------------------//
	pathMenu->addItem(L"Edit", EDIT_PATH, true, false, false, false);
}
//----------------------------------------------------------------------------//
//                             CAMERA SELECT GUI                              //
//----------------------------------------------------------------------------//
void SimGUI::setCameraDropdown()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    s32 cx,cy,cw,ch;
    cx = width_r + 10;
    cw = width - width_r -20;
    cy = 40;
    ch = 40;

    IGUIStaticText * ct = guienv->addStaticText(
        L"Choose Camera",
        rect<s32>(cx,cy,cx+cw,cy+20),
        false, true);
    IGUIComboBox* ccb = guienv->addComboBox(
        rect<s32>(cx, cy+20, cx+cw, cy+ch), 0, CAMERA_COMBO);

    vector<SimEntity*>* eVector = engine->getEntityVector();
    vector<SimEntity*>::iterator it;

    ccb->addItem(L"World Camera",-1);
    int index = 0;
    for(it = eVector->begin(); it!= eVector->end(); ++it)
    {
        SimCamera* s = dynamic_cast<SimCamera*>(*it);
        if(s)
        {
            std::string name = (*it)->getName();
            std::wstring wname(name.length(), L' ');
            std::copy(name.begin(), name.end(), wname.begin());
            ccb->addItem(wname.c_str(),index);
        }
        index++;
    }

}

void SimGUI::setCameraCapture()
{
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGUIElement * rootelem = guienv->getRootGUIElement();
    s32 cx,cy,cw,ch;
    cx = width_r + 10;
    cw = width - width_r -20;
    cy = 90;
    ch = 30;
    guienv->addButton(rect<s32>(cx,cy,cx+cw,cy+ch), 0,
                      CAPTURE_BUTTON,
                      L"Capture",
                      L"Captures Current scene");
}


//----------------------------------------------------------------------------//
//                      ENTITY ADD/EDIT MENU WINDOW GUI                       //
//----------------------------------------------------------------------------//
void SimGUI::createEntityObject(EntityType type, SimEntity* obj)
{
    engine->addEntity((EntityType)currType, obj);
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

    core::list<IGUIElement*> advChildren = adv_box->getChildren();
    core::list<IGUIElement*>::Iterator lit = advChildren.begin();;
    for(int i = 0; i < advChildren.size(); i++,lit++,it++)
    {
        cstr = (*lit)->getElementFromId(PROMPT_ADV_INPUT)->getText();
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
    currObj->update();
    ((CameraSceneNode*)sc)->update();
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
            obj = new SimGroundRobot("",0,0,0,0,0,0,0,0,0);
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
        case SUB_ENTITY_ENVIRONMENT_PLANE:
        {
            obj = new SimPlane("",0,0,0,0,0,0,0,0,0);
            currObj = obj;
            break;
        }
        case SUB_ENTITY_ENVIRONMENT_CUBE:
        {
            obj = new SimGroundRobot("",0,0,0,0,0,0,0,0,0);
            currObj = obj;
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
    core::list<IGUIElement*> advChildren = adv_box->getChildren();
    core::list<IGUIElement*>::Iterator lit;
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
            cb->addItem(L"Plane",SUB_ENTITY_ENVIRONMENT_PLANE);
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
                SimEnvironment* s = dynamic_cast<SimEnvironment*>(*it);
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

//----------------------------------------------------------------------------//
//                      ENTITY ATTACH/DETACH WINDOW GUI                       //
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
