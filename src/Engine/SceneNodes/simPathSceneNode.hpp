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
                int count = 0;
                int max = PathList.size();
                SColor lineColor(255,0,255,0);
                for(it = PathList.begin(); it != PathList.end() ; it++)
                {
                    if(count == 0)
                    {
                        n = (*it);
                        /* driver->draw3DBox(vec2box(n.Pos),lineColor); */
                        draw3DVector(driver,n.Pos,n.Rot,lineColor);
                    }
                    else
                    {
                        p = n;
                        n = (*it);
                        vector3df curp = p.Pos;
                        vector3df curr = p.Rot;
                        /* vector3df prev = p.Pos; */
                        /* vector3df diffr = n.Rot - curr; */

                        /* // connect path */
                        /* for(int i = 0; i < 100; i++) */
                        /* { */
                        /*     prev = curp; */
                        /*     vector3df diff = n.Pos - curp; */
                        /*     // advance path */
                        /*     curr = curr.normalize() + diff.normalize()*0.1;  */
                        /*     curp = curp + curr.normalize()*0.1; */
                        /*     driver->draw3DLine(prev, curp, lineColor); */
                        /* } */
                        driver->draw3DLine(curp, n.Pos, lineColor);
                        /* driver->draw3DBox(vec2box(n.Pos),lineColor); */
                        draw3DVector(driver,n.Pos,n.Rot,lineColor);
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
        std::list<PathNode> PathList;
        SMaterial Material;
        aabbox3d<f32> box;
        bool showPath;
    };
}  // Sim
#endif /* SIMPATHSCENENODE_H */
