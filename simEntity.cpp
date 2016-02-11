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

SimEntity::SimEntity(string _name,
                     double x, double y, double z,
                     double a, double b, double c)
{
    translation.X = x;
    translation.Y = y;
    translation.Z = z;

    rotation.Pitch = a;
    rotation.Roll = b;
    rotation.Yaw = c;

    name = _name;
    meshPath = "";
}

SimEntity::SimEntity(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     string _meshPath)
{
    translation.X = x;
    translation.Y = y;
    translation.Z = z;

    rotation.Pitch = a;
    rotation.Roll = b;
    rotation.Yaw = c;

    name = _name;
    meshPath = _meshPath;
}


SimEntity::SimEntity(const SimEntity & obj)
{
    this->translation = obj.getPosition();
    this->rotation = obj.getRotation();
    this->name = obj.getName();
    this->meshPath = obj.getMeshPath();
}

SimEntity& SimEntity::operator= (const SimEntity & rhs)
{
    translation = rhs.getPosition();
    rotation = rhs.getRotation();
    name = rhs.getName();
    meshPath = rhs.getMeshPath();
    return *this;
}

SimEntity::~SimEntity()
{
    // remove all dynamically allocated data
    // currently none
}

void SimEntity::setPosition(double x, double y, double z)
{
    translation.X = x;
    translation.Y = y;
    translation.Z = z;
}

void SimEntity::setRotation(double a, double b, double c)
{
    rotation.Pitch = a;
    rotation.Roll = b;
    rotation.Yaw = c;
}

void SimEntity::setName(string new_name)
{
    name = new_name;
}

void SimEntity::setMeshPath(string new_path)
{
    meshPath = new_path;
}
