/*============================================================================
 * @author: Jae Yong Lee
 * @file: simSensorCamera.cpp
 * @version:  
 * @summary:
 *      Definition file for monocular sensor object
 *
 *============================================================================*/
#include "simCamera.h"

/**
 * Default Constructor with mesh parameter
 * Initialized pinhole model camera
 */
SimCamera::SimCamera(string _name,
                     double x, double y, double z,
                     double a, double b, double c,
                     double fx, double fy, double fl,
                     string _meshPath):
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

    update();
}


void SimCamera::update()
{
    vector<AdvancedOption*>* av = getAdvancedOption();
    double fov_x = ((AdvancedOption_Double*)av->at(0))->value;
    double fov_y = ((AdvancedOption_Double*)av->at(1))->value;
    double fl = ((AdvancedOption_Double*)av->at(2))->value;
    vector<Triangle>* tv = getTriangles();
    vector<Rectangle>* rv = getRectangles();
    tv->clear();
    rv->clear();
    Point r1,r2,r3,r4,f;
    Triangle t1,t2,t3,t4,tr1,tr2;
    Rectangle rect1;

    r1.x = -tan(fov_x/2)*fl;
    r1.y = tan(fov_y/2)*fl;
    r1.z = fl;

    r2.x = tan(fov_x/2)*fl;
    r2.y = tan(fov_y/2)*fl;
    r2.z = fl;
    
    r3.x = -tan(fov_x/2)*fl;
    r3.y = -tan(fov_y/2)*fl;
    r3.z = fl;
    
    r4.x = tan(fov_x/2)*fl;
    r4.y = -tan(fov_y/2)*fl;
    r4.z = fl;

    f.x = 0;
    f.y = 0;
    f.z = 0;

    t1.a = r1;
    t1.b = r2;
    t1.c = f;

    t2.a = r1;
    t2.b = r3;
    t2.c = f;

    t3.a = r2;
    t3.b = r4;
    t3.c = f;

    t4.a = r3;
    t4.b = r4;
    t4.c = f;
    
    /* tr1.a = r1; */
    /* tr1.b = r2; */
    /* tr1.c = r3; */
    
    /* tr2.a = r4; */
    /* tr2.b = r3; */
    /* tr2.c = r2; */

    /* rect1.u = tr1; */
    /* rect1.d = tr2; */

    tv->push_back(t1);
    tv->push_back(t2);
    tv->push_back(t3);
    tv->push_back(t4);

    t1.a = r2;
    t1.b = r1;
    t1.c = f;

    t2.a = r3;
    t2.b = r1;
    t2.c = f;

    t3.a = r4;
    t3.b = r2;
    t3.c = f;

    t4.a = r4;
    t4.b = r3;
    t4.c = f;

    tv->push_back(t1);
    tv->push_back(t2);
    tv->push_back(t3);
    tv->push_back(t4);

    /* rv->push_back(rect1); */
}
