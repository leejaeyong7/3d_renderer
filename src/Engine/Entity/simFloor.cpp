/*============================================================================
 * @author: Jae Yong Lee
 * @file: simFloor.cpp
 * @description:
 *     Definition file for floor environment
 *
 *============================================================================*/
#include "simFloor.h"
SimFloor::SimFloor(std::string _name, double y, double w)
    :SimPlane(_name,0,y,0,0,0,0,w,w,0)
{
    
}
