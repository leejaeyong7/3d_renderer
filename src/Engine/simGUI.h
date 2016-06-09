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
#include <numeric> 
#include <typeinfo>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <ctime>
#include "enumerations.h"
#include "eventHandler.h"
#include "simEngine.h"
#include "simEntity.h"
#include "simRobot.h"
#include "simGroundRobot.h"
#include "simSensor.h"
#include "simCamera.h"
#include "simEnvironment.h"
#include "simPlane.h"
#include "simFloor.h"
#include "simCube.h"
#include "simGridNode.hpp"
#include "simEntitySceneNode.hpp"
#include "simCameraSceneNode.hpp"
#include "simWorldCameraSceneNode.hpp"
#include "RTSCamera.h"
#include "simPathSceneNode.hpp"
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
using namespace Sim;

//----------------------------------------------------------------------------//
//                                Global Variables
//----------------------------------------------------------------------------//
typedef struct EntityMesh{
    SimEntity* obj;
    Sim::SimSceneNode* node;
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
     * Updates position/rotation of entities
     * @return None
     */
    void update();

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
     * Attaches Sensor to Camera
     * @param SimRobot* robot - robot to attach sensor on
     * @param SimSensor* sensor- sensor to attach 
     * @return none
     */
    void attachEntityMesh(SimRobot * robot, SimSensor * sensor);

    /**
     * removes Sensor from Camera
     * @param SimRobot* robot - robot to detach sensor on
     * @param SimSensor* sensor- sensor to remove 
     * @return none
     */
    void detachEntityMesh(SimRobot * robot, SimSensor * sensor);
    /**
     * prompts user with modal window
     * @param none
     * @return creates prompt window accordingly to prompt type
     */
    void promptEntityWindow();

    /**
     * prompts user with entity attach window
     * @param none
     * @return attaches or detachs sensor to/from robot
     */
    void entityAttachWindow();


    void editPathWindow();

private:
    // Pointer to SimEngine
    SimEngine * engine;

    // Irrlicht 3D engine device
    IrrlichtDevice* device;
    
    // event handler for device
    EventHandler * eh;

    // world camera
    ICameraSceneNode * wc;
    
    // attached camera
    ICameraSceneNode * sc;

    // minimap camera
    ICameraSceneNode * mc;

    // world cameras
    // fps camera
    ICameraSceneNode * fc;

    // maya camera
    ICameraSceneNode * yc;

    // Placer camera
    ICameraSceneNode * pc;

    // holds paths
    Sim::PathSceneNode * paths;

    // window sizes
    u32 width, height, width_r, height_r;
    rect<s32> renderView;

    // Entity Mesh vector
    vector<SimSceneNode*> entityMeshVector;

    // converts point into vector
    vector3df convertPoint(Point p);
    vector3df convertPos(Position p);
    vector3df convertRot(Rotation p);

    // sets data for prompt window
    void setPromptData(SimEntity* obj);
    void setAddPromptData(s32 index);
    void setEditPromptData(s32 index);

    // used for entity manipulation
    SimEntity* currObj;
    SimEntity* placeObj;
    SimSceneNode * placeMesh;
    int currPrompt;
    int currType;
    void createEntityObject(EntityType type, SimEntity* obj);
    void editEntityObject();

    // used for camera manipulation
    void setCameraDropdown();
    void setCameraCapture();
    void capture();

    // sets up common prompt window gui objects
    void setPromptWindow(s32 wx, s32 wy, s32 ww, s32 wh);
    void setPromptComboBox(s32 cx, s32 cy, s32 cw, s32 ch);
    void setNameBox(s32 nx, s32 ny, s32 nw, s32 nh, s32 nm);
    void setDofBox(s32 dx, s32 dy, s32 dw, s32 dh, s32 dm);
    void setAdvancedSetting(s32 ax, s32 ay, s32 aw, s32 ah);
    void setButtons(s32 bx, s32 by, s32 bw, s32 bh);

    // attach/detach entity object based on selected combo box name
    void attachEntityObject();
    void setDetachData(s32 index);

    // sets placer menu
    void setPlacerMenu();
    void setPlacerSubMenu(int type);

    // sets up context menu(toolbar)
    void setContextMenu();

    // sets up path execute menu
    void setPathExec();
    void setPathFPS();
    void addPathNode();
    void execPath();
    void execUpdate();
    
    void updateCombos();
    
    // save/set up path data
    void savePathData();
    void setPathData(s32 index);

    // functor for remove_if predicate
    struct checkEntityPointer
    {
        checkEntityPointer(SimEntity* ptr) : ptr_holder(ptr) {}
        SimEntity* ptr_holder;
        bool operator()(SimSceneNode* obj)
        {
            return obj->getEntity() == ptr_holder;
        }
    };
    bool exec;
    bool placeMode;
    int cap_id;
    int cap_fps;
    int curr_node;
    wstring cap_path;
    int cap_node_count;
    clock_t curr_t,prev_t;
};
#endif
