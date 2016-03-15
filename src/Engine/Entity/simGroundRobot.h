/*=============================================================================
 * @author     : Jae Yong Lee
 * @project    : 
 * @file       : simRobotGround.h
 * @version    : 
 * @description: 
 *      Declaration file for simRobot Ground Robot
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
#ifndef _SIM_GROUND_ROBOT_H_
#define _SIM_GROUND_ROBOT_H_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <algorithm> 
#include <vector> 
#include "simEntity.h"
#include "simRobot.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                                 NAMESPACES                                 //
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                               END NAMESPACES                               //
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
                   double a, double b, double c,
                   double w, double h, double d,
                   std::string _meshPath="");

    /* default destructor */
    ~SimGroundRobot();
    virtual void update();
};
#endif
