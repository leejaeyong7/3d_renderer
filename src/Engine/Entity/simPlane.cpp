/*=============================================================================
 * @author     : Jae Yong Lee
 * @file       : simPlane.cpp
 * @description:
 *      Definition file for primitive environment plane
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include "simPlane.h"

SimPlane::SimPlane(std::string _name,
             double x, double y, double z,
             double a, double b, double c)
    :SimEnvironment(_name,x,y,z,a,b,c)
{
    
}


SimPlane::SimPlane(std::string _name,
             double x, double y, double z,
             double a, double b, double c,
             std::string _meshPath)
    :SimEnvironment(_name,x,y,z,a,b,c,_meshPath)
{

}
