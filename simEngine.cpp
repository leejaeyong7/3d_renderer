/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEngine.cpp
 * @version:  
 * @summary:
 *      Definition file for simulation engine
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                  Includes
//----------------------------------------------------------------------------//
#include "simEngine.h"
//----------------------------------------------------------------------------//
//                               Class Definitions
//----------------------------------------------------------------------------//

SimEngine::SimEngine(const wchar_t * text,
                     u32 m_width, u32 m_height,
                     u32 m_width_r, u32 m_height_r,
                     bool fullscreen)
{
    device = createDevice(
        // device type : opengl 
        // using video namespace
        // is able to choose between EDT_OPENGL, EDT_DIRECT3D8/9, etc
        EDT_OPENGL,

        // window size
        // dimension2d from core namespace
        // using macro values for window size
        dimension2d<u32>(m_width,m_height),

        // bits per pixel : 16 (default 16)
        // if not specified, set to 16
        16,

        // full screen mode : false (default false)
        false,

        // stencil buffer(??) : false (default false)
        false,

        // vsync : false (default false)
        false,

        // custom event handler
        0
        );
    width = m_width;
    height = m_height;
    width_r = m_width_r;
    height_r = m_height_r;

    // set window title
    device->setWindowCaption(text);
}
void SimEngine::setupGUI()
{
    simGUI = new SimGUI(this);
}
void SimEngine::setupRenderingCamera()
{
    ISceneManager * smgr = device->getSceneManager();
    SMaterial * floor_material = new SMaterial();
    if(floor_material)
    {
        // floor will be affected by ambient light
        floor_material->AmbientColor  = SColor(255, 110, 110, 110);
        floor_material->DiffuseColor  = SColor(255, 0, 0, 0);
        floor_material->SpecularColor = SColor(255, 0, 0, 0);
        // and emit gray-ish color
        floor_material->EmissiveColor = SColor(255, 110, 110, 110);
        floor_material->ColorMaterial = ECM_NONE;
    }
    // add floor for FPS like rendering
    // for basic testing only
    IAnimatedMesh * movingplane= smgr->addHillPlaneMesh(
        // mesh name
        "floor", 
        // tileSize
        dimension2d<f32>(15, 15),
        // tilecount
        dimension2d<u32>(40, 40),
        // material
        floor_material
        );
    IAnimatedMeshSceneNode *floor=smgr->addAnimatedMeshSceneNode(movingplane);

    // add cube scene node for testing
    ISceneNode * cube_node = smgr->addCubeSceneNode();
    if(cube_node)
    {
        cube_node->setPosition(vector3df(40,15,1));
        cube_node->setRotation(vector3df(10,0,0));

        cube_node->getMaterial(0).AmbientColor  = SColor(255, 255, 255, 255);
        cube_node->getMaterial(0).DiffuseColor  = SColor(255, 0, 255, 255);
        cube_node->getMaterial(0).ColorMaterial = ECM_DIFFUSE;

    }

    // set global lighting (weak gray)
    smgr->setAmbientLight(SColorf(0.1f,0.1f,0.1f,0.1f));

    // add sun light lighting (diffusive light)
    ILightSceneNode* sun_light = smgr->addLightSceneNode(
        //parent node
        0, 
        //scenemanager
        vector3df(0,10000,0), 
        // sun color
        SColorf(0.906f,0.882f,0.488f),
        // sun position
        15000.0f,

        -1 );
    // add sun sphere for view
    ISceneNode * sun_node = smgr->addSphereSceneNode();
    if(sun_node)
    {
        // put sun up high
        sun_node->setPosition(vector3df(0,10000,0));
        // scale sun large
        sun_node->setScale(vector3df(100,100,100));
        // sun is not affected by any lighting
        sun_node->setMaterialFlag(EMF_LIGHTING,false);
    }


    SKeyMap keyMap[8];

    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    ICameraSceneNode* rc = smgr->addCameraSceneNodeFPS(
        // parent node : default null
        0, 
        // rotation speed : default 100
        30,
        // movement speed : default 0.5
        0.03, 
        // id
        -1,
        // key mapping input
        keyMap,
        // key mapping size
        8
        );

    // set camera position
    rc->setPosition(vector3df(0,5,0));
    // sets far value of camera
    // extended for sun support
    rc->setFarValue(20000.0f);
    // hides mouse
    device->getCursorControl()->setVisible(false);

}
void SimEngine::addEntity(SimEntity * obj)
{
    simEntityVector.push_back(obj);
}

void SimEngine::run()
{
    IVideoDriver * driver = device->getVideoDriver();
    ISceneManager * smgr = device->getSceneManager();
    setupRenderingCamera();
    while(device->run())
    {
        if(device->isWindowActive())
        {
            driver->beginScene(true,true,SColor(255,200,200,200));
            driver->setViewPort(rect<s32>(0,20,width_r,height_r));
            smgr->drawAll();
            driver->setViewPort(rect<s32>(0,0,width,height));
            simGUI->draw();
            driver->endScene();
        }
        else
            device->yield();
    }
    device->drop();
}
