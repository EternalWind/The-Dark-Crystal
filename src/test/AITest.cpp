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
     
    dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
      dt::ResourceManager::get()->addResourceLocation("Material", "FileSystem");
     dt::ResourceManager::get()->addResourceLocation("models", "FileSystem");
     dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
     Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

     Alien* alien = new Alien("alien", "Sinbad.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    alien->setEyePosition(Ogre::Vector3(0, 10, 10));
    dt::Scene* scene = SceneLoader::loadScene("FirstFloor.scene");
    addScene(scene);
    this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);

    scene->addChildNode(alien);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->setPosition(33, 100, -30);    
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien);
    
    auto motion = alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->getRigidBody()->getMotionState();
    
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(33, 10, -27));

    motion->setWorldTransform(trans);

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
}
