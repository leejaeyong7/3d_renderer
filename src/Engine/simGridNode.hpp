/*============================================================================
 * @author: Jae Yong Lee
 * @file: simGridNode.hpp
 * @description:
 *     Grid node for simulator
 *
 *============================================================================*/
#include <irrlicht.h>
#include <iostream>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
class GridNode: public ISceneNode
{
    public:
        GridNode(ISceneNode * parent, ISceneManager* mgr, s32 id,
                s32 _x, s32 _z, s32 _step=1):
            ISceneNode(parent,mgr,id)
        {
            Material.Wireframe = true;
            Material.Lighting = false;
            x = _x;
            z = _z;
            step = _step;
            vectorArrayX = new vector3df[(2*x+1)*2];
            vectorArrayZ = new vector3df[(2*z+1)*2];
            for(int i = 0; i <= x; i++)
            {
                vectorArrayX[i] = vector3df(i*step,0,z);
                vectorArrayX[(2*x+1)+i] = vector3df(i*step,0,-z);
                vectorArrayX[((4*x+2)-i)%(4*x+2)] = vector3df(-i*step,0,z);
                vectorArrayX[(2*x+1)-i] = vector3df(-i*step,0,-z);
                box.addInternalPoint(vectorArrayX[i]);
                box.addInternalPoint(vectorArrayX[(2*x+1)+i]);
                box.addInternalPoint(vectorArrayX[((4*x+2)-i)%(4*x+2)]);
                box.addInternalPoint(vectorArrayX[(2*x+1)-i]);
            }
            for(int i = 0; i <= z; i++)
            {
                vectorArrayZ[i] = vector3df(x,0,i*step);
                vectorArrayZ[(2*z+1)+i] = vector3df(-x,0,i*step);
                vectorArrayZ[((4*z+2)-i)%(4*z+2)] = vector3df(x,0,-i*step);
                vectorArrayZ[(2*z+1)-i] = vector3df(-x,0,-i*step);
                box.addInternalPoint(vectorArrayZ[i]);
                box.addInternalPoint(vectorArrayZ[(2*z+1)+i]);
                box.addInternalPoint(vectorArrayZ[((4*z+2)-i)%(4*z+2)]);
                box.addInternalPoint(vectorArrayZ[(2*z+1)-i]);
            }
        }
        ~GridNode()
        {
            delete [] vectorArrayX;
            delete [] vectorArrayZ;
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
            driver->setMaterial(Material);
            driver->setTransform(ETS_WORLD,
                    AbsoluteTransformation);
            for(int i = 0; i <=x; i++)
            {
                driver->draw3DLine(
                        vectorArrayX[i],
                        vectorArrayX[(2*x+1)+i],
                        SColor(255,255,255,255));
                driver->draw3DLine(
                        vectorArrayX[((4*x+2)-i)%(4*x+2)],
                        vectorArrayX[(2*x+1)-i],
                        SColor(255,255,255,255));
            }
            for(int i = 0; i <= z; i ++)
            {
                driver->draw3DLine(
                        vectorArrayZ[i],
                        vectorArrayZ[(2*z+1)+i],
                        SColor(255,255,255,255));
                driver->draw3DLine(
                        vectorArrayZ[((4*z+2)-i)%(4*z+2)],
                        vectorArrayZ[(2*z+1)-i],
                        SColor(255,255,255,255));
            }
        }
        virtual const aabbox3d<f32>& getBoundingBox() const
        {
            return box;
        }
        virtual u32 getMaterialCount() const
        {
            return 1;
        }
        virtual SMaterial& getMaterial(u32 i)
        {
            i = i * 1;
            return Material;
        }
    private:
        SMaterial Material;
        aabbox3d<f32> box;
        s32 x;
        s32 z;
        s32 step;
        vector3df * vectorArrayX;
        vector3df * vectorArrayZ;
};

