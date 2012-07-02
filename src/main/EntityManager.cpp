#include "EntityManager.h"
#include "Monster.h"
#include "Alien.h"
#include "ParticlesEffect.h"
#include "MonsterAIAgent.h"
#include "PlayerAIAgent.h"
#include "Character.h"
#include "Agent.h"
#include "AIDivideAreaManager.h"
#include <Scene/Scene.hpp>
#include "Agent.h"
#include <vector>


const double  EntityManager::PI = acos(-1.0);
const double  EntityManager::THREAT_RANGE = 40.0;
const double  EntityManager::THREAT_HALF_DEGREE = 15.0;
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
    mAlien.push_back(mHuman);
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

    for (uint16_t i = 0; i < 60; i ++) {
        mMonsterNum ++;
        Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getArea(i % 60);
         Monster * monster = new Monster("monster" + dt::Utils::toString(mMonsterNum), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 20, 3);      
         MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(mMonsterNum));           
         addEntityInScene(monster, maa, monster_pos.x, 10, monster_pos.z, 0.03);
         addMonster(monster);
         monster->setAttackInterval(1.0);
         monster->setCurHealth(10);
         monster->setMaxHealth(10);
         monster->setAttackRange(80);        
    }
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
        for (vector<Character*>::iterator itr = mAlien.begin(); 
            itr != mAlien.end(); itr ++)  {
            Ogre::Vector3 alien_pos = (*itr)->getPosition();
            if (_dis(alien_pos, cur_pos) < range) {
                res.push_back(*itr);
                return res;
            }
        }
         for (vector<Character*>::iterator itr = mMonster.begin(); 
            itr != mMonster.end(); itr ++)  {
                Ogre::Vector3 monster_pos = (*itr)->getPosition(); 
                if (_dis(monster_pos, cur_pos) < range) {
                    res.push_back(*itr);
                    return res;
                }
        }
    } else {
        for (vector<Character*>::iterator itr = mMonster.begin(); 
            itr != mMonster.end(); itr ++)  {
                Ogre::Vector3 monster_pos = (*itr)->getPosition(); 
                if (_dis(monster_pos, cur_pos) < range) {
                    res.push_back(*itr);
                    return res;
                }
        }
         for (vector<Character*>::iterator itr = mAlien.begin(); 
            itr != mAlien.end(); itr ++)  {
            Ogre::Vector3 alien_pos = (*itr)->getPosition();
            if (_dis(alien_pos, cur_pos) < range) {
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
/*
     mMonsterNum ++;
        Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getArea(mMonsterNum % 60);
         Monster * monster1 = new Monster("monster" + dt::Utils::toString(mMonsterNum), "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 3, 20, 3);      
         MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(mMonsterNum));           
         addEntityInScene(monster1, maa, monster_pos.x, 10, monster_pos.z, 0.03);
         addMonster(monster1);
         monster1->setAttackInterval(1.0);
         monster1->setAttackRange(40);
         addMonster(monster1);
         */
    for (vector<Character*>::iterator itr = mMonster.begin(); 
        itr != mMonster.end(); itr ++) {
        if (*itr == monster) {
            mMonster.erase(itr);
            return;
        }
    }
}
void EntityManager::__isAlienDead(Character * alien) {
    if (alien == nullptr) return; 
    for (vector<Character*>::iterator itr = mAlien.begin(); 
        itr != mAlien.end(); itr ++) {
        if (*itr == alien) {
            mAlien.erase(itr);
            return;
        }
    }        
}

void EntityManager::addPlayer(Alien * playerAI) {
    if (playerAI != nullptr) {
        for (vector<Character*>::iterator itr = mAlien.begin(); 
            itr != mAlien.end(); itr ++) 
            if (*itr == playerAI) return; 
        mAlien.push_back(playerAI);
    }
}

void EntityManager::addMonster(Monster * monster) {
    if (monster != nullptr){
        for (vector<Character*>::iterator itr = mMonster.begin(); 
            itr != mMonster.end(); itr ++)
            if (*itr == monster) return; 
        mMonster.push_back(monster);
    }
}

double EntityManager::clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre) {
    nxt.y = pre.y = 0;
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 
    //由目标位置和当前位置算出期望方向的向量。
    Ogre::Vector3 dv = pre - nxt; 
    dv.y = 0; 
    double res = asin((double) ( dy.crossProduct(dv).y / (dy.length() * dv.length()) )) * 180 / PI;
       if (dy.dotProduct(dv) < 0)
            res = res > 0 ? 180.0 - res : -180.0 - res;
    return res;
}

void EntityManager::fixDegree(double & degree) {
    while (degree < -180.0) degree += 360.0; 
    while (degree > 180.0)  degree -= 360.0; 
}

void EntityManager::fixTurn(double & d_degree) {
    if (fabs(d_degree) > 180.0) {       
            if (d_degree < 0) d_degree = 360.0 + d_degree;
            else d_degree = d_degree - 360.0;
    }    
}

bool EntityManager::isForwardThreaten(Agent * agent) {
    if (agent == nullptr) {
        throw exception("null pointer!");
    }
    MonsterAIAgent * monsterAI = dynamic_cast<MonsterAIAgent *>(agent);
    if (monsterAI != nullptr) {
        for (vector<Character*>::iterator itr = mAlien.begin(); 
            itr != mAlien.end(); itr ++) {                
                Ogre::Vector3  threat_pos = (*itr)->getPosition();
                Ogre::Vector3  cur_pos = monsterAI->getParent()->getPosition();
                if (_dis(threat_pos, cur_pos) > THREAT_RANGE) continue;
                double threat_degree = clacDegree(threat_pos, cur_pos);
                double d_degree = threat_degree - monsterAI->getPreDegree();
                fixTurn(d_degree);
                if (fabs(d_degree) < THREAT_HALF_DEGREE) return true; 
        }
    }
    return false; 
}

vector<Character*> EntityManager::searchThreatEntity(Character * entity) {
     if (entity == nullptr) {
        throw exception("null pointer!");
     }
     Monster * monster = dynamic_cast<Monster*>(entity);
     vector<Character*> res;
    if (monster != nullptr) {
        for (vector<Character*>::iterator itr = mAlien.begin(); 
            itr != mAlien.end(); itr ++) {                
                Ogre::Vector3  threat_pos = (*itr)->getPosition();
                Ogre::Vector3  cur_pos = monster->getPosition();
                if (_dis(threat_pos, cur_pos) > THREAT_RANGE) continue;
                double threat_degree = clacDegree(threat_pos, cur_pos);
                double d_degree = threat_degree - dynamic_cast<MonsterAIAgent*>(monster->findChildNode("agent").get())->getPreDegree();
                fixTurn(d_degree);
                if (fabs(d_degree) < THREAT_HALF_DEGREE) {
                    res.push_back(*itr);
                }
        }
    }
    return res;
}
