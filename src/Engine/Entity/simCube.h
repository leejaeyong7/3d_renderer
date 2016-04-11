/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : simCube.h
 * @brief      : Reference file for primitive environment cube
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 =============================================================================*/
#ifndef SIMCUBE_H
#define SIMCUBE_H
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <algorithm>
#include <vector>
#include "simEnvironment.h"
//----------------------------------------------------------------------------//
//                                 NAMESPACES                                 //
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                             CLASS DECLARATION                              //
//----------------------------------------------------------------------------//
class SimCube : public SimEnvironment
{
public:
    SimCube(std::string _name,
            double x, double y, double z,
            double a, double b, double c,
            double w, double h, double d,
            int num_feature);
private:
};


#endif /* SIMCUBE_H */
