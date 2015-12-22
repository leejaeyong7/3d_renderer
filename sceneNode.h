/*============================================================================
 * @author: Jae Yong Lee
 * @file: sceneNode.h
 * @version: 1.0 
 * @summary:
 *      cusome scene node header file
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include <irrlicht.h>
//----------------------------------------------------------------------------//
//                               Class Definition
//----------------------------------------------------------------------------//
class WorldSceneNode : public scene::ISceneNode
{
public:

    WorldSceneNode(
        scene::ISceneNode* parent,
        scene:: ISceneManager* mgr,
        s32 id
        ); 
    private:
    core::aabbox3d<f32> Box;
    video::S3DVertex Vertices[4];
    video::SMaterial Material;

}
