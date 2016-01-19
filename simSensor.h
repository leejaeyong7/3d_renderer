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
    SimSensor();
    
    
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
