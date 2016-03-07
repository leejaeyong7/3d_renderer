/*=============================================================================
 * @author     : Jae Yong Lee
 * @file       : simPlane.h
 * @description: 
 *      Reference file for primitive environment Plane
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_PLANE_H_
#define _SIM_PLANE_H_
//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <algorithm> 
#include <vector> 
#include "simEnvironment.h"
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
class SimPlane: public SimEnvironment
{
public:
    /**
     * Default Constructor
     * Initialize plane with default parameters
     */
    SimPlane(std::string _name,
             double x, double y, double z,
             double a, double b, double c);

    /**
     * Default Constructor
     * Initialize plane with default parameters
     */
    SimPlane(std::string _name,
             double x, double y, double z,
             double a, double b, double c,
             std::string _meshPath);
    
};
#endif
