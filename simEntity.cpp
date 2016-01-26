/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEntity.cpp
 * @version:  
 * @summary:
 *      Definition file for entity object
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "simEntity.h"
//----------------------------------------------------------------------------//
//                               Class Definitions
//----------------------------------------------------------------------------//
SimEntity::SimEntity()
{
    meshSceneNode = 0;
    mesh = 0;
}

SimEntity::SimEntity(double x, double y, double z,
                     double a, double b, double c, stringw p_name)
{
    meshSceneNode = 0;
    mesh = 0;
    translation.X = x;
    translation.Y = y;
    translation.Z = z;

    rotation.X = a;
    rotation.Y = b;
    rotation.Z = c;

    name = p_name;
}
