/*============================================================================
 * @author: Jae Yong Lee
 * @file: simRobot.h
 * @version:  
 * @summary:
 *      Declaration file for simEntity Robot object
 *
 *============================================================================*/
#ifndef _SIM_ROBOT_H_
#define _SIM_ROBOT_H_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <algorithm> 
#include <vector> 
#include "simEntity.h"
#include "simSensor.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                                 NAMESPACES                                 //
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                               END NAMESPACES                               //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//
/* forward declaration */
class SimSensor;
class SimRobot: public SimEntity
{
public:
    /* default constructor */
    SimRobot(std::string _name,
             double x, double y, double z,
             double a, double b, double c,
             std::string _meshPath);

    /* default destructor */
    ~SimRobot();

    /* update callback */
    virtual void update()= 0;

    /* adds sensor to robot */
    void addSensor(SimSensor* obj);
    /* removes sensor to robot */
    void removeSensor(SimSensor* obj);

    /**
     * Get sensor vector pointer attached to robot
     * @return vector<SimSensor*>* - pointer to vector
     */
    vector<SimSensor*>* getSensorVector(){return &sensorVector;}; 

private:
    vector<SimSensor*> sensorVector;


};
#endif
