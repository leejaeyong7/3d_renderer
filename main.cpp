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
    SimCamera * cam1 = new SimCamera("test camera1",0,0,0,0,0,0,10,0,0);
    SimRobot* bot = new SimRobot("Robot",10,10,10,0,0,0,"models/android.obj");
    eng->addEntity(ENTITY_TYPE_ROBOT,bot);
    eng->addEntity(ENTITY_TYPE_SENSOR,cam1);
    eng->attachEntity(bot,cam1);
    eng->detachEntity(bot,cam1);
    eng->run();
}
