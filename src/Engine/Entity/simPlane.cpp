/*=============================================================================
 * @author     : Jae Yong Lee
 * @file       : simPlane.cpp
 * @description:
 *      Definition file for primitive environment plane
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 ============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include "simPlane.h"

SimPlane::SimPlane(std::string _name,
             double x, double y, double z,
             double a, double b, double c,
             double w, double h, int num_feature)
    :SimEnvironment(_name,x,y,z,a,b,c)
{
  vector<AdvancedOption*>* advVector = getAdvancedOption();
  AdvancedOption_Double * width =
      new AdvancedOption_Double("Width",DOUBLE,w);
  AdvancedOption_Double * height =
      new AdvancedOption_Double("Height",DOUBLE,h);
  AdvancedOption_Int * features =
      new AdvancedOption_Int("Feature #",INTEGER,num_feature);
  advVector->push_back(width);
  advVector->push_back(height);
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
    int num_f = ((AdvancedOption_Int*)(av->at(2)))->value;    
    Point p1,p2,p3,p4;
    Triangle t1, t2;
    Rectangle r1;

    p1.x = -w/2;
    p1.y = h/2;
    p1.z = 0;

    p2.x = w/2;
    p2.y = h/2;
    p2.z = 0;

    p3.x = -w/2;
    p3.y = -h/2;
    p3.z = 0;

    p4.x = w/2;
    p4.y = -h/2;
    p4.z = 0;

    t1.a = p1;
    t1.b = p2;
    t1.c = p3;

    t2.a = p4;
    t2.b = p3;
    t2.c = p2;

    r1.u = t1;
    r1.d = t2;


    rv->push_back(r1);
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
