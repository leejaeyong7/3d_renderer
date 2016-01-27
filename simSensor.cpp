/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensor.cpp
 * @version:  
 * @summary:
 *      Definition file for sim sensor objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                               INCLUDES
//----------------------------------------------------------------------------//
#include "simSensor.h"
//----------------------------------------------------------------------------//
//                               DEFINITION
//----------------------------------------------------------------------------//
SimSensor::SimSensor(double x, double y, double z,
                     double a, double b, double c, stringw p_name)
    :SimEntity(x,y,z,a,b,c,p_name)
{

}


void SimSensor::setMeshSceneNode(ISceneManager* smgr, const path &filename)
{
    IAnimatedMesh* mesh = smgr->getMesh(filename);
    SimEntity::setMeshSceneNode(smgr->addMeshSceneNode(
                         // mesh
                         mesh->getMesh(0),
                         //parent scene node
                         0,
                         // id
                         -1,
                         // position
                         getPosition(),
                         // rotation
                         getRotation()
                         // scale by default
                         )
        );

}
