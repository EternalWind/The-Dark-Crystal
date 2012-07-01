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
    mMonster.clear(); 
    mAlien.clear();
    mMonsterNum = 0;
    mCurScene = scene;
    Ogre::Vector3 human_pos = getHuman()->getPosition(); 
    uint16_t human_area = AIDivideAreaManager::get()->getIdByPosition(human_pos);
    //vector<uint16_t> area = AIDivideAreaManager::get()->getClosestArea(human_area);
    
   /* for (uint16_t i = 0; i < area.size(); i ++) {
        vector<std::pair<uint16_t, uint16_t> > vpuu;
        for (uint16_t j = 0; j < 10; j ++) {
            mMonsterNum ++;
            std::pair<uint16_t, uint16_t> puu = AIDivideAreaManager::get()->randomPosition(area[i]);
            Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getPositionById(puu);
            vpuu.push_back(puu);
            Monster * monster = new Monster("monster" + dt::Utils::toString(mMonsterNum), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 20, 3);      
            MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(mMonsterNum));           
            addEntityInScene(monster, maa, monster_pos.x, 8, monster_pos.z, 0.03);
        }

        Alien* alien = new Alien("alien" + dt::Utils::toString(i), "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
        alien->setEyePosition(Ogre::Vector3(0, 0.5, -2));
        PlayerAIAgent * pa = new PlayerAIAgent("rj" + dt::Utils::toString(i));    
        std::pair<uint16_t, uint16_t> puu = AIDivideAreaManager::get()->randomPosition(area[i]);
        vpuu.push_back(puu);
        Ogre::Vector3 alien_pos = AIDivideAreaManager::get()->getPositionById(puu);
        addEntityInScene(alien, pa, alien_pos.x, 10, alien_pos.z, 0.03);       
        EntityManager::get()->addPlayer(alien);    
        Weapon * weapon = new Weapon("RailGun","w" + dt::Utils::toString(i), Weapon::PRIMARY, 100, 60000, 60000, 1, 60000, 60000, 0, 1.0, 0, "", "", "", 50);
        alien->addWeapon(weapon);
        for (uint16_t j = 0; j < vpuu.size(); j ++) 
            AIDivideAreaManager::get()->destroy(vpuu[j]);
    }*/

    for (uint16_t i = 0; i < 10; i ++) {
        mMonsterNum ++;
        Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getArea(i);
         Monster * monster = new Monster("monster" + dt::Utils::toString(mMonsterNum), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 20, 3);      
         MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(mMonsterNum));           
         addEntityInScene(monster, maa, monster_pos.x, 10, monster_pos.z, 0.03);
         addMonster(monster);
         monster->setAttackInterval(1.0);
         monster->setAttackRange(40);
         std::cout << monster->getAttackRange() << endl; 
         std::cout << monster->findComponent<dt::InteractionComponent>("interator")->getIntervalTime() << std::endl;
    }

    /*for (uint16_t i = 50; i < 54; i ++) {
        mMonsterNum ++;
        Ogre::Vector3 alien_pos = AIDivideAreaManager::get()->getArea(i);
          Alien* alien = new Alien("alien" + dt::Utils::toString(i), "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
        PlayerAIAgent * pa = new PlayerAIAgent("pa" + dt::Utils::toString(mMonsterNum));           
         addEntityInScene(alien, pa, alien_pos.x, 10, alien_pos.z, 0.03); 
          EntityManager::get()->addPlayer(alien);    
        Weapon * weapon = new Weapon("RailGun","w" + dt::Utils::toString(i), Weapon::PRIMARY, 100, 60000, 60000, 1, 60000, 60000, 0, 1.0, 0, "", "", "", 50);
        alien->addWeapon(weapon);    
    }*/
}
void EntityManager::addEntityInScene(Character * entity, Agent * agent, double x, double y, double z, double scale) {
    mCurScene->addChildNode(entity);
    auto physics = entity->findComponent<dt::PhysicsBodyComponent>("physics_body");
    auto motion = physics->getRigidBody()->getMotionState();
    btTransform trans;
    motion->getWorldTransform(trans);
    trans.setOrigin(btVector3(x, y, z));
    trans.setRotation(btQuaternion(0,0,0,1));
    motion->setWorldTransform(trans);
    physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale, scale, scale));   
    entity->setPosition(x, y, z);
    entity->setScale(scale);
    agent->attachTo(entity);
}
vector<Character*> EntityManager::searchEntityByRange(Character * entity, double range) {
    
    Ogre::Vector3 cur_pos = entity->getPosition();
    vector<Character*> res;
    Alien * alien = dynamic_cast<Alien*>(entity);
    if (alien == nullptr) {
        Ogre::Vector3 human_pos = getHuman()->getPosition();
        if (_dis(human_pos, cur_pos) < range) {
            res.push_back(getHuman());
            return res;
        }
        for (set<Character*>::iterator itr = mAlien.begin(); 
            itr != mAlien.end(); itr ++)  {
            Ogre::Vector3 alien_pos = (*itr)->getPosition();
            if (_dis(alien_pos, cur_pos) < range) {
                res.push_back(*itr);
                return res;
            }
        }
    } else {
        for (set<Character*>::iterator itr = mMonster.begin(); 
            itr != mMonster.end(); itr ++)  {
                Ogre::Vector3 monster_pos = (*itr)->getPosition(); 
                if (_dis(monster_pos, cur_pos) < range) {
                    res.push_back(*itr);
                    return res;
                }
        }
    }
    return res;
}
double EntityManager::_dis(Ogre::Vector3 a, Ogre::Vector3 b) {
    a.y = 0; b.y = 0; 
    return a.distance(b);
}
void  EntityManager::__isMonsterDead(Character * monster) {
    if (monster == nullptr) return; 

     mMonsterNum ++;
        Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getArea(rand() % 60);
         Monster * monster1 = new Monster("monster" + dt::Utils::toString(mMonsterNum), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 20, 3);      
         MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(mMonsterNum));           
         addEntityInScene(monster1, maa, monster_pos.x, 10, monster_pos.z, 0.03);
         addMonster(monster1);
         monster1->setAttackInterval(1.0);
         monster1->setAttackRange(40);
         addMonster(monster1);
    mMonster.erase(mMonster.find(monster));    

}
void EntityManager::__isAlienDead(Character * alien) {
    if (alien == nullptr) return; 
    mAlien.erase(mAlien.find(alien));
}

void EntityManager::addPlayer(Alien * playerAI) {
    if (playerAI != nullptr)
    mAlien.insert(playerAI);
}

void EntityManager::addMonster(Monster * monster) {
    if (monster != nullptr){
        mMonster.insert(monster);
    }
}