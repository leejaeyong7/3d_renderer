/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensorCamera.h
 * @version:  
 * @summary:
 *      Header file for monocular sensor object in simulation
 *
 *============================================================================*/
#ifndef _SIM_SENSOR_CAM_H_
#define _SIM_SENSOR_CAM_H_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include "simSensor.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                                 NAMESPACES                                 //
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                               END NAMESPACES                               //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimCamera : public SimSensor
{
public:
    /* default constructor */
    SimCamera(std::string _name,
              double x, double y, double z,
              double a, double b, double c,
              double fx, double fy, double fl,
              std::string _meshPath="");

    /* update callback */
    virtual void update();

private:
};

#endif