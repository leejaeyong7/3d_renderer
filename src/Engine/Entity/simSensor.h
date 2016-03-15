/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensor.h
 * @version:  
 * @summary:
 *      Declaration file for simEntity Sensor object
 *
 *============================================================================*/
#ifndef _SIM_SENSOR_H_
#define _SIM_SENSOR_H_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <algorithm>
#include <vector>
#include "simEntity.h"
#include "simRobot.h"
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
class SimRobot;
class SimSensor : public SimEntity
{
public:
    /* default constructor */
    SimSensor(std::string _name,
              double x, double y, double z,
              double a, double b, double c,
              std::string _meshPath="");

    /* default destructor */
    ~SimSensor();

    /* updates callback */
    virtual void update()=0;

    /* attaches robot */
    void addAttachedRobot(SimRobot * obj);

    /* detach from robot */
    void removeAttachedRobot();

    /**
     * Get sensor vector pointer attached to robot
     * @return vector<SimSensor*>* - pointer to vector
     */
    SimRobot* getAttachedRobot(){return robot;}; 

private:
    SimRobot* robot;

};
#endif
