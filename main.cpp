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
//------------------------------Initialization--------------------------------//

    RenderingDevice context;
    // sets parameters for root Irrlicht object
    context.device =
        createDevice(
            // device type : opengl 
            // using video namespace
            // is able to choose between EDT_OPENGL, EDT_DIRECT3D8/9, etc
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
            0
            );

    // checks device is valid
    if(!context.device)
        return -1;

    // sets window name using macro value
    context.device->setWindowCaption(WINDOW_CAPTION);

    // sets IVideoDriver object
    // under video namespace
    // IrrlichtDevice->getVideoDriver() grants access to video driver
    // for 3D / 2D rendering
    IVideoDriver* driver = context.device->getVideoDriver();

    // sets ISceneManager object
    // under scene namespace
    // IrrlichtDevice->getSceneManager() grants access to scene manager
    // scene manager covers scene nodes, mesh resource, camera handlers
    ISceneManager* smgr = context.device->getSceneManager();

    // sets IGUIEnvironment object
    // under gui namespace
    // grants access to GUI environment
    IGUIEnvironment* guienv = context.device->getGUIEnvironment();
//-------------------------------GUI Setting----------------------------------//
    guienv->addButton(rect<s32>(10,240,110,240 + 32), 0,101, 
                   L"Quit", L"Exits Program");
    guienv->addButton(rect<s32>(10,280,110,280 + 32), 0, 102, 
                   L"New Window", L"Launches a new Window");
    guienv->addButton(rect<s32>(10,320,110,320 + 32), 0, 103, 
                   L"File Open", L"Opens a file");
//------------------------------Camera Setting--------------------------------//
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

    context.camera[0] =  smgr->addCameraSceneNodeFPS(
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
        0.8f
        );

    // set camera position
    context.camera[0]->setPosition(vector3df(0,5,0));
    // sets far value of camera
    // extended for sun support
    context.camera[0]->setFarValue(20000.0f);
    // hides mouse
    context.device->getCursorControl()->setVisible(false);


    context.camera[1] = smgr->addCameraSceneNodeMaya();
    context.camera[1]->setFarValue(20000.0f);
//------------------------------Event Setting---------------------------------//
    
    EventHandler handler(context);
    context.device->setEventReceiver(&handler);
    handler.setActiveCamera(context.camera[0]);

//------------------------------Mesh/Model Creation---------------------------//
    
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

//------------------------------Collision Handling----------------------------//
    // create Meta triangle selector for world collision detection
    IMetaTriangleSelector * metaSelector = smgr->createMetaTriangleSelector();
    // selector for individual triangles
    ITriangleSelector* selector = 0;
    
    // assign triangleselector for floor and
    // add floor to meta selector
    if (floor)
    {
        selector = smgr->createOctreeTriangleSelector(
            floor->getMesh(), floor, 128);
        floor->setTriangleSelector(selector);
        metaSelector->addTriangleSelector(selector);
        selector->drop();
    }
    // assign triangle mesh to cube node
    // add cube node to meta selector
    if(cube_node)
    {
        selector = smgr->createTriangleSelectorFromBoundingBox(cube_node);
        cube_node->setTriangleSelector(selector);
        metaSelector->addTriangleSelector(selector);
        selector->drop();
    }
    // use meta selector to check collisions
    if(metaSelector)
    {
        ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            // triangle selector
            metaSelector, 
            // camera scene node
            context.camera[0], 
            // collision detection radius
            vector3df(3,5,3),
            // gravity per sec
            vector3df(0,-3,0),
            // ellipsoid translation
            vector3df(0,0,0));
        metaSelector->drop();
        context.camera[0]->addAnimator(anim);
        anim->drop(); 
    }

    
//------------------------------Loop Setting----------------------------------//
    // decalre FPS tracker variable 
    int lastFPS = -1;    // get static time for frame length check
    u32 then = context.device->getTimer()->getTime();

    // set constant moving speed
    const f32 MOVEMENT_SPEED = 5.f;

    // main loop for drawing
    while(context.device->run())
    {
        if(context.device->isWindowActive())
        {
//------------------------------TIME TRACK------------------------------------//
            // calculate frame rate(frame delta time)
            const u32 now = context.device->getTimer()->getTime();

            // calculate time value in second
            const f32 frameDeltaTime = (f32)(now-then) / 1000.0f;
        
            // update past static time
            then = now;
 
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
            context.device->yield();
    }
    // deletes device and free memory
    context.device->drop();
    return 0;
}
