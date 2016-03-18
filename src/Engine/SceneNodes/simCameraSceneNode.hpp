/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : simCameraSceneNode.hpp
 * @brief      : Scenenode for camera 
 *
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 =============================================================================*/
#ifndef _SIM_CAMERA_SCENENODE_HPP_
#define _SIM_CAMERA_SCENENODE_HPP_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <irrlicht.h>
#include "simCamera.h"
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
    class CameraSceneNode: public ICameraSceneNode 
    {
    public:
        CameraSceneNode(ISceneNode * parent, ISceneManager* mgr, s32 id):
            ICameraSceneNode(parent,mgr,id)
            {
                obj = 0;
                UpVector = vector3df(0,1,0);
                Target = vector3df(0,0,0);
                InputReceiverEnabled = false;
                TargetAndRotationAreBound = false;
                
                ZNear = 0.001f;
                ZFar = 2000.0f;
            }
        SimCamera* getCamera()
            {
                return obj;
            }
        void attachCamera(SimCamera * _obj)
            {
                obj = _obj;
                if(obj)
                    update();
              
            }

        void detachCamera()
            {
                obj = 0;
            }

        void update()
            {
                if(!obj)
                    return;
                SimRobot* r = 0;
                vector<AdvancedOption*>* av = obj->getAdvancedOption();
                double fov_x = ((AdvancedOption_Double*)av->at(0))->value;
                double fov_y = ((AdvancedOption_Double*)av->at(1))->value;
                double fl = ((AdvancedOption_Double*)av->at(2))->value;

                vector3df rp;
                vector3df rr;

                vector3df cp = pos2vec(obj->getPosition());
                vector3df cr = rot2vec(obj->getRotation());

                vector3df ocp = cp;
                vector3df ocr = cr;
                if((r = obj->getAttachedRobot()))
                {
                    rp = pos2vec(r->getPosition());
                    rr = rot2vec(r->getRotation());
                }
                else
                {
                    rp = vector3df(0,0,0);
                    rr = vector3df(0,0,0);
                }
                vector3df tv(0,0,fl);
                vector3df uv(0,1,0);
                vector3df fv(0,0,0.00001);

                tv.rotateYZBy(cr.X,vector3df(0,0,0));
                tv.rotateXZBy(-cr.Y,vector3df(0,0,0));
                tv.rotateXYBy(cr.Z,vector3df(0,0,0));

                uv.rotateYZBy(cr.X,vector3df(0,0,0));
                uv.rotateXZBy(-cr.Y,vector3df(0,0,0));
                uv.rotateXYBy(cr.Z,vector3df(0,0,0));

                fv.rotateYZBy(cr.X,vector3df(0,0,0));
                fv.rotateXZBy(-cr.Y,vector3df(0,0,0));
                fv.rotateXYBy(cr.Z,vector3df(0,0,0));

                tv = tv + cp;
                uv = uv + cp;
                fv = fv + cp;

                cr.rotateYZBy(rr.X,vector3df(0,0,0));
                cr.rotateXZBy(-rr.Y,vector3df(0,0,0));
                cr.rotateXYBy(rr.Z,vector3df(0,0,0));

                cp.rotateYZBy(rr.X,vector3df(0,0,0));
                cp.rotateXZBy(-rr.Y,vector3df(0,0,0));
                cp.rotateXYBy(rr.Z,vector3df(0,0,0));

                tv.rotateYZBy(rr.X,vector3df(0,0,0));
                tv.rotateXZBy(-rr.Y,vector3df(0,0,0));
                tv.rotateXYBy(rr.Z,vector3df(0,0,0));

                uv.rotateYZBy(rr.X,vector3df(0,0,0));
                uv.rotateXZBy(-rr.Y,vector3df(0,0,0));
                uv.rotateXYBy(rr.Z,vector3df(0,0,0));

                fv.rotateYZBy(rr.X,vector3df(0,0,0));
                fv.rotateXZBy(-rr.Y,vector3df(0,0,0));
                fv.rotateXYBy(rr.Z,vector3df(0,0,0));

                tv = tv + rp;
                uv = uv + rp;
                fv = fv + rp;
                cp = cp + rp;


                ISceneNode::setRotation(cr);
                ISceneNode::setPosition(cp);
                Target = tv;
                UpVector = uv - ocp - rp;

                Aspect = (tan(fov_x/2.0f)/tan(fov_y/2.0f));
                fovy = fov_y;

                recalculateProjectionMatrix();
                recalculateviewport();
            }
        virtual void setRotation(const vector3df& rotation)
            {
                ISceneNode::setRotation(rotation);
            }
        virtual void setPosition(const vector3df& position)
            {
                ISceneNode::setPosition(position);
            }
//----------------------------------------------------------------------------//
//                             TO BE IMPLEMENTED                              //
//----------------------------------------------------------------------------//
        virtual void render()
            {
                vector3df pos = getAbsolutePosition();
                vector3df tgtv = Target - pos;
                tgtv.normalize();

                // if upvector and vector to the target are the same, we have a
                // problem. so solve this problem:
                vector3df up = UpVector;
                up.normalize();

                f32 dp = tgtv.dotProduct(up);

                if ( equals(abs_<f32>(dp), 1.f) )
                {
                    up.X += 0.5f;
                }

                viewport.getTransform(ETS_VIEW)
                    .buildCameraLookAtMatrixLH(pos, Target, up);
                viewport.getTransform(ETS_VIEW) *= Affector;
                recalculateviewport();

                IVideoDriver* driver = SceneManager->getVideoDriver();
                if ( driver)
                {
                    driver->setTransform(ETS_PROJECTION,
                                         viewport.getTransform(ETS_PROJECTION));
                    driver->setTransform(ETS_VIEW,
                                         viewport.getTransform( ETS_VIEW) );
                }
            }

        virtual const aabbox3d<f32>& getBoundingBox() const
            {
                return viewport.getBoundingBox();
            }
        /* virtual void setRotation(const core::vector3df& rotation) =0; */
        virtual bool OnEvent(const SEvent& event)
            {
                return true;
            }
        virtual void setProjectionMatrix(const matrix4& projection,
                                         bool isOrthogonal=false)
            {
                IsOrthogonal = isOrthogonal;
                viewport.getTransform (ETS_PROJECTION) = projection;
            }
        virtual const matrix4& getProjectionMatrix() const
            {
                return viewport.getTransform ( ETS_PROJECTION );
            }
        virtual const matrix4& getViewMatrix() const
            {
                return viewport.getTransform ( video::ETS_VIEW );
            }
        virtual void setViewMatrixAffector(const matrix4& affector)
            {
                Affector = affector;
            }
        virtual const matrix4& getViewMatrixAffector() const
            {
                return Affector;
            }
        virtual void setTarget(const vector3df& pos)
            {
                Target = pos;
                if(TargetAndRotationAreBound)
                {
                    const vector3df toTarget = Target - getAbsolutePosition();
                    ISceneNode::setRotation(toTarget.getHorizontalAngle());
                }
                return;
            }
        virtual const vector3df& getTarget() const
            {
                return Target;
            }
        virtual void setUpVector(const vector3df& pos)
            {
                UpVector = pos;
            }
        virtual const vector3df& getUpVector() const
            {
                return UpVector;
            }
        virtual f32 getNearValue() const
            {
                return ZNear;
            }
        virtual f32 getFarValue() const
            {
                return ZFar;
            }
        virtual f32 getAspectRatio() const
            {
                return Aspect;
            }
        virtual f32 getFOV() const
            {
                return fovy;
            }
        virtual void setNearValue(f32 zn)
            {
                ZNear = zn;
                recalculateProjectionMatrix();
            }
        virtual void setFarValue(f32 zf)
            {
                ZFar = zf;
                recalculateProjectionMatrix();
            }
        virtual void setAspectRatio(f32 aspect)
            {
                Aspect = aspect;
                recalculateProjectionMatrix();
            }
        virtual void setFOV(f32 _fovy)
            {
                fovy = _fovy;
                recalculateProjectionMatrix();
            }
        virtual const SViewFrustum* getViewFrustum() const
            {
                return &viewport;
            }
        virtual void setInputReceiverEnabled(bool enabled)
            {
                InputReceiverEnabled = enabled;
            }
        virtual bool isInputReceiverEnabled() const
            {
                return InputReceiverEnabled;
            }
        virtual void bindTargetAndRotation(bool bound)
            {
                TargetAndRotationAreBound = bound;
            }
        virtual bool getTargetAndRotationBinding(void) const
            {
                return TargetAndRotationAreBound;
            }
    private:
        vector3df pos2vec(Position pos)
            {
                return vector3df(pos.X,pos.Y,pos.Z);
            }
        vector3df rot2vec(Rotation rot)
            {
                return vector3df(rot.Roll,rot.Pitch,rot.Yaw);
            }

        void recalculateProjectionMatrix()
            {
                viewport.getTransform (ETS_PROJECTION)
                    .buildProjectionMatrixPerspectiveFovLH(fovy,
                                                           Aspect,
                                                           ZNear,
                                                           ZFar);
            }
        

        void recalculateviewport()
            {
                viewport.cameraPosition = getAbsolutePosition();

                matrix4 m(matrix4::EM4CONST_NOTHING);
                m.setbyproduct_nocheck(viewport.getTransform(ETS_PROJECTION),
                                       viewport.getTransform(ETS_VIEW));
                viewport.setFrom(m);
            }
        SimCamera * obj;

        SViewFrustum viewport;

        vector3df Target;
        vector3df UpVector;
        
        matrix4 Affector;

        f32 fovy;
        f32 Aspect;
        f32 ZNear;
        f32 ZFar;

        bool InputReceiverEnabled;
        bool TargetAndRotationAreBound;

    };
}
#endif
