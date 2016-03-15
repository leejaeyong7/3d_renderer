/*============================================================================
 * @author: Jae Yong Lee
 * @file: main.cpp
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

int main( int argc, char** argv)
{
    SimEngine * eng= new SimEngine(true);
    SimCamera * cam1 = new SimCamera("test camera1",0,0,0,0,0,0,
                                     (60.0f/180.0f*3.141592f),
                                     (45.0f/180.0f*3.141592f),
                                     1);
    SimGroundRobot* bot = new SimGroundRobot("Robot",10,10,10,0,0,0,1,1,1);
    SimPlane* plane = new SimPlane("test Plane",-10,10,-10,0,0,0,10,10,100);
    eng->addEntity(ENTITY_TYPE_ROBOT,bot);
    eng->addEntity(ENTITY_TYPE_SENSOR,cam1);
    eng->addEntity(ENTITY_TYPE_ENVIRONMENT,plane);
    eng->attachEntity(bot,cam1);
    eng->run();
}
