
#include <Scene/Scene.hpp>
#include <Core/ResourceManager.hpp>
#include <Logic/SimplePlayerComponent.hpp>
#include <Logic/CollisionComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Logic/AdvancedPlayerComponent.hpp>
#include <Input/InputManager.hpp>
#include <Graphics/LightComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <OgreProcedural.h>
#include <vector>
#include <Graphics/DisplayManager.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Physics/PhysicsManager.hpp>
#include "Monster.h"
#include "MonsterAIAgent.h"
#include "Alien.h"
#include "HumanAgent.h"
#include "PlayerAIAgent.h"
#include "AIAlienTest.h"
#include "AIDivideAreaManager.h"
#include <string>

using namespace std; 

AIAlienTest::AIAlienTest() {	
}
void AIAlienTest::onInitialize() {
	auto scene = addScene(new dt::Scene("testScene")); 
	//this->getScene("testScene")->getPhysicsWorld()->setShowDebug(true);
	dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	
	
	OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();
	OgreProcedural::PlaneGenerator().setSizeX(1000.0f).setSizeY(1000.0f).setUTile(10).setVTile(10).realizeMesh("Plane");
    
    AIDivideAreaManager::get()->loadMapInfo("map.txt"); 

	
	auto camNode = scene->addChildNode(new dt::Node("camNode")); 
	auto cam = camNode->addComponent(new dt::CameraComponent("cam")); 
	camNode->setPosition(0, 300, 0);
	cam->lookAt(0,0,0);




    /*
     Monster * mtr1 = new Monster("Monster1",
			"Sinbad.mesh",
            dt::PhysicsBodyComponent::BOX, 
			3.0f,
			"test_music_intro.ogg",
			"test_music_intro.ogg",
			"test_music_intro.ogg",
            "test_music_intro.ogg",
            3,
            2.0,
            1.0
		);
     MonsterAIAgent * mat1 = new MonsterAIAgent("ai1");
    auto MonsterNode1 = scene->addChildNode(mtr1);
    MonsterNode1->setPosition(0,10,10);
    MonsterNode1->addChildNode(mat1);
	*/
	
    Alien * alien = new Alien("alien",
			"Sinbad.mesh",
            dt::PhysicsBodyComponent::BOX, 
			1.0f,
			"test_music_intro.ogg",
			"test_music_intro.ogg",
			"test_music_intro.ogg"          
		);
    alien->setEyePosition(Ogre::Vector3(0,3,-3));     
    scene->addChildNode(alien);
   /* HumanAgent * ha = new HumanAgent("RJ");
    ha->attachTo(alien);	*/
    PlayerAIAgent * pt = new PlayerAIAgent("RJ", 1); 
    pt->attachTo(alien); 
	
		
	auto lightNode = scene->addChildNode(new dt::Node("lightNode"));
	lightNode->addComponent(new dt::LightComponent("light1"));
	lightNode->setPosition(0,100,0);


	auto planeNode = scene->addChildNode(new dt::Node("planeNode"));
	planeNode->setPosition(0, -2, 0); 
	planeNode->addComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    planeNode->addComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body",
        dt::PhysicsBodyComponent::BOX, 0.0f));

    
	/*if (!QObject::connect(trigger.get(),
				SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
                     this, 
                     SLOT(areaTriggered(dt::TriggerAreaComponent*, 
                     dt::Component*))) ) {
		
	}*/
	
	
}
void AIAlienTest::updateStateFrame(double simulation_frame_time) {
}
void AIAlienTest::onDeinitialize() {
}
void AIAlienTest::areaTriggered(dt::TriggerAreaComponent* trigger_area, dt::Component* component) {
	dt::Logger::get().info("triggerrrrr!!!!");	
	
	trigger_area->disable();
}