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

/**
 * Constructor with position/rotation and empty scenenode
 * @param std::stringw p_name - name of entity
 * @param double x - x axis coordinate
 * @param double y - y axis coordinate
 * @param double z - z axis coordinate
 * @param double a - a axis coordinate
 * @param double b - b axis coordinate
 * @param double c - c axis coordinate
 * @param std::string _meshPath - path of mesh file
 * @return SimEntity object with no mesh to render
 */
SimEntity::SimEntity(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     string _meshPath)
{
    translation.X = x;
    translation.Y = y;
    translation.Z = z;

    rotation.Roll = a;
    rotation.Pitch = b;
    rotation.Yaw = c;

    name = _name;
    meshPath = _meshPath;
}


/**
 * Copy Constructor
 * @param SimEntity - SimEntity Object to copy from
 * @return Copied SimEntity object
 */
SimEntity::SimEntity(const SimEntity & obj)
{
    this->translation = obj.getPosition();
    this->rotation = obj.getRotation();
    this->name = obj.getName();
    this->meshPath = obj.getMeshPath();
}

/**
 * Assignment Operator
 * @param SimEntity - Simentity Object to copy from
 * @return Copied SimEntity object
 */
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
    vector<AdvancedOption*>* av = getAdvancedOption();
    vector<AdvancedOption*>::iterator it;
    for(it = av->begin(); it != av->end(); it++)
    {
        delete (*it);
    }
}

/**
 * sets position of entity
 * @param double x - x axis coordinate
 * @param double y - y axis coordinate
 * @param double z - z axis coordinate
 * @return none
 */
void SimEntity::setPosition(double x, double y, double z)
{
    translation.X = x;
    translation.Y = y;
    translation.Z = z;
}
/**
 * sets rotation of entity
 * @param double a - a axis coordinate
 * @param double b - b axis coordinate
 * @param double c - c axis coordinate
 * @return none
 */
void SimEntity::setRotation(double a, double b, double c)
{
    rotation.Roll = a;
    rotation.Pitch = b;
    rotation.Yaw = c;
}

/**
 * sets name of entity
 * @return none
 */
void SimEntity::setName(string new_name)
{
    name = new_name;
}

/**
 * sets path name of entity
 * @return none
 */
void SimEntity::setMeshPath(string new_path)
{
    meshPath = new_path;
}
