/*=============================================================================
 * @author     : Jae Yong Lee
 * @project    : SimEngine
 * @file       : simEnvironment.h
 * @version    : 1.0
 * @description:
 *      Declaration file for simulation environment entity
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
#ifndef _SIM_ENVIRONMENT_H_
#define _SIM_ENVIRONMENT_H_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <algorithm>
#include <vector>
#include "simEntity.h"
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
class SimEnvironment : public SimEntity
{
public:
    /**
     * Default Constructor with mesh path
     * Initialize Environment with default parameters
     */
    SimEnvironment(std::string _name,
                   double x, double y, double z,
                   double a, double b, double c);

    virtual void update() =0;
private:

};
#endif
