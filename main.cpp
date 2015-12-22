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
#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 480
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
            dimension2d<u32>(WINDOW_HEIGHT,WINDOW_WIDTH),

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

    // sets IGUIEnvironment object
    // under gui namespace
    // grants access to GUI environment
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    
    // add sphere scene node for testing movement
    ISceneNode * test_node = smgr->addSphereSceneNode();
    if(test_node)
    {
        test_node->setPosition(vector3df(0,0,15));
        test_node->setMaterialFlag(EMF_WIREFRAME,true);
    }

    // add cube scene node for static (relative to test node) 
    ISceneNode * static_node = smgr->addCubeSceneNode();
    if(static_node)
    {
        static_node->setPosition(vector3df(0,0.3,1));
        static_node->setRotation(vector3df(30,10,30));
        static_node->setScale(vector3df(0.07,0.07,0.07));
        static_node->setMaterialFlag(EMF_WIREFRAME,true);
        // add test node to its children for test movement
        static_node->addChild(test_node);
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
            // get current testnode position
            vector3df nodePos = test_node->getPosition();

            if(handler.IsKeyDown(KEY_KEY_W))
                nodePos.Y += MOVEMENT_SPEED * frameDeltaTime;
            else if(handler.IsKeyDown(KEY_KEY_S))
                nodePos.Y -= MOVEMENT_SPEED * frameDeltaTime;

            if(handler.IsKeyDown(KEY_KEY_A))
                nodePos.X -= MOVEMENT_SPEED * frameDeltaTime;
            else if(handler.IsKeyDown(KEY_KEY_D))
                nodePos.X += MOVEMENT_SPEED * frameDeltaTime;

            test_node->setPosition(nodePos);
//------------------------------SCENE RENDERERING-----------------------------//
            // all drawings must be placed between beginScene and endScene
            driver->beginScene(true, true);

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
