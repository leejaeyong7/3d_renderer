/*============================================================================
 * @author: Jae Yong Lee
 * @file: simRobot.h
 * @version:  
 * @summary:
 *      Declaration file for simEntity Robot object
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ROBOT_H_
#define _SIM_ROBOT_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <algorithm> 
#include <vector> 
#include "simEntity.h"
#include "simSensor.h"

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

class SimSensor;
class SimRobot: public SimEntity
{
    public:
        /**
         * Default Constructor
         * Initialize Sensor with default parameters
         */
        SimRobot(std::string _name,
                double x, double y, double z,
                double a, double b, double c);

        /**
         * Default Constructor
         * Initialize Sensor with default parameters
         */
        SimRobot(std::string _name,
                double x, double y, double z,
                double a, double b, double c,
                std::string _meshPath);

        /**
         * Callback function when remove entity is called
         * All sensors will be removed from this robot
         */
        virtual void removeCallback();

        /**
         * Adds Sensor to robot
         * @param SimSensor* - pointer to sensor object to add
         * @return None
         */
        void addSensor(SimSensor* obj);

        /**
         * Remove Sensor to robot
         * @param SimSensor* - pointer to sensor object to remove 
         * @return None
         */
        void removeSensor(SimSensor* obj);

        /**
         * Get sensor vector pointer attached to robot
         * @return vector<SimSensor*>* - pointer to vector
         */
        vector<SimSensor*>* getSensorVector(){return &sensorVector;}; 
        /**
         * Estimates sensor's new position
         * This is pure virtual function and needs to be overridden!
         * @param None
         * @return None
         */
        //virtual void estimate() = 0;

    private:
        vector<SimSensor*> sensorVector;


};
#endif
