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
#include "EntityManager.h"


void AITest::onInitialize() {


    dt::ResourceManager::get()->addResourceLocation("Material", "FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem"); 
    

     dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
     dt::ResourceManager::get()->addResourceLocation("models/alien.zip", "Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    //AIDivideAreaManager::get()->loadMapInfo("map.txt");
    
    AIDivideAreaManager::get()->beforeLoadScene(34.0, 8.0);   
    dt::Scene* scene = SceneLoader::loadScene("FirstFloor.scene");
    AIDivideAreaManager::get()->afterLoadScene();   

    addScene(scene);
    this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);
    scene->getPhysicsWorld()->setGravity(Ogre::Vector3::ZERO);

    
    Alien* alien = new Alien("alien", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");    
     scene->addChildNode(alien);
    alien->setEyePosition(Ogre::Vector3(0, 3, 3));
    HumanAgent* human_agent = new HumanAgent("Player");
    
    alien->setPosition(-50, 10, -100);
    alien->setScale(0.01);
    human_agent->attachTo(alien);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    EntityManager::get()->setHuman(alien);

    //auto cam = scene->addChildNode(
    
    Alien* alien1 = new Alien("alien1", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    scene->addChildNode(alien1);

    
    alien1->setPosition(0, 10, -100);
    alien1->setScale(0.005);    
    

    PlayerAIAgent * pa = new PlayerAIAgent("rj");
    pa->attachTo(alien1);

    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
    
    EntityManager::get()->addPlayer(alien1);


     Alien* alien2 = new Alien("alien2", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    scene->addChildNode(alien2);
    
    alien2->setPosition(0, 10, -90);
    alien2->setScale(0.005);    
    
    
    PlayerAIAgent * pa2 = new PlayerAIAgent("rj2");
    pa2->attachTo(alien2);

    alien2->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien2->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
    
    EntityManager::get()->addPlayer(alien2);
   /* alien->setCurSpeed(4.0);
    alien1->setCurSpeed(4.0);*/
     Ogre::Degree kk; 
            Ogre::Vector3 kkk;
            alien1->getRotation().ToAngleAxis(kk, kkk);
            std::cout << kk << ' ' << kkk.x << ' ' << kkk.y << ' ' << kkk.z << endl; 
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
