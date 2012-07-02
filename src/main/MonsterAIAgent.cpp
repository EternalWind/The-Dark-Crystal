#include "MonsterAIAgent.h"
#include "AIDivideAreaManager.h"
#include "Monster.h"
#include "EntityManager.h"
#include "AttackDetectComponent.h"
#include <Logic/RaycastComponent.hpp>
#include "Alien.h"
#include <Utils/Utils.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Character.h"
#include "EntityManager.h"

const double MonsterAIAgent::ENTER_SCOPE = 6.0;
const double  MonsterAIAgent::THREAT_COOL_TIME = 0.3;
const QString MonsterAIAgent::INTERACTOR_COMPONENT = "Monster_INTERACTOR_COMPONENT";
const QString MonsterAIAgent::TRIGGER_AREA_COMPONENT = "Monster_TRIGGER_AREA_COMPONENT";
const double  MonsterAIAgent::eps = 1e-4;
const double  MonsterAIAgent::MOVE_ROTATE_SPEED = 180;
const double  MonsterAIAgent::GUARD_ROTATE_SPEED = 180;
const double  MonsterAIAgent::PI = acos(-1.0);
const double  MonsterAIAgent::ROTATE_FLOAT = 6.0; 
const double  MonsterAIAgent::GUARD_RANGE = 30.0;
const double  MonsterAIAgent::AVOID_COLLI_RANGE = 10;


double MonsterAIAgent::getPreDegree() {
    return mPreDegree;
}
MonsterAIAgent::MonsterAIAgent(QString name, MonsterAIAgent::MonsterType type) : mType(type), Agent(name){
    this->mHasEnemy = this->mThreat = this->mOnWay = false;   
    mPreDegree = 0;   
    mOnMovePress = false;
    mColli = 0; 
    mSpeedUpPress = 0;
    mAttackPress = 0;
}

bool MonsterAIAgent::isThreat() {
    return mThreat;
}
void MonsterAIAgent::setThreat(bool type) {
    mThreat = type; 
}
void MonsterAIAgent::setBody(Monster* body) {
	mBody = body;
}
void MonsterAIAgent::onInitialize() {        
    setBody(dynamic_cast<Monster *>(this->getParent()));       
}

void MonsterAIAgent::onDeinitialize() {  
}

void MonsterAIAgent::findAndAttack(double time_diff) {
       bool threat = EntityManager::get()->isForwardThreaten(this);
       if (threat) {
           mThreatTime = THREAT_COOL_TIME;
           if (!mAttackPress) {
                emit(sAttack(true));
                mAttackPress = 1;
           }
       } else {
           if (mAttackPress) {
                emit(sAttack(false));
                mAttackPress = 0;
           }
       }                 
       fixDegree(mPreDegree);
       double d_degree = mExpectDegree - mPreDegree;
       fixTurn(d_degree);      
       lookAround(d_degree, time_diff, GUARD_ROTATE_SPEED);         
   
}

void MonsterAIAgent::walk(double time_diff) {   
   //[-180,180]    
    double cur_degree;    
   //修正一下角度，使它一直在[-180,180]。
    fixDegree(mPreDegree);
    //指向z轴正方向的向量。
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1);      
    Ogre::Vector3 pre_position = mBody->getPosition();    
    pre_position.y =0;
    Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);   
    nxt_area_position.y = 0;
    //如果目标已经到达，则停止走动并退出。
    if (nxt_area_position.distance(pre_position) < ENTER_SCOPE) {
            mOnWay = false;     
            if (mOnMovePress) {
                emit(sMove(Entity::STOP, true));                 
                mOnMovePress = 0; 
            }
            return;
    }

    mExpectDegree = clacDegree(nxt_area_position, pre_position);

    double d_degree = mExpectDegree - mPreDegree;   

    fixTurn(d_degree);

    //当前帧如果已经在角度幅度内，则开始走动。
    if (fabs(d_degree) < ROTATE_FLOAT) { 
        if (!mOnMovePress) {            
            if (!mSpeedUpPress) {
                emit(sSpeedUp(true));
                mSpeedUpPress = 1;
            }            
            emit(sMove(Entity::FORWARD, true)); 
            mOnMovePress = 1;            
        }
      } else{              
          lookAround(d_degree, time_diff, MOVE_ROTATE_SPEED);      
    }
}
void MonsterAIAgent::onUpdate(double time_diff) {  
    if (time_diff == 0.0) return; 
    if (this->getParent() == nullptr) return; 
     //update调用子节点的update和它的component。    
    dt::Node::onUpdate(time_diff);  
    vector<Character *> vc = EntityManager::get()->searchEntityByRange(mBody, GUARD_RANGE);
    for (uint16_t i = 0; i < vc.size(); i ++) onTriggerr(vc[i]);
    
  /*  
    if (mColli) {
        mOnWay = false; 
        mThreat = true; 
        mThreatTime = THREAT_COOL_TIME;
        if (mOnMovePress) {
            mOnMovePress = 0; 
            emit(sMove(Entity::STOP, true));
        }
        mColli = 0;
    }
    */
     //警戒状态下，警戒状态是因为有敌人出现在警戒区域。
  
    if (mThreat) {
        findAndAttack(time_diff); 
        mThreatTime -= time_diff;         
        if (mThreatTime <= eps) {
            mThreat = false; 
            if (mAttackPress) {
                mAttackPress = 0; 
                emit(sAttack(false));
            }            
        }
       
    } else if (mOnWay) { //在行走，则走之。
        walk(time_diff); 
    } else decision(time_diff);  // 否则，决策之。
            
            
}
void MonsterAIAgent::decision(double time_diff) {

        Ogre::Vector3 cur_pos = mBody->getPosition(); 
        uint16_t cur_id = AIDivideAreaManager::get()->getIdByPosition(cur_pos); 
        Ogre::Vector3 human_pos = EntityManager::get()->getHuman()->getPosition(); 
        
        uint16_t des_id = AIDivideAreaManager::get()->getIdByPosition(human_pos); 
        uint16_t diff = AIDivideAreaManager::get()->getAreaNumBetween(cur_id, des_id); 
        if (diff > 0) {
            uint16_t nxt_id = AIDivideAreaManager::get()->getNxtClosestId(cur_id, des_id);                   
            std::pair<uint16_t, uint16_t> tmp = AIDivideAreaManager::get()->randomPosition(nxt_id);
            if (tmp.first != -1) {     
                    AIDivideAreaManager::get()->destroy(mNxtArea);
                    mNxtArea = tmp;
                    mOnWay = true;                    
                    return; 
            }
        }  

          //否则，原地警戒。
    mThreat = true; 
    mThreatTime = THREAT_COOL_TIME;   
}


