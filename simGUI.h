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
#include <stdlib.h>
#include <vector>
#include <typeinfo>
#include <string>
#include "eventHandler.h"
#include "simEngine.h"
#include "simEntity.h"
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
class EventHandler;
class SimEntity;
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
    
    /**
     * prompts user for adding entity
     * @param int - entity type
     * @see 
     * @return calls addEntity in SimEngine
     */
    void promptAddEntity(u32 entityType);

    /**
     * prompts user for editing entity
     * @param int - entity type
     * @see 
     * @return calls addEntity in SimEngine
     */
    void promptEditEntity(u32 entityType);

    /**
     * creates entity object from add window
     * @param none
     * @see 
     * @return SimEntity object with given params
     */
    SimEntity * createEntityObject();

    /**
     * edits entity object from edit window
     * @param none
     * @see 
     * @return SimEntity object with fixed params
     */
    SimEntity * editEntityObject(SimEntity * obj);

    /**
     * checks entity creation is valid
     * @param none
     * @see 
     * @return bool - true if valid
     */
    bool checkEntityValid();

    /**
     * alerts user for failing to create entity
     * @param none
     * @see 
     * @return opens alert window
     */
    void alertCreationFailure(const wchar_t*  message);
    

    // enables/disables GUI element of window
    void setAddPromptWindowEnabled(bool enabled);
    void setEditPromptWindowEnabled(bool enabled);

private:
    // Sets up add entity Prompt Window
    void setAddPromptWindow(s32 x, s32 y, s32 w, s32 h);

    // Sets up edit entity prompt window
    void setEditPromptWindow(s32 x, s32 y, s32 w, s32 h);

    // Sets up context menu
    void setContextMenu();

    // Pointer to SimEngine
    SimEngine * engine;
    
    // event handler for device
    EventHandler * eh;

    // Contains pointer to GUI environment manager of Irrlicht
    IGUIEnvironment * guienv;
};
#endif
