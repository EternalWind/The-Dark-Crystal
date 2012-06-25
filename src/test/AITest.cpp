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

void AITest::onInitialize() {


    dt::ResourceManager::get()->addResourceLocation("Material", "FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem"); 
    

     dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
     dt::ResourceManager::get()->addResourceLocation("models/alien.zip", "Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

   
    
    Alien* alien = new Alien("alien", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    alien->setEyePosition(Ogre::Vector3(0, 10, 10));
    dt::Scene* scene = SceneLoader::loadScene("FirstFloor.scene");
    addScene(scene);
    this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);
    scene->getPhysicsWorld()->setGravity(Ogre::Vector3::ZERO);

    scene->addChildNode(alien);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->setPosition(33, 100, -30);    
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien);

    alien->setPosition(33, 50, -27);
    alien->setScale(0.05);
    /*auto motion = alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->getRigidBody()->getMotionState();

    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(33, 100, -27));

    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->getRigidBody()->setWorldTransform(trans);
    motion->setWorldTransform(trans);*/

    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    /*alien->disable();
    alien->enable();*/
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    //auto scene = addScene(new dt::Scene("1"));

    /*auto mesh = scene->addChildNode(new dt::Node("me"));
    mesh->addComponent(new dt::MeshComponent("rock.01.mesh", "Wall-03", "123"));
    mesh->setPosition(0, 0, -50);
    auto lightnode = scene->addChildNode(new dt::Node("light"));
    lightnode->addComponent(new dt::LightComponent());*/
    /*auto camnode = scene->addChildNode(new dt::Node("cam"));
    camnode->addComponent(new dt::CameraComponent(""))->lookAt(0, 0, -1);*/

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
