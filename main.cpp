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
#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 600
#define RENDERING_WIDTH 640 
#define RENDERING_HEIGHT 480
#define WINDOW_CAPTION L"TEST WINDOW"

//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include <irrlicht.h>
#include "simEngine.h"

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
    SimEngine * simEngine = new SimEngine(WINDOW_CAPTION,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          RENDERING_WIDTH,
                                          RENDERING_HEIGHT,
                                          false);
    simEngine->setupGUI();
    simEngine->run();
}
