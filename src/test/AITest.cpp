#include "AITest.h"
#include "SceneLoader.h"
#include <Core/ResourceManager.hpp>
#include <Scene/Scene.hpp>

#include <Scene/Game.hpp>
#include <Graphics/LightComponent.hpp>

#include <OgreProcedural.h>
#include "Alien.h"
#include "HumanAgent.h"
#include "Monster.h"
#include "PlayerAIAgent.h"
#include "MonsterAIAgent.h"
#include "AIDivideAreaManager.h"
#include "EntityManager.h"
#include "Agent.h"
#include "Character.h"
#include "Weapon.h"



void AITest::onInitialize() {


    dt::ResourceManager::get()->addResourceLocation("Material", "FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem"); 
    

     dt::ResourceManager::get()->addResourceLocation("models/monster.zip", "Zip", true);
     dt::ResourceManager::get()->addResourceLocation("models/alien_mirror.zip", "Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

     addScene(SceneLoader::loadScene("CarTest.scene"));
}
void AITest::onDeinitialize() {

}

void AITest::updateStateFrame(double simulation_frame_time) {
    //static bool flag = false;

    //if (!flag) {
    //    flag = true;
    //    this->getScene("test.scene")->getPhysicsWorld()->setGravity(Ogre::Vector3(0, -9.8, 0));
    //}
}
