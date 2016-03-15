/*=============================================================================
 * @author     : Jae Yong Lee
 * @project    : SimEngine
 * @file       : simRobotGround.cpp
 * @version    : 
 * @description: 
 *      Definition file for simRobot ground robot
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/

#include "simGroundRobot.h"
SimGroundRobot::SimGroundRobot(std::string _name,
                               double x, double y, double z,
                               double a, double b, double c,
                               double w, double h, double d,
                               string _meshPath)
    :SimRobot(_name,x,y,z,a,b,c,_meshPath)
{
    vector<AdvancedOption*>* av = getAdvancedOption();

    AdvancedOption_Double * width =
        new AdvancedOption_Double("Width",DOUBLE,w);
    AdvancedOption_Double * height =
        new AdvancedOption_Double("Height",DOUBLE,h);
    AdvancedOption_Double * depth =
        new AdvancedOption_Double("Depth",DOUBLE,d);

    av->push_back(width);
    av->push_back(height);
    av->push_back(depth);
    update();
}

void SimGroundRobot::update()
{

    vector<Rectangle>* rv = getRectangles();
    vector<AdvancedOption*>* av = getAdvancedOption();
    double w = ((AdvancedOption_Double*)av->at(0))->value;
    double h = ((AdvancedOption_Double*)av->at(1))->value;
    double d = ((AdvancedOption_Double*)av->at(2))->value;

    rv->clear();

    Point p1,p2,p3,p4,p5,p6,p7,p8;
    Triangle t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12;
    Rectangle r1,r2,r3,r4,r5,r6;

    p1.x = -w/2;
    p1.y = h/2;
    p1.z = -d/2;

    p2.x = w/2;
    p2.y = h/2;
    p2.z = -d/2;

    p3.x = -w/2;
    p3.y = h/2;
    p3.z = d/2;
    
    p4.x = w/2;
    p4.y = h/2;
    p4.z = d/2;

    p5.x = -w/2;
    p5.y = -h/2;
    p5.z = -d/2;

    p6.x = w/2;
    p6.y = -h/2;
    p6.z = -d/2;

    p7.x = -w/2;
    p7.y = -h/2;
    p7.z = d/2;
    
    p8.x = w/2;
    p8.y = -h/2;
    p8.z = d/2;

    t1.a = p1;
    t1.b = p2;
    t1.c = p3;

    t2.a = p4;
    t2.b = p3;
    t2.c = p2;

    t3.a = p1;
    t3.b = p2;
    t3.c = p5;

    t4.a = p6;
    t4.b = p5;
    t4.c = p2;

    t5.a = p2;
    t5.b = p4;
    t5.c = p6;

    t6.a = p8;
    t6.b = p6;
    t6.c = p4;

    t7.a = p1;
    t7.b = p3;
    t7.c = p5;

    t8.a = p7;
    t8.b = p5;
    t8.c = p3;

    t9.a = p3;
    t9.b = p4;
    t9.c = p7;

    t10.a = p8;
    t10.b = p7;
    t10.c = p4;

    t11.a = p5;
    t11.b = p6;
    t11.c = p7;

    t12.a = p8;
    t12.b = p7;
    t12.c = p6;

    r1.u = t1;
    r1.d = t2;
    
    r2.u = t3;
    r2.d = t4;
    
    r3.u = t5;
    r3.d = t6;
    
    r4.u = t7;
    r4.d = t8;
    
    r5.u = t9;
    r5.d = t10;
    
    r6.u = t11;
    r6.d = t12;
    
    rv->push_back(r1);
    rv->push_back(r2);
    rv->push_back(r3);
    rv->push_back(r4);
    rv->push_back(r5);
    rv->push_back(r6);
}
