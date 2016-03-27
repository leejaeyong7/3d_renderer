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
                Material.Lighting = false;
                showPath = true;
            };
        
        void addPathNode(vector3df p, vector3df r, int d)
            {
                
            }
        void removePathNode(int index)
            {


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
        list<PathNode> PathList;
        SMaterial Material;
        aabbox3d<f32> box;
        bool showPath;

       
    };
 
    
    
}  // Sim


#endif /* SIMPATHSCENENODE_H */
