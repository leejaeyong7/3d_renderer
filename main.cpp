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
#include "SimEngine.h"

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
                                          WINDOW_WIDTH+GUI_WIDTH,
                                          WINDOW_HEIGHT,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          false);
    simEngine->run();
}
