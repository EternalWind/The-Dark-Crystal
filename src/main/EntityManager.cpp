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

#include <QtXml/QtXml>


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
void EntityManager::afterLoadScene(dt::Scene * scene, QString stage) {
    if (stage == "01") mCurStage = 0; 
    if (stage == "02") mCurStage = 1;
    if (stage == "03") mCurStage = 2;     
    std::cout << mCurStage << endl; 
    mMonster.clear(); 
    mAlien.clear();
    mAlien.push_back(mHuman);
    mMonsterNum = 0;
    mCurScene = scene;

    mg[0][0] = 54, 
        mg[0][1] = 23,
        mg[0][2] = 51; 
    mg[0][3] = 1, 
        mg[0][4] = 55, 
        mg[0][5] = 34;
    monsterNum[0] = 100;

    mg[1][0] = 81, mg[1][1] = 48, mg[1][2] = 83; 
    mg[1][3] = 12, mg[1][4] = 57, mg[1][5] = 70;

      mg[2][0] = 83, mg[2][1] = 15, mg[2][2] = 65; 
    mg[2][3] = 152, mg[2][4] = 42, mg[2][5] = 80;

    monsterNum[1] = 100;

    //0:血量；1：速度；2：攻击力；3：攻击间隔。4：体型
    monsterValue[0][0] = 100, monsterValue[0][1] = 8, monsterValue[0][2] = 10, monsterValue[0][3] = 4, monsterValue[0][4] = 0.03;
    monsterValue[1][0] = 40, monsterValue[1][1] = 12, monsterValue[1][2] = 4, monsterValue[1][3] = 2,  monsterValue[1][4] = 0.05;

    __loadMonster("yangmu");

    //for (uint16_t i = 0; i < 1; i ++) {
    for (uint16_t i = 0;  i < mMonsterInfo.mInitNum; ++i) {
        monsterNum[mCurStage] --;

        if (monsterNum[mCurStage] > 0) {        
            Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getPositionById(
                AIDivideAreaManager::get()->randomPosition(mg[mCurStage][monsterNum[mCurStage] % 6])
                );
            /* std::cout << monster_pos.x << ' ' << monster_pos.z << endl; 
            std::cout << mg[mCurStage][monsterNum[mCurStage] % 6] << endl; */

            /*
            Monster * monster = new Monster("monster" + dt::Utils::toString(monsterNum[mCurStage]),
                "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 
                monsterValue[monsterNum[mCurStage] % 2][2],  //攻击力
                40,  //攻击range   
                monsterValue[monsterNum[mCurStage] % 2][3]);      //攻击间隔
                */

            Monster *monster = new Monster(
                "monster" + dt::Utils::toString(monsterNum[mCurStage]),
                mMonsterInfo.mMeshHandle,
                dt::PhysicsBodyComponent::BOX,
                mMonsterInfo.mMass,
                "sounds/Monster_walk.wav",
                "sounds/Monster_jump.wav",
                "sounds/Monster_run.wav",
                "sounds/Monster_attack.wav",
                mMonsterInfo.mAttackValue,
                mMonsterInfo.mAttackRange,
                mMonsterInfo.mAttackInterval);

            MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(monsterNum[mCurStage]));  


            addEntityInScene(monster, maa, monster_pos.x, 10, monster_pos.z, monsterValue[monsterNum[mCurStage] % 2][4]);
            addMonster(monster);
            //monster->setMaxHealth(monsterValue[monsterNum[mCurStage] % 2][0]); //血量
            //monster->setCurSpeed(monsterValue[monsterNum[mCurStage] % 2][1]); //行走速度
            //monster->setCurHealth(monster->getMaxHealth());  
            monster->setMaxHealth(mMonsterInfo.mMaxHealth);
            monster->setCurHealth(mMonsterInfo.mMaxHealth);
            monster->setOrigSpeed(mMonsterInfo.mOrigSpeed);
            monster->setCurSpeed(mMonsterInfo.mOrigSpeed);
        }
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
        /*   for (vector<Character*>::iterator itr = mMonster.begin(); 
        itr != mMonster.end(); itr ++)  {
        Ogre::Vector3 monster_pos = (*itr)->getPosition(); 
        if (_dis(monster_pos, cur_pos) < range) {
        res.push_back(*itr);
        return res;
        }
        }*/
    } else {
        for (vector<Character*>::iterator itr = mMonster.begin(); 
            itr != mMonster.end(); itr ++)  {
                Ogre::Vector3 monster_pos = (*itr)->getPosition(); 
                if (_dis(monster_pos, cur_pos) < range) {
                    res.push_back(*itr);
                    return res;
                }
        }
        /*  for (vector<Character*>::iterator itr = mAlien.begin(); 
        itr != mAlien.end(); itr ++)  {
        Ogre::Vector3 alien_pos = (*itr)->getPosition();
        if (_dis(alien_pos, cur_pos) < range) {
        res.push_back(*itr);
        return res;
        }
        }*/
    }
    return res;
}
double EntityManager::_dis(Ogre::Vector3 a, Ogre::Vector3 b) {
    a.y = 0; b.y = 0; 
    return a.distance(b);
}
void  EntityManager::__isMonsterDead(Character * monster) {
    if (monster == nullptr) return; 

    MonsterAIAgent * ma = dynamic_cast<MonsterAIAgent*>(monster->findChildNode("agent").get());

    uint16_t k; 
    if (monsterNum[mCurStage] > 0) k = 2; 
    else k = 1; 
    for (uint16_t i = 0; i < k; i ++) {
        monsterNum[mCurStage] --;

        if (monsterNum[mCurStage]) {        
            Ogre::Vector3 monster_pos = AIDivideAreaManager::get()->getPositionById(
                AIDivideAreaManager::get()->randomPosition(mg[mCurStage][monsterNum[mCurStage] % 6])
                );

            /*
            Monster * monster = new Monster("monster" + dt::Utils::toString(monsterNum[mCurStage]),
                "monster.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "","", 
                monsterValue[monsterNum[mCurStage] % 2][2],  //攻击力
                40,  //攻击range   
                monsterValue[monsterNum[mCurStage] % 2][3]);      //攻击间隔
                */
            Monster *monster = new Monster(
                "monster" + dt::Utils::toString(monsterNum[mCurStage]),
                mMonsterInfo.mMeshHandle,
                dt::PhysicsBodyComponent::BOX,
                mMonsterInfo.mMass,
                "sounds/Monster_walk.wav",
                "sounds/Monster_jump.wav",
                "sounds/Monster_run.wav",
                "sounds/Monster_attack.wav",
                mMonsterInfo.mAttackValue,
                mMonsterInfo.mAttackRange,
                mMonsterInfo.mAttackInterval);

            MonsterAIAgent * maa = new MonsterAIAgent("ma" + dt::Utils::toString(monsterNum[mCurStage]));  


            addEntityInScene(monster, maa, monster_pos.x, 10, monster_pos.z, monsterValue[monsterNum[mCurStage] % 2][4]);
            addMonster(monster);

            /*
            monster->setMaxHealth(monsterValue[monsterNum[mCurStage] % 2][0]); //血量
            monster->setCurSpeed(monsterValue[monsterNum[mCurStage] % 2][1]); //行走速度
            monster->setCurHealth(monster->getMaxHealth());  
            */
            monster->setMaxHealth(mMonsterInfo.mMaxHealth);
            monster->setCurHealth(mMonsterInfo.mMaxHealth);
            monster->setOrigSpeed(mMonsterInfo.mOrigSpeed);
            monster->setCurSpeed(mMonsterInfo.mOrigSpeed);
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

//
double EntityManager::avoidCollic(Character* entity, double range) {
    if (entity == nullptr) {
        throw exception("null pointer!");
    }     
    Monster * monster = dynamic_cast<Monster*>(entity);
    double res = dynamic_cast<MonsterAIAgent*>(monster->findChildNode("agent").get())->getPreDegree();
    if (monster != nullptr) {
        for (vector<Character*>::iterator itr = mMonster.begin(); 
            itr != mMonster.end(); itr ++) {                  
                Ogre::Vector3  threat_pos = (*itr)->getPosition();
                Ogre::Vector3  cur_pos = monster->getPosition();
                if (_dis(threat_pos, cur_pos) > range) continue;
                double threat_degree = clacDegree(cur_pos, threat_pos);
                double d_degree = res - threat_degree;
                fixTurn(d_degree);
                if (fabs(d_degree) < 90.0) {
                    double tmp = 90.0 - fabs(d_degree); 
                    if (d_degree < 0) tmp = - fabs(tmp); 
                    res -= tmp;
                    fixDegree(res);                    
                    return res;
                }
        }
    }    
    return res;
}

void EntityManager::__loadMonster(QString monster_name) {
    QFile file("AIMonsterAttribute.xml");

    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly)) {
        dt::Logger::get().error("Couldn't open file MonsterAttribute.xml");
    }

    doc.setContent(&file);

    QDomElement root = doc.documentElement();
    QDomElement w_node = root.firstChildElement(monster_name);

    mMonsterInfo.mMeshHandle = w_node.firstChildElement("mesh_handle").text();
    mMonsterInfo.mInitNum = w_node.firstChildElement("init_num").text().toUInt();
    mMonsterInfo.mMaxHealth = w_node.firstChildElement("max_health").text().toUInt();
    mMonsterInfo.mOrigSpeed = w_node.firstChildElement("orig_speed").text().toUInt();
    mMonsterInfo.mMass = w_node.firstChildElement("mass").text().toFloat();
    mMonsterInfo.mAttackValue = w_node.firstChildElement("attack_value").text().toFloat();
    mMonsterInfo.mAttackRange = w_node.firstChildElement("attack_range").text().toFloat();
    mMonsterInfo.mAttackInterval = w_node.firstChildElement("attack_interval").text().toFloat();
}