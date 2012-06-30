#include "EntityManager.h"
#include "Monster.h"
#include "Alien.h"
#include "MonsterAIAgent.h"
#include "PlayerAIAgent.h"
#include "Character.h"
#include "Agent.h"
#include "AIDivideAreaManager.h"
#include <Scene/Scene.hpp>

#include <vector>


EntityManager* EntityManager::get() {
	static EntityManager * singleton = new EntityManager(); 
	return singleton;
}

Alien * EntityManager::getHuman() {
    return mHuman; 
}
void EntityManager::setHuman(Alien * human) {
    mHuman = human;
}
void EntityManager::afterLoadScene(dt::Scene * scene) {
    mMonsterNum = 0;
    mCurScene = scene;
    Ogre::Vector3 human_pos = getHuman()->getPosition(); 
    uint16_t human_area = AIDivideAreaManager::get()->getIdByPosition(human_pos);
    vector<uint16_t> area = AIDivideAreaManager::get()->getClosestArea(human_area);
    for (uint16_t i = 0; i < area.size(); i ++) {
        vector<std::pair<uint16_t, uint16_t> > vpuu;
        for (uint16_t j = 0; j < 4; j ++) {
            mMonsterNum ++;
            std::pair<uint16_t, uint16_t> puu = AIDivideAreaManager::get()->randomPosition(area[i]);
            Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getPositionById(puu);
            vpuu.push_back(puu);
            Monster * monster = new Monster("monster" + dt::Utils::toString(mMonsterNum), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 20, 3);      
            MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(mMonsterNum));           
            addMonsterInScene(monster, maa, monster_pos.x, 10, monster_pos.z);
        }
        for (uint16_t j = 0; j < vpuu.size(); j ++) 
            AIDivideAreaManager::get()->destroy(vpuu[j]);
    }
}
void EntityManager::addMonsterInScene(Monster * monster, MonsterAIAgent * agent, double x, double y, double z) {
    mCurScene->addChildNode(monster);
    auto physics = monster->findComponent<dt::PhysicsBodyComponent>("physics_body");
    auto motion = physics->getRigidBody()->getMotionState();
    btTransform trans;
    motion->getWorldTransform(trans);
    trans.setOrigin(btVector3(x, y, z));
    motion->setWorldTransform(trans);
    physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(0.01, 0.01, 0.01));   
    monster->setPosition(x, y, z);
    monster->setScale(0.015);
    agent->attachTo(monster);
}