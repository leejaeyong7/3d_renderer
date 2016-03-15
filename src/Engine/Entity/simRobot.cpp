/*============================================================================
 * @author: Jae Yong Lee
 * @file: simRobot.cpp
 * @version:  
 * @summary:
 *      Definition file for sim robot objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include "simRobot.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                                DEFINITIONS                                 //
//----------------------------------------------------------------------------//

/**
 * Default Constructor
 * Initialize Robot with default parameters
 */
SimRobot::SimRobot(string _name,
                   double x, double y, double z,
                   double a, double b, double c,
                   string _meshPath)
    :SimEntity(_name,x,y,z,a,b,c,_meshPath)
{

}

/**
 * Default Destructor 
 * Removes sensors attached to robot
 */
SimRobot::~SimRobot()
{
    vector<SimSensor*>::iterator it;
    for(it = sensorVector.begin() ; it < sensorVector.end(); ++it)
    {
        removeSensor(*it);
        (*it)->removeAttachedRobot();
    }
}
/**
 * Adds Sensor to robot
 * @param SimSensor* - pointer to sensor object to add
 * @return None
 */
void SimRobot::addSensor(SimSensor* obj)
{
    sensorVector.push_back(obj); 
    obj->addAttachedRobot(this);
}

/**
 * Remove Sensor to robot
 * @param SimSensor* - pointer to sensor object to remove 
 * @return None
 */
void SimRobot::removeSensor(SimSensor* obj)
{
    if(!obj)
        return;
    if(obj->getAttachedRobot()!= this)
        return;
    sensorVector.erase(
        std::remove(sensorVector.begin(),
                    sensorVector.end(),
                    obj),
        sensorVector.end());

    obj->removeAttachedRobot();
}

