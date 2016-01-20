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
#include <irrlicht.h>
#include <vector>
#include "simEngine.h"
#include "eventHandler.h"
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


//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimEngine;
class SimGUI
{
public:
    /**
     * Default Constructor for Simulation GUI
     * Initializes Irrlicht device
     * @param SimEngine * - simulator engine for running simengine methods
     * @see SimEngine
     * @return None
     */
    SimGUI(SimEngine * eng);
    /**
     * Draws GUI elements on device
     * @return None
     */
    void draw();

private:
    // Pointer to SimEngine
    SimEngine * engine;
    
    // event handler for device
    EventHandler * eh;

    // Contains pointer to GUI environment manager of Irrlicht
    IGUIEnvironment * guienv;
};
#endif
