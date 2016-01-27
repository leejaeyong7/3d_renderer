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
     * sets rotation of entity
     * @param double a - a axis coordinate
     * @param double b - b axis coordinate
     * @param double c - c axis coordinate
     * @return none
     */
    void setName(stringw new_name);

    /**
     * gets position of entity
     * @param none
     * @return array size of 3 of double representing positional vector
     */
    const vector3df getPosition() const {return translation;};

    /**
     * gets position of entity
     * @param none
     * @return vector size of 3 of double representing rotational matrix 
     */
    const vector3df getRotation() const {return rotation;};

    /**
     * gets name of this entity
     */
    const stringw getName() const {return name;};
    
    /**
     * sets mesh scenenode for entity(pure virtual function
     * @param ISceneManager - scene manager to hold mesh scenenode
     * @param path - path to .obj file
     * @see 
     * @return Imeshscenenode attached to entity
     */
    virtual void setMeshSceneNode(ISceneManager* smgr, const path &filename) =0;


    /**
     * sets mesh scenenode through pointer(for inherited class access)
     * @param IMeshSceneNode* - pointer to mesh scenenode to set
     * @return none
     */
    void setMeshSceneNode(IMeshSceneNode * mSN){meshSceneNode = mSN;};

    /**
     * gets mesh scenenode pointer for entity
     * @return IMeshScenenode * - pointer of mesh scenenode
     */
    IMeshSceneNode * getMeshSceneNode() {return meshSceneNode;};

private:
    // mesh scene node
    IMeshSceneNode * meshSceneNode;

    // translation coordinates
    vector3df translation;

    // rotation coordinates
    vector3df rotation;

    // name of entity
    stringw name;

};
#endif
