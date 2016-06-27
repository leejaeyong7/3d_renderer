/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : simPathSceneNode.hpp
 * @brief      : Simulator scene node for path simulation
 * 
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 =============================================================================*/
#ifndef SIMPATHSCENENODE_H
#define SIMPATHSCENENODE_H
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <irrlicht.h>
#include <list>
#include <iostream>
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
//                            TYPEDEF DECLARATION                             //
//----------------------------------------------------------------------------//
typedef struct PathNode{
    vector3df Pos;
    vector3df Rot;
    int dur;
} PathNode;
//----------------------------------------------------------------------------//
//                              CLASS DEFINITION                              //
//----------------------------------------------------------------------------//
namespace Sim {
    class PathSceneNode : public ISceneNode
    {
    public:
        PathSceneNode(ISceneNode * parent, ISceneManager * mgr, s32 id):
            ISceneNode(parent,mgr,id)
            {
                Material.Wireframe = false;
                Material.Lighting = true;
                showPath = true;
                box.reset(vector3df(0,0,0));
            };
        void editPathNode(int index, vector3df p, vector3df r, int d)
            {
                if(PathList.size() >= index)
                {
                    PathNode n;
                    n.Pos = p;
                    n.Rot = r;
                    n.dur = d;
                    std::list<PathNode>::iterator it = PathList.begin();
                    std::advance(it,index);
                    PathList.insert(it, n);
                }
            }
        void addPathNode(vector3df p, vector3df r, int d)
            {
                PathNode n;
                n.Pos = p;
                n.Rot = r;
                n.dur = d;
                PathList.push_back(n);
            }
        void removePathNode(int index)
            {
                if(PathList.size() >= index)
                {
                    std::list<PathNode>::iterator it = PathList.begin();
                    std::advance(it,index);
                    PathList.erase(it);
                }
            }
        std::list<PathNode>* getPathList()
            {
                return &PathList;
            }
        
        virtual void render()
            {
                IVideoDriver * driver = SceneManager->getVideoDriver();
                driver->setTransform(ETS_WORLD,
                                     AbsoluteTransformation);
                driver->setMaterial(getMaterial(1));
                std::list<PathNode>::iterator it;
                PathNode p;
                PathNode n;
                vector3df pv;
                vector3df nv;
                int count = 0;
                int max = PathList.size();
                SColor lineColor(255,0,255,0);
                if(max > 1)
                {
                    for(int i = 0; i < max*120; i++)
                    {
                        if(i == 0)
                        {
                            nv = bezier(0);
                        }
                        else
                        {
                            pv = nv;
                            nv = bezier((double)i/((double)max*120));
                            driver->draw3DLine(pv, nv, lineColor);
                        }
                    }
                }
                for(it = PathList.begin(); it != PathList.end() ; it++)
                {
                    if(count == 0)
                    {
                        n = (*it);
                        draw3DVector(driver,n.Pos,n.Rot,lineColor);
                    }
                    else
                    {
                        p = n;
                        n = (*it);
                        vector3df curp = p.Pos;
                        vector3df curr = p.Rot;
                        /* driver->draw3DLine(curp, n.Pos, lineColor); */
                        draw3DVector(driver,n.Pos,n.Rot,lineColor);
                        driver->draw3DBox(box,SColor(255,255,0,0));
                    }
                    count++;
                }
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

        virtual void OnRegisterSceneNode()
            {
                if(IsVisible)
                    SceneManager->registerNodeForRendering(this);
                ISceneNode::OnRegisterSceneNode();
            }
    private:
        void draw3DVector(IVideoDriver *drv,
                          vector3df pos,
                          vector3df rot, SColor c)
            {
                vector3df tr1(0,0,-0.3);
                vector3df tr2(0.1,0,0.3);
                vector3df tr3(-0.1,0,0.3);

                tr1.rotateYZBy(rot.X,vector3df(0,0,0));
                tr1.rotateXZBy(rot.Y,vector3df(0,0,0));
                tr1.rotateXYBy(rot.Z,vector3df(0,0,0));

                tr2.rotateYZBy(rot.X,vector3df(0,0,0));
                tr2.rotateXZBy(rot.Y,vector3df(0,0,0));
                tr2.rotateXYBy(rot.Z,vector3df(0,0,0));

                tr3.rotateYZBy(rot.X,vector3df(0,0,0));
                tr3.rotateXZBy(rot.Y,vector3df(0,0,0));
                tr3.rotateXYBy(rot.Z,vector3df(0,0,0));
                
                tr1 = tr1 + pos;
                tr2 = tr2 + pos;
                tr3 = tr3 + pos;
                drv->draw3DTriangle(triangle3df(tr1,tr2,tr3),c);
                drv->draw3DTriangle(triangle3df(tr3,tr2,tr1),c);
            }
        aabbox3d<f32> vec2box(vector3df p)
            {
                return aabbox3d<f32>(
                    vector3df(p.X+0.1,p.Y+0.1,p.Z+0.1),
                    vector3df(p.X-0.1,p.Y-0.1,p.Z-0.1));
            }
        vector3df bezier(double t)
            {
                // t must be between 0 and 1
                std::list<PathNode>::iterator it;
                int count = 0;
                
                vector3df sum = vector3df(0,0,0);

                int max = PathList.size();
                for(it = PathList.begin(); it != PathList.end() ; it++)
                {
                    sum += (it->Pos)*bernstein(count,max-1,t);
                    count++;
                }
                return sum;
            }
        double bernstein(int i, int n, double t)
            {
                return binomial(n,i)*powerOf(t,i)*powerOf((1-t),n-i);
            }
        int binomial(int n, int i)
            {
                return factorial(n) / (factorial(i)*factorial(n-i));
            }
        int factorial(int n)
            {
                return n ? n*factorial(n-1) : 1;
            }
        double powerOf(double t, int p)
            {
                return p ? t*powerOf(t,p-1) : 1;
            }
        std::list<PathNode> PathList;
        SMaterial Material;
        aabbox3d<f32> box;
        bool showPath;
    };
}  // Sim
#endif /* SIMPATHSCENENODE_H */
