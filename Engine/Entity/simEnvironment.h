/*=============================================================================
 * @author     : Jae Yong Lee
 * @project    : SimEngine
 * @file       : simEnvironment.h
 * @version    : 1.0
 * @description: 
 *      Declaration file for simulation environment entity
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ENVIRONMENT_H_
#define _SIM_ENVIRONMENT_H_
//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <algorithm>
#include <vector>
#include "simEntity.h"
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
class SimEnvironment : public SimEntity
{
    public:
        /**
         * Default Constructor
         * Initialize Environment with default parameters
         */
        SimEnvironment(std::string _name,
                double x, double y, double z,
                double a, double b, double c);

        /**
         * Default Constructor with mesh path
         * Initialize Environment with default parameters
         */
        SimEnvironment(std::string _name,
                double x, double y, double z,
                double a, double b, double c,
                std::string _meshPath);

    private:

};
#endif
