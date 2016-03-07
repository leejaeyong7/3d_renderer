/*=============================================================================
 * @author     : Jae Yong Lee
 * @project    : SimEngine
 * @file       : simRobotGround.cpp
 * @version    : 
 * @description: 
 *      Definition file for simRobot ground robot
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
//----------------------------------------------------------------------------//
//                               INCLUDES
//----------------------------------------------------------------------------//
#include "simGroundRobot.h"
SimGroundRobot::SimGroundRobot(std::string _name,
                               double x, double y, double z,
                               double a, double b, double c)
    :SimRobot(_name,x,y,z,a,b,c,"data/models/android.obj")
{

}
SimGroundRobot::SimGroundRobot(std::string _name,
                               double x, double y, double z,
                               double a, double b, double c,
                               string _meshPath)
    :SimRobot(_name,x,y,z,a,b,c,_meshPath)
{

}
