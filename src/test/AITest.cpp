#include "AITest.h"
#include "SceneLoader.h"
#include <Core/ResourceManager.hpp>
#include <Scene/Scene.hpp>

#include <Scene/Game.hpp>
#include <Graphics/LightComponent.hpp>


#include "Alien.h"
#include "HumanAgent.h"
#include "Monster.h"
#include "PlayerAIAgent.h"
#include "MonsterAIAgent.h"
#include "AIDivideAreaManager.h"


void AITest::onInitialize() {


    dt::ResourceManager::get()->addResourceLocation("Material", "FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem"); 
    

     dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
     dt::ResourceManager::get()->addResourceLocation("models/alien.zip", "Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    AIDivideAreaManager::get()->loadMapInfo("map.txt");
    
   
    dt::Scene* scene = SceneLoader::loadScene("FirstFloor.scene");
    addScene(scene);
    this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);
    scene->getPhysicsWorld()->setGravity(Ogre::Vector3::ZERO);

    
    Alien* alien = new Alien("alien", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");    
     scene->addChildNode(alien);
    alien->setEyePosition(Ogre::Vector3(0, 3, 3));
    HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien);
    alien->setPosition(33, 10, -27);
    alien->setScale(0.01);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

   

    //auto cam = scene->addChildNode(
    
    Alien* alien1 = new Alien("alien1", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    scene->addChildNode(alien1);
    PlayerAIAgent * pa = new PlayerAIAgent("rj");
    pa->attachTo(alien1);
    alien1->setPosition(0, 10, -27);
    alien1->setScale(0.01);    
    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
    
}
void AITest::onDeinitialize() {

}

void AITest::updateStateFrame(double simulation_frame_time) {
    static bool flag = false;

    if (!flag) {
        flag = true;
        this->getScene("FirstFloor.scene")->getPhysicsWorld()->setGravity(Ogre::Vector3(0, -9.8, 0));
    }
}
