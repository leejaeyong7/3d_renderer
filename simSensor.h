/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensor.h
 * @version:  
 * @summary:
 *      Declaration file for simEntity Sensor object
 *
 *============================================================================*/


//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_SENSOR_H_
#define _SIM_SENSOR_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include "simEntity.h"
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

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//

class SimSensor : public SimEntity
{
public:
    /**
     * Default Constructor
     * Initialize Sensor with default parameters
     */
    SimSensor(double x, double y, double z,
              double a, double b, double c, stringw p_name);

    /**
     * sets mesh scene node for this entity and add it on scenemanager
     * @param IScenemManager - scene manager with scene nodes
     * @param IMesh - mesh to be translated into IMeshSceneNode
     * @return IMeshSceneNode added to ISceneManager
     */
    virtual void setMeshSceneNode(ISceneManager* smgr, const path &filename);
    
    /**
     * Estimates sensor's new position
     * This is pure virtual function and needs to be overridden!
     * @param None
     * @return None
     */
    virtual void estimate() = 0;
    
private:


};
#endif
