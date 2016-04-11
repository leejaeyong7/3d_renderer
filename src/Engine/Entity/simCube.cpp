/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : simCube.cpp
 * @brief      : Definition file for primitive environment cube
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 =============================================================================*/
#include "simCube.h"

SimCube::SimCube(std::string _name,
                 double x, double y, double z,
                 double a, double b, double c,
                 double w, double h, double d,
                 int num_feature)
    :SimEnvironment(_name,x,y,z,a,b,c)
{
  vector<AdvancedOption*>* advVector = getAdvancedOption();
  AdvancedOption_Double * width =
      new AdvancedOption_Double("Width",DOUBLE,w);
  AdvancedOption_Double * height =
      new AdvancedOption_Double("Height",DOUBLE,h);
  AdvancedOption_Double * depth=
      new AdvancedOption_Double("Depth",DOUBLE,d);
  AdvancedOption_Int * features =
      new AdvancedOption_Int("Feature #",INTEGER,num_feature);
  advVector->push_back(width);
  advVector->push_back(height);
  advVector->push_back(depth);
  advVector->push_back(features);
  update();
}


void SimPlane::update()
{
    vector<Point>* kv = getKeyPoints();
    vector<Rectangle>* rv = getRectangles();
    kv->clear();
    rv->clear();
    vector<AdvancedOption*>* av = getAdvancedOption();
    double w = ((AdvancedOption_Double*)(av->at(0)))->value;
    double h = ((AdvancedOption_Double*)(av->at(1)))->value;
    double d = ((AdvancedOption_Double*)(av->at(2)))->value;
    int num_f = ((AdvancedOption_Int*)(av->at(3)))->value;    

    Point p1,p2,p3,p4,p5,p6,p7,p8;
    Triangle t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12;
    Rectangle r1,r2,r3,r4,r5,r6;

    //up 4
    // left back
    p1.x = -w/2;
    p1.y = h/2;
    p1.z = -d/2;

    // left front
    p2.x = -w/2;
    p2.y = h/2;
    p2.z = d/2;

    // right back
    p3.x = w/2;
    p3.y = h/2;
    p3.z = -d/2;

    // right front
    p4.x = w/2;
    p4.y = h/2;
    p4.z = d/2;

    // down 4
    // left back
    p5.x = -w/2;
    p5.y = -h/2;
    p5.z = -d/2;

    // left front
    p6.x = -w/2;
    p6.y = -h/2;
    p6.z = d/2;

    // right back
    p7.x = w/2;
    p7.y = -h/2;
    p7.z = -d/2;

    // right front
    p8.x = w/2;
    p8.y = -h/2;
    p8.z = d/2;

    // up : 1,2,3,4
    // up triangles
    t1.a = p1;
    t1.b = p2;
    t1.c = p3;

    t2.a = p4;
    t2.b = p3;
    t2.c = p2;

    // back : 1,3,5,7
    // back triangles
    t3.a = p1;
    t3.b = p3;
    t3.c = p5;

    t4.a = p7;
    t4.b = p5;
    t4.c = p3;

    // down : 5,6,7,8
    // down triangles
    t5.a = p5;
    t5.b = p6;
    t5.c = p7;

    t6.a = p8;
    t6.b = p7;
    t6.c = p6;

    // front : 2,4,6,8
    // front triangles
    t7.a = p2;
    t7.b = p4;
    t7.c = p6;

    t8.a = p8;
    t8.b = p6;
    t8.c = p4;

    // right : 3,4,7,8
    // right triangles
    t9.a = p3;
    t9.b = p4;
    t9.c = p7;

    t10.a = p8;
    t10.b = p7;
    t10.c = p4;

    // left : 1,2,5,6
    // left triangles
    t11.a = p1;
    t11.b = p2;
    t11.c = p5;

    t12.a = p6;
    t12.b = p5;
    t12.c = p2;

    // up rectangle
    r1.u = t1;
    r1.d = t2;
    
    // back rectangle
    r2.u = t3;
    r2.d = t4;
    
    // back rectangle
    r3.u = t5;
    r3.d = t6;

    // front rectangle
    r4.u = t7;
    r4.d = t8;

    // right rectangle
    r5.u = t9;
    r5.d = t10;

    // left rectangle
    r6.u = t11;
    r6.d = t12;

    rv->push_back(r1);
    rv->push_back(r2);
    rv->push_back(r3);
    rv->push_back(r4);
    rv->push_back(r5);
    rv->push_back(r6);

    vector<Point>* keyPoints = getKeyPoints();
    keyPoints->clear();

    if(num_f != 0)
    {
        int sn = round(sqrt(num_f));
        int ho = (num_f % sn == 0) ? 0 : 1;

        for(int i = 0; i < num_f ; i++)
        {
            Point k;
            k.x = ((i % sn)+0.5 - ((double)sn/2))*w/sn;
            k.y = ((i / sn)+0.5 -((double)((num_f/sn)+ho)/2))*h/
                ((double)(num_f/sn)+ho);
            k.z = 0;
            keyPoints->push_back(k);
        }
    }
}
