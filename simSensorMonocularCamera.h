/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensorMonocularCamera.h
 * @version:  
 * @summary:
 *      Header file for monocular sensor object in simulation
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_SENSOR_MONO_CAM_H_
#define _SIM_SENSOR_MONO_CAM_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include "simSensor.h"
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

class SimMonocularCamera : public SimSensor
{
public:
    /**
     * Default Constructor
     * Initialized monocular camera
     */
    SimMonocularCamera(double x, double y, double z,
                       double a, double b, double c, stringw p_name);

    virtual void estimate(){return ;};

private:

};

#endif
