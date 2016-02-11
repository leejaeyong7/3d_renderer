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

    // sets up common prompt window gui objects
    void setPromptWindow();

    // adds advanced option attribute 
    void addPromptAdvancedAttribute(stringw p_name, s32 gui_id);

    // Sets up context menu
    void setContextMenu();

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

    // prompt window parameters 
    s32 px,py,pw,ph;
    s32 window_offset;
    s32 boxMargin;
    s32 combo_box_x,combo_box_y,combo_box_w,combo_box_h;
    s32 combo_x,combo_y,combo_w,combo_h;
    s32 name_x,name_y,name_w,name_h;
    s32 nameInputMargin, nameText_w_offset;
    s32 boxInputMargin;
    s32 boxText_h_margin,boxText_w_margin, boxText_w_offset;
    s32 boxInputText_h, boxInputText_w;
    s32 dof_x, dof_y, dof_w, dof_h;
    s32 boxWidth;
    s32 boxInputText_w_offset;
    s32 advSetting_y, advSetting_h;
    s32 buttons_y, buttons_h;
};
#endif
