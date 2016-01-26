/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEntity.h
 * @version:  
 * @summary:
 *      Declaration file for simulation objects
 *
 *============================================================================*/

//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ENTITY_H_
#define _SIM_ENTITY_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <irrlicht.h>
#include <vector>

//----------------------------------------------------------------------------//
//                                  Namespaces
//----------------------------------------------------------------------------//
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//----------------------------------------------------------------------------//
//                                Global Variables
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//

class SimEntity
{
public:
    
    /**
     * Default Constructor
     * Initialize Irrlicht, Entities, and physics
     * All degrees of freedom are set to 0
     */
    SimEntity();
    
    /**
     * Constructor with position/rotation and empty scenenode
     * @param double x - x axis coordinate
     * @param double y - y axis coordinate
     * @param double z - z axis coordinate
     * @param double a - a axis coordinate
     * @param double b - b axis coordinate
     * @param double c - c axis coordinate
     * @param stringw p_name - name of entity
     * @return SimEntity object with no mesh to render
     */
    SimEntity(double x, double y, double z,
              double a, double b, double c, stringw p_name);

    /**
     * Constructor with position/rotation and empty scenenode
     * @param double x - x axis coordinate
     * @param double y - y axis coordinate
     * @param double z - z axis coordinate
     * @param double a - a axis coordinate
     * @param double b - b axis coordinate
     * @param double c - c axis coordinate
     * @param stringw p_name - name of entity
     * @param IMesh- mesh to be rendered
     * @return SimEntity object with mesh object
     */
    SimEntity(double x, double y, double z,
              double a, double b, double c, stringw p_name,
              IMesh* m);

    /**
     * Copy Constructor
     * @param SimEntity - SimEntity Object to copy from
     * @return Copied SimEntity object
     */
    SimEntity(const SimEntity & obj);
    
    /**
     * Assignment Operator
     * @param SimEntity - Simentity Object to copy from
     * @return Copied SimEntity object
     */
    SimEntity& operator= (const SimEntity & rhs);

    /**
     * Destructor
     * Initialize Irrlicht, Entities, and physics
     */
    ~SimEntity();

    /**
     * sets position of entity
     * @param double x - x axis coordinate
     * @param double y - y axis coordinate
     * @param double z - z axis coordinate
     * @return none
     */
    void setPosition(double x, double y, double z);

    /**
     * sets rotation of entity
     * @param double a - a axis coordinate
     * @param double b - b axis coordinate
     * @param double c - c axis coordinate
     * @return none
     */
    void setRotation(double a, double b, double c);

    /**
     * gets position of entity
     * @param none
     * @return array size of 3 of double representing positional vector
     */
    const vector3df getPosition();

    /**
     * gets position of entity
     * @param none
     * @return vector size of 3 of double representing rotational matrix 
     */
    const vector3df getRotation();

    /**
     * gets name of this entity
     */
    stringw getName() {return name;};
    
    /**
     * checks sceneNode exists and returns true if so
     * @return bool - true if scnenode exists
     */
    bool drawable() {return mesh != 0;};

private:
    // mesh scene node
    IMeshSceneNode * meshSceneNode;

    // mesh for drawing
    IMesh * mesh;

    // translation coordinates
    vector3df translation;

    // rotation coordinates
    vector3df rotation;

    // name of entity
    stringw name;

};
#endif
