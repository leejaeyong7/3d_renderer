/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensor.cpp
 * @version:  
 * @summary:
 *      Definition file for sim sensor objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include "simSensor.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                                 DEFINITION                                 //
//----------------------------------------------------------------------------//
/**
 * Default Constructor with mesh path
 * Initialize Sensor with default parameters
 */
SimSensor::SimSensor(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     string _meshPath)
    :SimEntity(_name,x,y,z,a,b,c,_meshPath)
{
    robot = 0;
}

/**
 * Destructor when removing entity is called
 * All robots that has this sensor will remove this sensor
 */
SimSensor::~SimSensor()
{
    if(robot)
        robot->removeSensor(this);
}

/**
 * adds a robot entity that added this sensor
 * @param SimRobot* - robot that attached this sensor
 * @return None
 */
void SimSensor::addAttachedRobot(SimRobot * obj)
{
    robot = obj;
}

/**
 * removes robot entity that added this sensor entity
 * @param None
 * @return None
 */
void SimSensor::removeAttachedRobot()
{
    robot = 0;
}

/* void SimSensor::removeCallback() */
/* { */
/*     if(robot) */
/*         robot->removeSensor(this); */
/* } */
