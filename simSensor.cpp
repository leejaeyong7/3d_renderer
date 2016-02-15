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

}

SimSensor::SimSensor(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     string _meshPath)
    :SimEntity(_name,x,y,z,a,b,c,_meshPath)
{

}

void SimSensor::addAttachedRobot(SimRobot * obj)
{
    robot = obj;
}

void SimSensor::removeAttachedRobot(SimRobot * obj)
{
    robot = 0;
}
void SimSensor::removeCallback()
{
    robot->removeSensor(this);

}
