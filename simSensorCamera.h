/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensorCamera.h
 * @version:  
 * @summary:
 *      Header file for monocular sensor object in simulation
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_SENSOR_CAM_H_
#define _SIM_SENSOR_CAM_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include "simSensor.h"
//----------------------------------------------------------------------------//
//                                  Namespaces
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                                Global Variables
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//

class SimCamera : public SimSensor
{
public:
    /**
     * Default Constructor
     * Initialized pinhole model camera
     */
    SimCamera(std::string _name,
              double x, double y, double z,
              double a, double b, double c,
              double fx, double fy, double fl);

    /**
     * Default Constructor with mesh parameter
     * Initialized pinhole model camera
     */
    SimCamera(std::string _name,
              double x, double y, double z,
              double a, double b, double c,
              std::string _meshPath,
              double fx, double fy, double fl);

    int foo(){return 50;};

    virtual void estimate(){return ;};

private:
};

#endif
