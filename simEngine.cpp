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

SimEngine::SimEngine(bool enableGUI)
{
    if(enableGUI)
    {
        setupGUI();
    }
    else
    {
        simGUI = 0;
    }
    simPhysics = new SimPhysics();
}

void SimEngine::setupGUI()
{
    simGUI = new SimGUI(this,L"TEST WINDOW",840,600,640,480,false);
}

void SimEngine::addEntity(EntityType type, SimEntity * obj)
{
    simEntityVector.push_back(obj);
    if(simGUI)
    {
        simGUI->addEntitySceneNode(type, obj);
    }
}
SimEntity* SimEngine::getEntityByName(std::string name)
{
    std::vector<SimEntity*>::iterator it;

    it = std::find_if(
        simEntityVector.begin(),
        simEntityVector.end(),
        checkEntityName(name)
        );
    if (it != simEntityVector.end())
        return (*it);
    else
        return 0;
}
void SimEngine::removeEntity(SimEntity * obj)
{
    if(!obj)
        return;
    simEntityVector.erase(
        std::remove(simEntityVector.begin(),
                    simEntityVector.end(),
                    obj),
        simEntityVector.end());
    obj->removeCallback();
    if(simGUI)
    {
        simGUI->removeEntitySceneNode(obj);
    }
}

void SimEngine::run()
{
    if(simGUI)
    {
        while(simGUI->isRunning())
        {
            simGUI->draw();
        }
    }
    else
    {

    }
}

//----------------------------------------------------------------------------//
//                                Private functions
//----------------------------------------------------------------------------//
