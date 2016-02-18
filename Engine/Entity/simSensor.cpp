/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensor.cpp
 * @version:  
 * @summary:
 *      Definition file for sim sensor objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                               INCLUDES
//----------------------------------------------------------------------------//
#include "simSensor.h"
//----------------------------------------------------------------------------//
//                               DEFINITION
//----------------------------------------------------------------------------//
SimSensor::SimSensor(string _name,
                     double x, double y, double z,
                     double a, double b, double c)
    :SimEntity(_name,x,y,z,a,b,c)
{
    robot = 0;
}

SimSensor::SimSensor(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     string _meshPath)
    :SimEntity(_name,x,y,z,a,b,c,_meshPath)
{
    robot = 0;
}

void SimSensor::addAttachedRobot(SimRobot * obj)
{
    robot = obj;
}

void SimSensor::removeAttachedRobot()
{
    robot = 0;
}
void SimSensor::removeCallback()
{
    if(robot)
        robot->removeSensor(this);
}
