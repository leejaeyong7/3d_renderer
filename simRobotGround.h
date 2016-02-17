/*=============================================================================
 * @author     : Jae Yong Lee
 * @project    : 
 * @file       : simRobotGround.h
 * @version    : 
 * @description: 
 *      Declaration file for simRobot Ground Robot
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ROBOT_GROUND_H_
#define _SIM_ROBOT_GROUND_H_
//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <algorithm> 
#include <vector> 
#include "simEntity.h"
#include "simRobot.h"
//----------------------------------------------------------------------------//
//                                  Namespaces
//----------------------------------------------------------------------------//
using namespace std;

//----------------------------------------------------------------------------//
//                                Global Variables
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimGroundRobot: public SimRobot
{
public:
    /**
     * Default Constructor
     * Initialize Sensor with default parameters
     */
    SimGroundRobot(std::string _name,
             double x, double y, double z,
             double a, double b, double c);

    /**
     * Default Constructor
     * Initialize Sensor with default parameters
     */
    SimGroundRobot(std::string _name,
             double x, double y, double z,
             double a, double b, double c,
             std::string _meshPath);
};
#endif
