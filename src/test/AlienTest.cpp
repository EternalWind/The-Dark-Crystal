#include "AlienTest.h"

#include "Alien.h"

#include <Graphics/CameraComponent.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/LightComponent.hpp>

#include <OgreProcedural.h>

void AlienTest::onInitialize() {
    dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto scene = addScene(new dt::Scene("AlienTest"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::PlaneGenerator().setSizeX(100).setSizeY(100).realizeMesh("plane");

    auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    auto alien_node = scene->addChildNode(new Alien("alien", "Sinbad.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", ""));
    auto plane_node = scene->addChildNode(new dt::Node("plane_node"));
    auto light_node = scene->addChildNode(new dt::Node("light"));

    light_node->addComponent(new dt::LightComponent("l"));

    plane_node->addComponent(new dt::MeshComponent("plane", "PrimitivesTest/Pebbles", "Plane"));
    plane_node->setPosition(0, -10, 0);
    plane_node->addComponent(new dt::PhysicsBodyComponent("Plane", "plane_body", dt::PhysicsBodyComponent::BOX, 0.0f));

    camnode->setPosition(0, 5, 5);
    camnode->addComponent(new dt::CameraComponent("camera"))->lookAt(0, 0, 0);
}

void AlienTest::updateStateFrame(double time_diff) {}