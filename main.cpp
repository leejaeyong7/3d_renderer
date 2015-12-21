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
    // declare Irrlicht device
    // sets parameters for root Irrlicht object
    IrrlichtDevice * device =
        createDevice(
            // device type : software
            // using video namespace
            // is able to choose between EDT_OPENGL, EDT_DIRECT3D8/9, etc
            EDT_SOFTWARE,

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

            // event receiver ptr : null (default null)
            0
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
    guienv->addStaticText(
        L"testing..",
        rect<s32>(10,10,260,22),
        true
        );
    
    // main loop for drawing
    while(device->run())
    {
        // all drawings must be placed between beginScene and endScene
        driver->beginScene(true, true, SColor(255,100,101,140));

        // draws scene manager drawings
        smgr->drawAll();

        // draws gui drawings
        guienv->drawAll();

        driver->endScene();
    }

    // deletes device and free memory
    device->drop();
    return 0;
}
