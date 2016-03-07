/*============================================================================
 * @author: Jae Yong Lee
 * @file: simFloor.h
 * @description:
 *     Declarartion file for floor environment
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//-------------------------------INCLUDE GUARDS-------------------------------//
//----------------------------------------------------------------------------//
#ifndef _SIM_FLOOR_H
#define _SIM_FLOOR_H 
//----------------------------------------------------------------------------//
//----------------------------------INCLUDES----------------------------------//
//----------------------------------------------------------------------------//
#include "simEnvironment.h"
class SimFloor : public SimEnvironment
{
    public:
        /**
         * Default constructor; floor only needs height
         */
        SimFloor(std::string _name, double y, std::string _meshPath);


    private:
};
#endif /* ifndef _SIM_FLOOR_H */
