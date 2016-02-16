/*============================================================================
 * @author: Jae Yong Lee
 * @file: simRobot.cpp
 * @version:  
 * @summary:
 *      Definition file for sim robot objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                               INCLUDES
//----------------------------------------------------------------------------//
#include "simRobot.h"
//----------------------------------------------------------------------------//
//                               DEFINITION
//----------------------------------------------------------------------------//
SimRobot::SimRobot(string _name,
                   double x, double y, double z,
                   double a, double b, double c)
    :SimEntity(_name,x,y,z,a,b,c)
{

}

SimRobot::SimRobot(string _name,
                   double x, double y, double z,
                   double a, double b, double c,
                   string _meshPath)
    :SimEntity(_name,x,y,z,a,b,c,_meshPath)
{

}
void SimRobot::addSensor(SimSensor* obj)
{
    sensorVector.push_back(obj); 
    obj->addAttachedRobot(this);
}

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

void SimRobot::removeCallback()
{
    vector<SimSensor*>::iterator it;
    for(it = sensorVector.begin() ; it < sensorVector.end(); ++it)
    {
        removeSensor(*it);
        (*it)->removeAttachedRobot();
    }
}
