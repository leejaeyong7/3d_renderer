/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : simEntitySceneNode.hpp
 * @brief      : Simulator scenenode for entities
 *
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 =============================================================================*/
#ifndef _SIM_ENTITYSCENENODE_HPP_
#define _SIM_ENTITYSCENENODE_HPP_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <irrlicht.h>
#include "simEntity.h"
#include <cmath>
//----------------------------------------------------------------------------//
//                                 NAMESPACE                                  //
//----------------------------------------------------------------------------//
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//----------------------------------------------------------------------------//
//                              CLASS DEFINITION                              //
//----------------------------------------------------------------------------//
namespace Sim{
    class SimSceneNode : public ISceneNode
    {
    public:
        SimSceneNode(ISceneNode * parent, ISceneManager* mgr, s32 id,
                     SimEntity* _obj):
            ISceneNode(parent,mgr,id)
            {
                /* set material */
                Material.Wireframe = false;
                Material.Lighting = true;
                obj = _obj;
                setScale(vector3df(1,1,1));
                setAutomaticCulling(EAC_FRUSTUM_BOX);
                update();
                drawfeature = true;
            }
        virtual void OnRegisterSceneNode()
            {
                if(IsVisible)
                    SceneManager->registerNodeForRendering(this);
                ISceneNode::OnRegisterSceneNode();
            }
        virtual void render()
            {
                IVideoDriver * driver = SceneManager->getVideoDriver();
                driver->setTransform(ETS_WORLD,
                                     AbsoluteTransformation);

                driver->setMaterial(getMaterial(1));
                vector<Rectangle>* rv = obj->getRectangles();
                vector<Triangle>* tv = obj->getTriangles();
                vector<Point>* pv = obj->getPoints();

                vector<Rectangle>::iterator itr;
                vector<Triangle>::iterator itt;
                vector<Point>::iterator itp;
                SColor c(255,200,200,200);

                for(itr = rv->begin(); itr != rv->end(); itr++)
                {
                    driver->draw3DTriangle(convertTriangle(itr->u),c);
                    driver->draw3DTriangle(convertTriangle(itr->d),c);
                    /* driver->draw3DTriangle(convertTriangleInv(itr->u),c); */
                    /* driver->draw3DTriangle(convertTriangleInv(itr->d),c); */
                }
                for(itt = tv->begin(); itt != tv->end(); itt++)
                {
                    driver->draw3DTriangle(convertTriangle(*itt),c);
                    /* driver->draw3DTriangle(convertTriangleInv(*itt),c); */
                }
                for(itp = pv->begin(); itp != pv->end(); itp++)
                {
                    driver->draw3DBox(convertPoint(*itp),c);
                }
                if(drawfeature)
                {
                    vector<Point>* kv = obj->getKeyPoints();
                    vector<Point>::iterator itk;
                    SColor r(255,255,0,0);
                    for(itk = kv->begin(); itk != kv->end(); itk++)
                    {
                        driver->draw3DBox(convertPoint(*itk),r);
                    }
                }
                driver->draw3DBox(box,c);
            }
        void setDrawFeature(bool v)
            {
                drawfeature = v;
            }
        virtual void update()
            {
                setPosition(pos2vec(obj->getPosition()));
                setRotation(rot2vec(obj->getRotation()));
                updateBox();
            }
        virtual const aabbox3d<f32>& getBoundingBox() const
            {
                return box;
            };
        virtual u32 getMaterialCount() const
            {
                return 1;
            }
        virtual SMaterial& getMaterial(u32 i)
            {
                i = i * 1;
                return Material;
            }
        SimEntity* getEntity()
            {
                return obj;
            }
    private:
        triangle3df convertTriangle(Triangle t)
            {
                return triangle3df(point2vec(t.a),
                                   point2vec(t.b),
                                   point2vec(t.c));
            }

        triangle3df convertTriangleInv(Triangle t)
            {
                return triangle3df(point2vec(t.c),
                                   point2vec(t.b),
                                   point2vec(t.a));
            }
        aabbox3d<f32> convertPoint(Point p)
            {
                return aabbox3d<f32>(
                    vector3df(p.x+0.01,p.y+0.01,p.z+0.01),
                    vector3df(p.x-0.01,p.y-0.01,p.z-0.01));
            }
        vector3df pos2vec(Position pos)
            {
                return vector3df(pos.X,pos.Y,pos.Z);
            }
        vector3df rot2vec(Rotation rot)
            {
                return vector3df(rot.Roll,rot.Pitch,rot.Yaw);
            }
        vector3df point2vec(Point p)
            {
                return vector3df(p.x,p.y,p.z);
            }
        void updateBox()
            {
                box.reset(vector3df(0,0,0));
                vector<Rectangle>* rv = obj->getRectangles();
                vector<Triangle>* tv = obj->getTriangles();
                vector<Point>* pv = obj->getPoints();

                vector<Rectangle>::iterator itr;
                vector<Triangle>::iterator itt;
                vector<Point>::iterator itp;
                for(itr = rv->begin(); itr != rv->end(); itr++)
                {
                    Triangle t;
                    t = itr->u;
                    box.addInternalPoint(point2vec(t.a));
                    box.addInternalPoint(point2vec(t.b));
                    box.addInternalPoint(point2vec(t.c));

                    t = itr->d;
                    box.addInternalPoint(point2vec(t.a));
                    box.addInternalPoint(point2vec(t.b));
                    box.addInternalPoint(point2vec(t.c));
                }
                for(itt = tv->begin(); itt != tv->end(); itt++)
                {
                    box.addInternalPoint(point2vec(itt->a));
                    box.addInternalPoint(point2vec(itt->b));
                    box.addInternalPoint(point2vec(itt->c));
                }
                for(itp = pv->begin(); itp != pv->end(); itp++)
                {
                    box.addInternalPoint(point2vec(*itp));
                }
            }
        SMaterial Material;
        aabbox3d<f32> box;
        SimEntity* obj;
        bool drawfeature;
    };
}
#endif
