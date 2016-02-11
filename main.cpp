/*============================================================================
 * @author: Jae Yong Lee
 * @file: main.cpp
 * @version: 1.0 
 * @summary:
 *      contains main loop for visual odometry simulation engine
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "simEngine.h"
#include <iostream>
#include <algorithm>
#include <vector>
//----------------------------------------------------------------------------//
//                                  Main function
//----------------------------------------------------------------------------//
int main()
{
    SimEngine * eng= new SimEngine(true);
    SimCamera * cam1 = new SimCamera("test camera1",20,20,20,0,0,0,0,0,0);
    SimCamera * cam2 = new SimCamera("test camera2",10,20,20,0,0,0,0,0,0);
    SimCamera * cam3 = new SimCamera("test camera3",00,30,20,0,0,0,0,0,0);
    SimRobot* bot = new SimRobot("Robot",40,40,40,0,0,0);

    eng->addEntity(ENTITY_TYPE_SENSOR,cam1);
    eng->addEntity(ENTITY_TYPE_SENSOR,cam2);
    eng->addEntity(ENTITY_TYPE_SENSOR,cam3);
    eng->addEntity(ENTITY_TYPE_ROBOT,bot);
    SimEntity* camera_object = eng->getEntityByName("test camera3");
    SimEntity* robot_object = eng->getEntityByName("Robot");
    // ((SimRobot*) robot_object)->addSensor((SimSensor*)camera_object);
    // cout<<((SimRobot*)robot_object)->getSensorVector()->size()<<endl;
    // cout<<((SimSensor*)camera_object)->getRobotVector()->size()<<endl;
    // eng->removeEntity(robot_object);
    // eng->removeEntity(camera_object);
    // cout<<((SimRobot*)robot_object)->getSensorVector()->size()<<endl;
    // cout<<((SimSensor*)camera_object)->getRobotVector()->size()<<endl;
    eng->run();
}
