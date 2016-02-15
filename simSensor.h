/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensor.h
 * @version:  
 * @summary:
 *      Declaration file for simEntity Sensor object
 *
 *============================================================================*/


//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_SENSOR_H_
#define _SIM_SENSOR_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <algorithm>
#include <vector>
#include "simEntity.h"
#include "simRobot.h"

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

class SimRobot;
class SimSensor : public SimEntity
{
public:
    /**
     * Default Constructor
     * Initialize Sensor with default parameters
     */
    SimSensor(std::string _name,
              double x, double y, double z,
              double a, double b, double c);

    /**
     * Default Constructor with mesh path
     * Initialize Sensor with default parameters
     */
    SimSensor(std::string _name,
              double x, double y, double z,
              double a, double b, double c,
              std::string _meshPath);

    /**
     * Callback function when removing entity is called
     * All robots that has this sensor will remove this sensor
     */
    virtual void removeCallback();

    /**
     * adds a robot entity that added this sensor entity to vector 
     * @param SimRobot* - robot that attached this sensor
     * @return None
     */
    void addAttachedRobot(SimRobot * obj);


    /**
     * removes robot entity that added this sensor entity to vector
     * @param SimRobot* - robot that removed this sensor
     * @return None
     */
    void removeAttachedRobot(SimRobot * obj);
    
    /**
     * Get sensor vector pointer attached to robot
     * @return vector<SimSensor*>* - pointer to vector
     */
    SimRobot* getAttachedRobot(){return robot;}; 
    
    /**
     * Estimates sensor's new position
     * This is pure virtual function and needs to be overridden!
     * @param None
     * @return None
     */
    virtual void estimate() = 0;
    
private:
    SimRobot* robot;

};
#endif
