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
class SimEngine
{
public:

    /**
     * Constructor
     * Initialize Irrlicht, Entities, and physics
     */
    SimEngine(const wchar_t * text,
              u32 width, u32 height, 
              u32 width_r, u32 height_r, 
              bool fullscreen);

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

    // pointer to driver for drawing
    IVideoDriver * driver;

    // Pointer to SimGUI object that handles irrlicht GUI design
    SimGUI * simGUI;

    // Pointer to SimPhysics object that handles all the physics works in
    // simulation engine
    SimPhysics * simPhysics;

    // vector of SimEntity object
    vector<SimEntity*> simEntityVector;
    
    // vector of mesh scene node for entities
    vector<IMeshSceneNode*> meshSceneNodeVector;
}
#endif
