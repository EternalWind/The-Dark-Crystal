#include "PlayerAIAgent.h"
#include "AIDivideAreaManager.h"
#include "Monster.h"
#include <Logic/RaycastComponent.hpp>

const QString PlayerAIAgent::INTERACTOR_COMPONENT = "Player_AI_Agent_Interactor";
const QString PlayerAIAgent::TRIGGER_AREA_COMPONENT = "Player_AI_TRIGGER_AREA_COMPONENT";
const double  PlayerAIAgent::THREAT_COOL_TIME = 2.0;
const double  PlayerAIAgent::eps = 1e-4;
const double  PlayerAIAgent::MOVE_ROTATE_SPEED = 270;
const double  PlayerAIAgent::GUARD_ROTATE_SPEED = 90;
const double  PlayerAIAgent::PI = acos(-1.0);
const double  PlayerAIAgent::ROTATE_FLOAT = 180 / 24.0; 
const double  PlayerAIAgent::ENTER_SCOPE = 0.1;

PlayerAIAgent::PlayerAIAgent(QString name): Agent(name) {    
    //初始化无任何状态
    mFollow = mThreat = mOnWay = false;     
    mPreDegree = 0;    
}

bool PlayerAIAgent::isOnWay() {
	return mOnWay;
}

void PlayerAIAgent::setOnWay(bool type) {
	mOnWay = type;
}

Alien* PlayerAIAgent::getBody() {
	return mBody;
}

void PlayerAIAgent::setBody(Alien * body) {
	mBody = body;
}

void PlayerAIAgent::lookAround(double d_degree, double time_diff) {
     if (d_degree > 0) {
              emit(sLookAround(Ogre::Quaternion(Ogre::Degree(mPreDegree + MOVE_ROTATE_SPEED * time_diff),
                                                Ogre::Vector3(0,1,0))));
              mPreDegree += MOVE_ROTATE_SPEED * time_diff;
        } else { 
              emit(sLookAround(Ogre::Quaternion(Ogre::Degree(mPreDegree - MOVE_ROTATE_SPEED * time_diff),
              Ogre::Vector3(0,1,0))));
              mPreDegree -= MOVE_ROTATE_SPEED * time_diff;  
        }
}
double PlayerAIAgent::clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre) {
    nxt.y = pre.y = 0;
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 
    //由目标位置和当前位置算出期望方向的向量。
    Ogre::Vector3 dv = nxt - pre; 
    dv.y = 0; 
    double res = asin((double) ( dy.crossProduct(dv).y / (dy.length() * dv.length()) )) * 180 / PI;
       if (dy.dotProduct(dv) < 0)
            res = res > 0 ? 180 - res : -180 - res;
    return res;
}

void PlayerAIAgent::fixDegree(double & degree) {
    while (degree < -180.0) degree += 360.0; 
    while (degree > 180.0)  degree -= 360.0; 
}

void PlayerAIAgent::fixTurn(double & d_degree) {
    if (fabs(d_degree) > 180 + eps) {       
            if (d_degree < 0) d_degree = 360 + d_degree;
            else d_degree = d_degree - 360;
    }    
}
Alien * PlayerAIAgent::getHumanBody() {
    return mHumanBody; 
}
void PlayerAIAgent::setHumanBody(Alien * body) {
    this->mHumanBody = body; 
}
void PlayerAIAgent::walk(double time_diff) {      
   //[-180,180]    
    double cur_degree;    
   //修正一下角度，使它一直在[-180,180]。
    fixDegree(mPreDegree);
    //指向z轴正方向的向量。
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 

    Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);    
    Ogre::Vector3 pre_position = mBody->getPosition(); 
    pre_position.y = nxt_area_position.y = 0;

    //如果目标已经到达，则停止走动并退出。
    if (nxt_area_position.distance(pre_position) < ENTER_SCOPE) {
            mOnWay = false;     
            emit(sMove(Entity::STOP, true)); 
            return;
    }

    mExpectDegree = clacDegree(nxt_area_position, pre_position);

    double d_degree = mExpectDegree - mPreDegree;
    fixDegree(d_degree);
    //当前帧如果已经在角度幅度内，则开始走动。
    if (fabs(d_degree) < ROTATE_FLOAT) {           
        emit(sMove(Entity::STOP, true));
        emit(sMove(Entity::BACKWARD, true));          
      } else{        
      //  emit(sMove(Entity::STOP, true));   
          lookAround(d_degree, time_diff);      
    }
}

