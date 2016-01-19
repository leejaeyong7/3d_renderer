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
#include <irrlicht.h>
#include <vector>

#include 

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
    SimEngine();
    /**
     * Runs Simulation with physics, environment, and 3D rendering
     */
    void run();

private:

    // Pointer to SimGUI object that handles irrlicht GUI design
    SimGUI * simGUI;

    // Pointer to SimPhysics object that handles all the physics works in
    // simulation engine
    SimPhysics * simPhysics;

    // Pointer to head of linked list of entities
    vector<SimEntity> simEntityVector;

}
#endif
