/*============================================================================
 * @author: Jae Yong Lee
 * @file: main.cpp
 * @version: 1.0 
 * @summary:
 *      contains main loop for 3D rendering software using irrlicht
 *
 *============================================================================*/

//----------------------------------------------------------------------------//
//                               Pre Processors
//----------------------------------------------------------------------------//
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define GUI_WIDTH 200
#define WINDOW_CAPTION L"TEST WINDOW"

//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include <irrlicht.h>
#include "eventHandler.h"

//----------------------------------------------------------------------------//
//                             namespace declarations
//----------------------------------------------------------------------------//
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//----------------------------------------------------------------------------//
//                                  Main function
//----------------------------------------------------------------------------//
int main()
{
    // declare Event Handler
    // see eventhandler.h for detail
    EventHandler handler;

    // declare Irrlicht device
    // sets parameters for root Irrlicht object
    IrrlichtDevice * device =
        createDevice(
            // device type : software
            // using video namespace
            // is able to choose between EDT_OPENGL, EDT_DIRECT3D8/9, etc
            //EDT_SOFTWARE,
            EDT_OPENGL,

            // window size
            // dimension2d from core namespace
            // using macro values for window size
            dimension2d<u32>(WINDOW_WIDTH,WINDOW_HEIGHT),

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
            &handler
            );

    // checks device is valid
    if(!device)
        return -1;

    // sets window name using macro value
    device->setWindowCaption(WINDOW_CAPTION);

    // sets IVideoDriver object
    // under video namespace
    // IrrlichtDevice->getVideoDriver() grants access to video driver
    // for 3D / 2D rendering
    IVideoDriver* driver = device->getVideoDriver();

    // sets ISceneManager object
    // under scene namespace
    // IrrlichtDevice->getSceneManager() grants access to scene manager
    // scene manager covers scene nodes, mesh resource, camera handlers
    ISceneManager* smgr = device->getSceneManager();

    // set camera scene node as FPS (alternative)
    // this will set mouse action as FPS-like environment
    // sets key map for fps-like traversal
    SKeyMap keyMap[9];
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

    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_SPACE;

    ICameraSceneNode * camera =  smgr->addCameraSceneNodeFPS(
        // parent node : default null
        0, 
        // rotation speed : default 100
        100,
        // movement speed : default 0.5
        0.03, 
        // id
        -1,
        // key mapping input
        keyMap,
        // key mapping size
        9,
        // disables vertical traversal
        true,
        // jump speed
        5.0f
        );

    // set camera position
    camera->setPosition(vector3df(0,5,0));
    // sets far value of camera
    // extended for sun support
    camera->setFarValue(20000.0f);
    // hides mouse
    device->getCursorControl()->setVisible(false);

    // sets IGUIEnvironment object
    // under gui namespace
    // grants access to GUI environment
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    
    // sets material of the floor
    SMaterial * floor_material = new SMaterial();
    if(floor_material)
    {
        // floor will be affected by ambient light
        floor_material->AmbientColor  = SColor(255, 110, 110, 110);
        floor_material->DiffuseColor  = SColor(255, 0, 0, 0);
        floor_material->SpecularColor = SColor(255, 0, 0, 0);
        // and emit gray-ish color
        floor_material->EmissiveColor = SColor(255, 110, 110, 110);
        floor_material->ColorMaterial = ECM_NONE;
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
        floor_material
        );
    IAnimatedMeshSceneNode *floor=smgr->addAnimatedMeshSceneNode(movingplane);

    // add cube scene node for testing
    ISceneNode * cube_node = smgr->addCubeSceneNode();
    if(cube_node)
    {
        cube_node->setPosition(vector3df(40,15,1));
        cube_node->setRotation(vector3df(10,0,0));

        cube_node->getMaterial(0).AmbientColor  = SColor(255, 255, 255, 255);
        cube_node->getMaterial(0).DiffuseColor  = SColor(255, 0, 255, 255);
        cube_node->getMaterial(0).ColorMaterial = ECM_DIFFUSE;

    }
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

    // decalre FPS tracker variable 
    int lastFPS = -1;

    // get static time for frame length check
    u32 then = device->getTimer()->getTime();

    // set constant moving speed
    const f32 MOVEMENT_SPEED = 5.f;

    // main loop for drawing
    while(device->run())
    {
        if(device->isWindowActive())
        {
//------------------------------TIME TRACK------------------------------------//
            // calculate frame rate(frame delta time)
            const u32 now = device->getTimer()->getTime();

            // calculate time value in second
            const f32 frameDeltaTime = (f32)(now-then) / 1000.0f;
        
            // update past static time
            then = now;
//------------------------------SCENE NODE UPDATE-----------------------------//

//------------------------------SCENE RENDERERING-----------------------------//
            // all drawings must be placed between beginScene and endScene
            driver->beginScene(true, true, SColor(255,179,234,252));

            // draws scene manager drawings
            smgr->drawAll();

            // draws gui drawings
            guienv->drawAll();

            // ends irrlict drawing
            driver->endScene();
        }
        else
            device->yield();
    }
    // deletes device and free memory
    device->drop();
    return 0;
}
