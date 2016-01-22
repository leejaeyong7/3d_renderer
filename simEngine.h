/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEngine.h
 * @version:  
 * @summary:
 *      Declaration file for simulator engine
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ENGINE_H_
#define _SIM_ENGINE_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <vector>
#include "simGUI.h"
#include "simPhysics.h"
#include "simEntity.h"
#include <irrlicht.h>

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
class SimGUI;
class SimEngine
{
public:

    /**
     * Constructor
     * Initialize Irrlicht, Entities, and physics
     * @param wchar_t text - window title
     * @param u32 m_width - window width
     * @param u32 m_height - window height 
     * @param u32 m_width_r - rendering width
     * @param u32 m_height_r - rendering height
     * @param bool fullscreeen - True to run on full screen mode
     */
    SimEngine(const wchar_t * text,
              u32 m_width, u32 m_height, 
              u32 m_width_r, u32 m_height_r, 
              bool fullscreen);
    /**
     * sets up simGUI
     * @param None
     * @return None
     */
    void setupGUI();

    /**
     * sets up FPS camera
     * @param None
     * @return None
     */
    void setupRenderingCamera();
    
    /**
     * Runs Simulation with physics, environment, and 3D rendering
     */
    void run();
    
    /**
     * fetch Irrlicht device pointer
     * @return pointer to Irrlicht device object
     */
    IrrlichtDevice * getDevice(){return device;};

    /**
     * fetch SimGUI object pointer
     */
    SimGUI * getSimGUI(){return simGUI;};


    /**
     * fetch pointer to entity vector
     * @param 
     * @see 
     * @return 
     */
    vector<SimEntity*> * getEntityVector(){return &simEntityVector;};
    
    /**
     * adds entity to rendering
     * @param SimEntity - entity object to add
     * @see SimEntity
     * @return None
     */
    void addEntity(SimEntity * obj);

    /**
     * removes entity object from rendering
     * if entity object doesn't exist, it will do nothing
     * @param SimEntity - pointer to entity object to remove
     * @return None
     */
    void removeEntity(SimEntity * obj);

private:
    // pointer to Irrliche device for drawing entities and gui
    IrrlichtDevice * device;

    // window width/height
    u32 width;
    u32 height;

    // rendering width/height
    u32 width_r;
    u32 height_r;

    // Pointer to SimGUI object that handles irrlicht GUI design
    SimGUI * simGUI;

    // Pointer to SimPhysics object that handles all the physics works in
    // simulation engine
    SimPhysics * simPhysics;

    // vector of SimEntity object
    vector<SimEntity*> simEntityVector;
};
#endif
