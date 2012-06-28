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

void AITest::onInitialize() {

    dt::ResourceManager::get()->addResourceLocation("", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("a", "FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("sinbad.zip", "Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    Alien* alien = new Alien("alien", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    alien->setEyePosition(Ogre::Vector3(0, 10, 10));
    dt::Scene* scene = SceneLoader::loadScene("FirstFloor.scene");
    addScene(scene);
    this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);
    scene->getPhysicsWorld()->setGravity(Ogre::Vector3::ZERO);

    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::PlaneGenerator().setSizeX(10000).setSizeY(10000).setUTile(100).setVTile(100).realizeMesh("plane");

    auto plane_node = scene->addChildNode(new dt::Node("plane_node"));
    plane_node->setPosition(0, -10, 0);
    plane_node->addComponent<dt::MeshComponent>(new dt::MeshComponent("plane", "PrimitivesTest/Pebbles", "Plane"));
    plane_node->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent("Plane", "plane_body", dt::PhysicsBodyComponent::BOX, 0.0f));

    scene->addChildNode(alien);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->setPosition(33, 100, -30);    
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien);

    alien->setPosition(33, 50, -27);
    alien->setScale(0.05);

    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    /*Weapon* weapon = new Weapon("RailGun", Weapon::PRIMARY, 1, 1, 1, 0, 1, 1, true, 1, "", "", "", 1);
    scene->addChildNode(weapon);

    weapon->setPosition(0, 100, 0);
    weapon->setScale(0.05);

    weapon->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    weapon->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();*/

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
