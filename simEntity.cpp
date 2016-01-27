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
}

SimEntity::SimEntity(double x, double y, double z,
                     double a, double b, double c, stringw p_name)
{
    meshSceneNode = 0;

    translation.X = x;
    translation.Y = y;
    translation.Z = z;

    rotation.X = a;
    rotation.Y = b;
    rotation.Z = c;

    name = p_name;
}


SimEntity::SimEntity(const SimEntity & obj)
{
    this->translation = vector3d<f32>(obj.getPosition());
    this->rotation = vector3d<f32>(obj.getRotation());
    this->name = obj.getName();
    this->meshSceneNode = 0;
}

SimEntity& SimEntity::operator= (const SimEntity & rhs)
{
    translation = vector3d<f32>(rhs.getPosition());
    rotation = vector3d<f32>(rhs.getRotation());
    name = rhs.getName();
    meshSceneNode = 0;
    return *this;
}

SimEntity::~SimEntity()
{
    if(meshSceneNode)
        ((ISceneNode*)meshSceneNode)->removeAll();
}

void SimEntity::setPosition(double x, double y, double z)
{
    translation.X = x;
    translation.Y = y;
    translation.Z = z;
}

void SimEntity::setRotation(double a, double b, double c)
{
    rotation.X = a;
    rotation.Y = b;
    rotation.Z = c;
}

void SimEntity::setName(stringw new_name)
{
    name = new_name;
}