void PlayerAIAgent::guard(double time_diff) {
    //瞧瞧前面有没有异类，有就统统消灭！
    this->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();

    if (!mHasEnemy) {        
       double d_degree = mExpectDegree - mPreDegree;
       fixDegree(d_degree);      
       lookAround(d_degree, time_diff);      
    }
    mHasEnemy = false; 
}
void PlayerAIAgent::decision(double time_diff) {   
    //如果主角发出了跟随命令。
    if (mFollow) {
        Ogre::Vector3 cur_pos = mBody->getPosition(); 
        uint16_t cur_id = AIDivideAreaManager::get()->getIdByPosition(cur_pos); 
        Ogre::Vector3 human_pos = getHumanBody()->getPosition(); 
        uint16_t des_id = AIDivideAreaManager::get()->getIdByPosition(human_pos); 
        uint16_t diff = AIDivideAreaManager::get()->getAreaNumBetween(cur_id, des_id); 
        if (diff < 3) {
            uint16_t nxt_id = AIDivideAreaManager::get()->getNxtClosestId(cur_id, des_id);
        
            if (nxt_id != -1) {
                mOnWay = true; 
                AIDivideAreaManager::get()->markArea(mNxtArea, false); 
                mNxtArea = nxt_id; 
                AIDivideAreaManager::get()->markArea(mNxtArea, true);
                return; 
            }
        }
    }
    //否则，原地警戒。
    mThreat = true; 
    mThreatTime = THREAT_COOL_TIME; 
    /*
    mNxtArea = (mNxtArea + 1) % 17; 
    mOnWay = true;
    */
}
void PlayerAIAgent::onUpdate(double time_diff) {
    //警戒状态下，警戒状态是因为有敌人出现在警戒区域。
    //或者是有队友在警戒区域，为了防止两方相撞而设置不同的警戒时间。
    if (mThreat) {
        mThreatTime -= time_diff; 
        if (mThreatTime <= eps) {
            mThreat = false; 
        }
        guard(time_diff); 
    } else if (mOnWay) { //在行走，则走之。
        walk(time_diff); 
    } else decision(time_diff);  // 否则，决策之。
}

void PlayerAIAgent::onInitialize() {

    setBody(dynamic_cast<Alien *>(this->getParent()));

    mIteractor = this->addComponent<dt::InteractionComponent>(
        new dt::RaycastComponent(INTERACTOR_COMPONENT)).get();
    
    mIteractor->setRange(3000.0f);    
    connect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
            this, SLOT(__onFire(dt::PhysicsBodyComponent*)));

    mTrigger = this->addComponent<dt::TriggerAreaComponent>(new dt::TriggerAreaComponent(
        new btBoxShape(btVector3(5.0f, 5.0f, 5.0f)), TRIGGER_AREA_COMPONENT)).get();
    connect(mTrigger, SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
            this, SLOT(__onTrigger(dt::TriggerAreaComponent*, dt::Component*)));


}
void PlayerAIAgent::onDeinitialize() {
     disconnect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
            this, SLOT(__onFire(dt::PhysicsBodyComponent*)));
     disconnect(mTrigger, SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
            this, SLOT(__onTrigger(dt::TriggerAreaComponent*, dt::Component*)));
}

void PlayerAIAgent::__onFire(dt::PhysicsBodyComponent* pbc) {
     Monster* enemy = dynamic_cast<Monster*>(pbc->getNode());
     if (enemy != nullptr) {
        emit(sAttack(true));
        mHasEnemy = true; 
     } else {
        emit(sAttack(false));        
     }
}

void PlayerAIAgent::__onTrigger(dt::TriggerAreaComponent* tac, dt::Component* c) {
    //敌人
    Monster* enemy = dynamic_cast<Monster*>(c->getNode());
    if (enemy != nullptr) {
        mThreat = true; 
        mThreatTime = THREAT_COOL_TIME; 
        mExpectDegree = clacDegree(enemy->getPosition(), mBody->getPosition());
        return; 
    }
    //炮友!
    Alien * gun_friend = dynamic_cast<Alien *>(c->getNode());
    if (gun_friend != nullptr) {
       // gun_friend->get
        if (!mThreat) {
            mThreat = true; 
            //遇到队友就随机冷却时间防止互撞。
            mThreatTime = (rand()) % 10;
        }
    }    
}