void MonsterAIAgent::onTriggerr(Character * c) {
    if (c == nullptr) {
       return;
    }
    Alien* enemy = dynamic_cast<Alien*>(c);    
    if (enemy != nullptr) {        
        mThreat = true; 
        mThreatTime = THREAT_COOL_TIME;
        AIDivideAreaManager::get()->destroy(mNxtArea);
        mOnWay = 0; 
        if (mOnMovePress) {
            mOnMovePress = 0; 
            emit(sMove(Entity::STOP, true));            
        }
        mExpectDegree = clacDegree(enemy->getPosition(), mBody->getPosition());
        return; 
    } 
    if (mThreat) return;
   /* Monster * ym = dynamic_cast<Monster*>(c);
    
    if (ym != nullptr) {
        Ogre::Vector3 ym_pos = ym->getPosition();
        Ogre::Vector3 my_pos = mBody->getPosition();
        if (ym_pos.distance(my_pos) < AVOID_COLLI_RANGE) {
            mColli ++;
            if (dynamic_cast<MonsterAIAgent*>(ym->findChildNode("agent").get())->isThreat()) 
                mColli --;
        }
    }*/
}



void MonsterAIAgent::__onFind(dt::PhysicsBodyComponent* pbc) {
    //操蛋的居然会传入空指针！！！
    if (pbc == nullptr) {
        return; 
    }
    Alien* enemy = dynamic_cast<Alien*>(pbc->getNode());
    
     if (enemy != nullptr) {

         Ogre::Vector3 enemy_pos = enemy->getPosition();
         enemy_pos.y = 0;
         Ogre::Vector3 my_pos = mBody->getPosition();
         my_pos.y = 0;

         if (!mAttackPress && !mOnMovePress) {
            if (!mSpeedUpPress) {
                emit(sSpeedUp(true));
                mSpeedUpPress = 1;
            }
            emit(sMove(Entity::FORWARD, true));   
            mOnMovePress = 1; 
        }
        mHasEnemy = true;    
        if (enemy_pos.distance(my_pos) < mBody->getAttackRange()) {
            if (!mAttackPress) {
                if (mOnMovePress) {
                    mOnMovePress = 0; 
                    emit(sMove(Entity::STOP, true));                    
                }
                emit(sAttack(true));
                mAttackPress = 1;                
            }
         }       
     }  else {
        if (mOnMovePress) {
            emit(sMove(Entity::STOP, true));
            mOnMovePress = 0;
        }
        mHasEnemy = false; 
     }
}

void MonsterAIAgent::lookAround(double d_degree, double time_diff, double speed) {
     if (d_degree > 0) {
         emit(sLookAround(Ogre::Quaternion(Ogre::Degree(speed * time_diff),
                                                Ogre::Vector3(0,1,0))));
              mPreDegree += speed * time_diff;
        } else { 
            emit(sLookAround(Ogre::Quaternion(Ogre::Degree(speed * time_diff),
                                                Ogre::Vector3(0,-1,0))));
              mPreDegree -= speed * time_diff;  
             
        }
}
double MonsterAIAgent::clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre) {
    nxt.y = pre.y = 0;
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 
    //由目标位置和当前位置算出期望方向的向量。
    Ogre::Vector3 dv = pre - nxt; 
    dv.y = 0; 
    double res = asin((double) ( dy.crossProduct(dv).y / (dy.length() * dv.length()) )) * 180 / PI;
       if (dy.dotProduct(dv) < 0)
            res = res > 0 ? 180 - res : -180 - res;
    return res;
}

void MonsterAIAgent::fixDegree(double & degree) {
    while (degree < -180.0) degree += 360.0; 
    while (degree > 180.0)  degree -= 360.0; 
}

void MonsterAIAgent::fixTurn(double & d_degree) {
    if (fabs(d_degree) > 180 + eps) {       
            if (d_degree < 0) d_degree = 360 + d_degree;
            else d_degree = d_degree - 360;
    }    
}