#include "AITest.h"
#include "SceneLoader.h"
#include <Core/ResourceManager.hpp>
#include <Scene/Scene.hpp>

#include "Alien.h"
#include "HumanAgent.h"
#include "Monster.h"
#include "PlayerAIAgent.h"
#include "MonsterAIAgent.h"

void AITest::onInitialize() {
     
     dt::ResourceManager::get()->addResourceLocation("Material", "FileSystem");
     dt::ResourceManager::get()->addResourceLocation("Mesh", "FileSystem");
     dt::ResourceManager::get()->addResourceLocation("Mesh/sinbad.zip", "Zip", true);
     Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

     Alien* alien = new Alien("alien", "Sinbad.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    alien->setEyePosition(Ogre::Vector3(0, 3, -3));
    auto scene = addScene(SceneLoader::loadScene("FirstFloor.scene"));
    scene->addChildNode(alien);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    alien->setPosition(30, 100, -30);    
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien);
     
}
void AITest::onDeinitialize() {

}

void AITest::updateStateFrame(double simulation_frame_time) {
}