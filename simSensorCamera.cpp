/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensorCamera.cpp
 * @version:  
 * @summary:
 *      Definition file for monocular sensor object
 *
 *============================================================================*/
#include "simSensorCamera.h"

SimCamera::SimCamera(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     double fx, double fy, double fl):
    SimSensor(_name,x,y,z,a,b,c,"Models/camera.obj")
{
    vector<AdvancedOption*>* advVector = getAdvancedOption();
    AdvancedOption_Double * fov_x =
        new AdvancedOption_Double("FOV X",DOUBLE,fx);
    AdvancedOption_Double * fov_y =
        new AdvancedOption_Double("FOV Y",DOUBLE,fy);
    AdvancedOption_Double * focal_length =
        new AdvancedOption_Double("Focal Length",DOUBLE,fl);
    advVector->push_back(fov_x);
    advVector->push_back(fov_y);
    advVector->push_back(focal_length);
}

SimCamera::SimCamera(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     string _meshPath,
                     double fx, double fy, double fl):
    SimSensor(_name,x,y,z,a,b,c,_meshPath)
{
    vector<AdvancedOption*>* advVector = getAdvancedOption();
    AdvancedOption_Double * fov_x =
        new AdvancedOption_Double("FOV X",DOUBLE,fx);
    AdvancedOption_Double * fov_y =
        new AdvancedOption_Double("FOV Y",DOUBLE,fy);
    AdvancedOption_Double * focal_length =
        new AdvancedOption_Double("Focal Length",DOUBLE,fl);
    advVector->push_back(fov_x);
    advVector->push_back(fov_y);
    advVector->push_back(focal_length);
}
