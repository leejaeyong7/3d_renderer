/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensorMonocularCamera.cpp
 * @version:  
 * @summary:
 *      Definition file for monocular sensor object
 *
 *============================================================================*/
#include "simSensorMonocularCamera.h"

SimMonocularCamera::SimMonocularCamera(double x, double y, double z,
                                       double a, double b, double c,
                                       stringw p_name):
    SimSensor(x,y,z,a,b,c,p_name)
{

}
