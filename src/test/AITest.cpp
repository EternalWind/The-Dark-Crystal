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

    //AIDivideAreaManager::get()->loadMapInfo("map.txt");
    
    AIDivideAreaManager::get()->beforeLoadScene(30.0, 8.0);   
    dt::Scene* scene = SceneLoader::loadScene("test.scene");
    //AIDivideAreaManager::get()->afterLoadScene();   

    addScene(scene);
    this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);
    scene->getPhysicsWorld()->setGravity(Ogre::Vector3::ZERO);

    
    //Alien* alien = new Alien("alien", "alien_mirror.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");    
    // scene->addChildNode(alien);
    // Weapon * weapon = new Weapon("RailGun", Weapon::PRIMARY, 3, 60000, 60000, 1, 60000, 60000, 0, 1.0, 0, "", "", "", 300);
    // alien->addWeapon(weapon);
    // 
    //alien->setEyePosition(Ogre::Vector3(0, 1.5, 1.5));
    //HumanAgent* human_agent = new HumanAgent("Player");    
    //  Ogre::Vector3 t = AIDivideAreaManager::get()->getArea(40);
    //  weapon->setPosition(t.x + 3, 4, t.z + 3);
    //setP(alien, human_agent, t.x, 10, t.z);    
    //alien->setScale(0.01);
    //EntityManager::get()->setHuman(alien);

   
  /*  
    Alien* alien1 = new Alien("alien1", "alien_mirror.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    scene->addChildNode(alien1);
    PlayerAIAgent * pa = new PlayerAIAgent("rj");    
    setP(alien1, pa, 0, 10, -100);   
    EntityManager::get()->addPlayer(alien1);*/


    /* Alien* alien2 = new Alien("alien2", "alien_mirror.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    scene->addChildNode(alien2);
    PlayerAIAgent * pa2 = new PlayerAIAgent("rj2");
    Ogre::Vector3 t = AIDivideAreaManager::get()->getArea(60);
    setP(alien2, pa2, t.x, 10, t.z);    
    EntityManager::get()->addPlayer(alien2);
*/

    //Monster * monster = new Monster("monster", "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 3, 3);
    //scene->addChildNode(monster);
    //MonsterAIAgent * ma = new MonsterAIAgent("ma");
    //setP(monster, ma, 52, 10, 95);
    //monster->setCurSpeed(6.0);
    //monster->setScale(0.015);
    //EntityManager::get()->addMonster(monster);





    //for (int  i = 0; i < 60; i ++) {
    //    Monster * monster = new Monster("monster" + dt::Utils::toString(i), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 3, 3);
    //    scene->addChildNode(monster);
    //    //monster->setCurHealth(100);
    //    //dt::Logger().get().debug(monster->getFullName());
    //    MonsterAIAgent * ma5 = new MonsterAIAgent("ma" + dt::Utils::toString(i));
    //    Ogre::Vector3 tmp = AIDivideAreaManager::get()->getArea(i);
    //    setP(monster, ma5, tmp.x, 10, tmp.z);
    //    monster->setCurSpeed(6.0);
    //    monster->setScale(0.015);
    //    EntityManager::get()->addMonster(monster);
    //}
 /*   for (int i = -6; i < 6; i ++) 
        for (int j = -6; j < 6; j ++) {
            if (i == 0 && j == 0) continue;
             Monster * monster5 = new Monster("monster" + dt::Utils::toString(i * 1000 + j), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 3, 3);
        scene->addChildNode(monster5);
        dt::Logger().get().debug(monster5->getFullName());
        MonsterAIAgent * ma5 = new MonsterAIAgent("ma" + dt::Utils::toString(i * 1000 + j));
        Ogre::Vector3 tmp = EntityManager::get()->getHuman()->getPosition();
        setP(monster5, ma5, tmp.x + i * 0.6, 10, tmp.z + j * 0.6);
        monster5->setCurSpeed(6.0);
        monster5->setScale(0.001);
        EntityManager::get()->addMonster(monster5);
            
        }*/
   /* OgreProcedural::PlaneGenerator().setSizeX(1000.0f).setSizeY(1000.0f).setUTile(100).setVTile(100).realizeMesh("Plane");
    auto planeNode = scene->addChildNode(new dt::Node("planeNode"));
	planeNode->setPosition(0, 0, 0); 
	planeNode->addComponent(new dt::MeshComponent("Plane", "Wall-05", "plane-mesh"));
    planeNode->addComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body",
        dt::PhysicsBodyComponent::BOX, 0.0f));*/

 
}
void AITest::onDeinitialize() {

}

void AITest::updateStateFrame(double simulation_frame_time) {
    static bool flag = false;

    if (!flag) {
        flag = true;
        this->getScene("test.scene")->getPhysicsWorld()->setGravity(Ogre::Vector3(0, -9.8, 0));
    }
}
