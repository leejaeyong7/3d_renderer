/*============================================================================
 * @author: Jae Yong Lee
 * @file: simGUI.h
 * @version:  
 * @summary:
 *      Declaration file for simulating GUI objects
 *
 *============================================================================*/

//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_GUI_H_
#define _SIM_GUI_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include "simEngine.h"
#include "eventHandler.h"
#include <irrlicht.h>
#include <vector>

//----------------------------------------------------------------------------//
//                                  Namespaces
//----------------------------------------------------------------------------//
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//----------------------------------------------------------------------------//
//                                Global Variables
//----------------------------------------------------------------------------//

enum
{
    GUI_ID_QUIT_BUTTON = 0x10000;
}


//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimGUI
{
public:
    /**
     * Default Constructor for Simulation GUI
     * Initializes Irrlicht device
     * @param SimEngine * - simulator engine for running simengine methods
     * @param u32 width - window width
     * @param u32 height - window height 
     * @param u32 width_r - rendering width
     * @param u32 height_r - rendering height
     * @param bool fullscreeen - True to run on full screen mode
     * @see SimEngine
     * @return None
     */
    SimGUI(SimEngine * eng);
    /**
     * Draws GUI elements on device
     * @return None
     */
    void Draw();

private:
    // Pointer to SimEngine
    SimEngine * engine;
    
    // event handler for device
    eventHandler * eh;

    // Contains pointer to GUI environment manager of Irrlicht
    IGUIEnvironment * guienv;
};
#endif
