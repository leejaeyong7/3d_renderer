/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEngine.h
 * @version:  
 * @summary:
 *      Declaration file for simulator engine
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ENGINE_H_
#define _SIM_ENGINE_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include "simGUI.h"
#include "simPhysics.h"
#include "simEntity.h"
#include "simRobot.h"
#include "simSensor.h"
#include "enumerations.h"

//----------------------------------------------------------------------------//
//                                  Namespaces
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                                Global Variables
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
class SimGUI;
class SimEngine
{
public:

    /**
     * Constructor
     * Initialize Irrlicht, Entities, and physics
     * @param bool - if true, setup GUI
     */
    SimEngine(bool enableGUI);

    /**
     * sets up simGUI
     * @param None
     * @return None
     */
    void setupGUI();

    /**
     * Runs Simulation with physics, environment, and 3D rendering
     */
    void run();

    /**
     * fetch SimGUI object pointer
     */
    SimGUI * getSimGUI(){return simGUI;};

    /**
     * fetch pointer to entity vector
     * @return pointer to vector of enitty pointers
     */
    vector<SimEntity*> * getEntityVector(){return &simEntityVector;};
    
    /**
     * fetch pointer to entity searched by name
     * @param string - name
     * @return SimEntity * - pointer to entity object
     */
    SimEntity* getEntityByName(std::string name);

    /**
     * adds entity to rendering
     * @param EntityType - type of entity object to add
     * @param SimEntity - entity object to add
     * @see SimEntity
     * @return None
     */
    void addEntity(EntityType type, SimEntity * obj);

    /**
     * removes entity object from rendering
     * if entity object doesn't exist, it will do nothing
     * @param SimEntity - pointer to entity object to remove
     * @return None
     */
    void removeEntity(SimEntity * obj);

    /**
     * Attaches Sensor to Camera
     * @param SimRobot* robot - robot to attach sensor on
     * @param SimSensor* sensor- sensor to attach 
     * @return none
     */
    void attachEntity(SimRobot * robot, SimSensor * sensor);

    /**
     * removes Sensor from Camera
     * @param SimRobot* robot - robot to detach sensor from  
     * @param SimSensor* sensor- sensor to detach 
     * @return none
     */
    void detachEntity(SimRobot * robot, SimSensor * sensor);

private:
    // Pointer to SimGUI object that handles irrlicht GUI design
    SimGUI * simGUI;

    // Pointer to SimPhysics object that handles all the physics works in
    // simulation engine
    SimPhysics * simPhysics;

    // vector of SimEntity object
    vector<SimEntity*> simEntityVector;

    //functor for find_if predicate
    struct checkEntityName 
    {
        checkEntityName(const std::string name) : name_holder(name) {}
        std::string name_holder;
        bool operator()(SimEntity* obj)
        {
            return obj->getName().compare(name_holder) == 0;
        }
    };
};
#endif
