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
     */
    SimEntity();

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
     * @return array size of 3 of double as x,y,z respectively
     */
    const vector<double> getPosition();

    /**
     * gets position of entity
     * @param none
     * @return array size of 3 of double as x,y,z respectively
     */
    const vector<double> getRotation();
    
    /**
     * Draws entity on 3D rendering screen at given position/rotation
     * This is pure virtual function : must be overridden!
     * @param none
     * @return drawn scene node
     */
    virtual void Draw() = 0;

private:
    // Scene node for drawing
    ISceneNode * sceneNode;

    // translation coordinates
    vector<double> translation;

    // rotation coordinates
    vector<double> rotation;
};
#endif
