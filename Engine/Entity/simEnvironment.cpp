/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEnvironment.cpp
 * @version:  
 * @summary:
 *      Definition file for sim environment objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include "simEnvironment.h"

//----------------------------------------------------------------------------//
//                               Class Implementation 
//----------------------------------------------------------------------------//
SimEnvironment::SimEnvironment(std::string _name,
        double x, double y, double z,
        double a, double b, double c)
    :SimEntity(_name,x,y,z,a,b,c)
{
}

SimEnvironment::SimEnvironment(std::string _name,
        double x, double y, double z,
        double a, double b, double c,
        std::string _meshPath)
    :SimEntity(_name,x,y,z,a,b,c,_meshPath)
{

}

