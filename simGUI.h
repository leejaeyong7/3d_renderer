/*============================================================================
 * @author: Jae Yong Lee
 * @file: simGUI.h
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
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <string>
#include "enumerations.h"
#include "eventHandler.h"
#include "simEngine.h"
#include "simEntity.h"
#include "simRobot.h"
#include "simSensor.h"
#include "simSensorCamera.h"
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
typedef struct EntityMesh{
    SimEntity* obj;
    IMeshSceneNode * mesh;
} EntityMesh;
//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class EventHandler;
class SimEntity;
class SimEngine;
class SimGUI
{
public:
    friend class EventHandler;
    /**
     * Default Constructor for Simulation GUI
     * Initializes Irrlicht device
     * @param SimEngine * - simulator engine for running simengine methods
     * @see SimEngine
     * @return None
     */
    SimGUI(SimEngine * eng, const wchar_t * text,
           u32 m_width, u32 m_height,
           u32 m_width_r, u32 m_height_r,
           bool fullscreen);

    /**
     * Sets up GUI environment
     * @return None
     */
    void setup();

    /**
     * Draws GUI elements on device
     * @return None
     */
    void draw();

    /**
     * Checks device is running
     * @return None
     */
    bool isRunning();
    
    /**
     * Finish irrlicht engine
     * @return None
     */
    void end();

    /**
     * Adds SceneNode of entity
     * @param SimEntity * - pointer of entity object to add
     * @return None
     */
    void addEntitySceneNode(EntityType type, SimEntity * obj);

    /**
     * remove SceneNode of entity
     * @param SimEntity * - pointer of entity object to remove 
     * @return None
     */
    void removeEntitySceneNode(SimEntity * obj);


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

    /**
     * prompts user with modal window
     * @param s32 prompt - Prompt type
     * @param u32 entityType - entity type
     * @return creates prompt window accordingly to prompt type
     */
    void promptWindow(s32 prompt, u32 entityType);

private:
    // Pointer to SimEngine
    SimEngine * engine;

    // Irrlicht 3D engine device
    IrrlichtDevice* device;
    
    // event handler for device
    EventHandler * eh;

    // window sizes
    u32 width, height, width_r, height_r;

    // Entity Mesh vector
    vector<EntityMesh> entityMeshVector;

    // sets data for prompt window
    void setPromptData(s32 prompt, s32 index);

    // sets up common prompt window gui objects
    void setPromptWindow(s32 prompt, u32 entityType,
                         s32 wx, s32 wy, s32 ww, s32 wh);

    void setPromptComboBox(s32 prompt, u32 entityType,
                           s32 cx, s32 cy, s32 cw, s32 ch);

    void setNameBox(s32 prompt, s32 nx, s32 ny, s32 nw, s32 nh, s32 nm);

    void setDofBox(s32 prompt, s32 dx, s32 dy, s32 dw, s32 dh, s32 dm);

    void setAdvancedSetting(s32 prompt, u32 entityType,
                            s32 ax, s32 ay, s32 aw, s32 ah);

    void setButtons(s32 prompt, s32 bx, s32 by, s32 bw, s32 bh);

    void setContextMenu();

    // functor for remove_if predicate
    struct checkEntityPointer
    {
        checkEntityPointer(SimEntity* ptr) : ptr_holder(ptr) {}
        SimEntity* ptr_holder;
        bool operator()(EntityMesh obj)
        {
            return obj.obj == ptr_holder;
        }
    };

};
#endif
