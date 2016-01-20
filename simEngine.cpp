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

    // checks device is valid
    if(!device)
        return -1;

    // set window title
    device->setWindowCaption(text);

    // initialize GUI
    simGUI = new simGUI(this);
}

void SimEngine::run()
{
    IVideoDriver * driver = device->getVideoDriver();
    ISceneManager * smgr = device->getSceneManager();
    while(device->run())
    {
        if(device->isWindowActive())
        {
            driver->setViewPort(rect<s32>(0,0,width_r,height_r))
            driver->beginScene(true,true,SColor(255,179,234,252));
            smgr->drawAll();
            driver->endScene();

            driver->setViewPort(rect<s32>(0,0,width,height));
            driver->beginScene(true,true,SColor(255,179,234,252));
            simGUI->draw();
            driver->endScene();
        }
        else
            device->yield();
    }
    device->drop();
}
