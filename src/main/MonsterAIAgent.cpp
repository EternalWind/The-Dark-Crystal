#include "MonsterAIAgent.h"
#include "AIDivideAreaManager.h"
#include "Monster.h"
#include <Logic/RaycastComponent.hpp>
#include <Logic/RaycastComponent.hpp>
#include "Alien.h"

const QString MonsterAIAgent::ENTER_SCOPE = 1.0;
const QString MonsterAIAgent::INTERACTOR_COMPONENT = "Monster_INTERACTOR_COMPONENT";
const QString MonsterAIAgent::TRIGGER_AREA_COMPONENT = "Monster_TRIGGER_AREA_COMPONENT";
const double  MonsterAIAgent::eps = 1e-4;
const double  MonsterAIAgent::MOVE_ROTATE_SPEED = 360;
const double  MonsterAIAgent::GUARD_ROTATE_SPEED = 180;
const double  MonsterAIAgent::PI = acos(-1.0);
const double  MonsterAIAgent::ROTATE_FLOAT = PI / 24; 

MonsterAIAgent::MonsterAIAgent(QString name, MonsterAIAgent::MonsterType type, uint16_t cur_area) : mType(type), Agent(name), mCurArea(cur_area){
    this->mHasEnemy = this->mThreat = this->mOnWay = this->mHasEnemy = false; 
}

bool MonsterAIAgent::isThreat() {
    return mThreat;
}
void MonsterAIAgent::setThreat(bool type) {
    mThreat = type; 
}
void MonsterAIAgent::findAndAttack(double time_diff) {
    this->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();

    if (!mHasEnemy) {
        Ogre::Vector3 tmp;
        Ogre::Degree td; 
        mBody->getRotation().ToAngleAxis(td, tmp);
        double cur_degree = td.valueDegrees();
        emit(sLookAround(Ogre::Quaternion(Ogre::Degree(cur_degree + time_diff * GUARD_ROTATE_SPEED),
                                          Ogre::Vector3(0,1,0))));
    }

    mHasEnemy = false; 

}
void MonsterAIAgent::onInitialize() {

    setBody(dynamic_cast<Monster *>(this->getParent()));

    mIteractor = this->addComponent<dt::InteractionComponent>(
        new dt::RaycastComponent(INTERACTOR_COMPONENT)).get();
    //只要没有障碍物，就可以攻击。
    mIteractor->setRange(3000.0f);    
    connect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
            this, SLOT(__onFind(dt::PhysicsBodyComponent*)));    

    mTrigger = this->addComponent<dt::TriggerAreaComponent>(new dt::TriggerAreaComponent(
        new btBoxShape(btVector3(5.0f, 5.0f, 5.0f)), TRIGGER_AREA_COMPONENT)).get();
    connect(mTrigger, SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
            this, SLOT(__onTrigger(dt::TriggerAreaComponent*, dt::Component*)));
}
void MonsterAIAgent::onDeinitialize() {
    disconnect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
            this, SLOT(__onFind(dt::PhysicsBodyComponent*)));    
    disconnect(mTrigger, SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
            this, SLOT(__onTrigger(dt::TriggerAreaComponent*, dt::Component*)));
}
void MonsterAIAgent::setBody(Monster* body) {
	mBody = body;
}
void MonsterAIAgent::walk(double time_diff) {   
   //[-180,180]
    double pre_degree;
    double cur_degree;
    double expect_degree;
    Ogre::Vector3 tmp;
    Ogre::Degree td; 
    mBody->getRotation().ToAngleAxis(td, tmp);
    pre_degree =td.valueDegrees() * tmp.y;
    
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 
    Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);    
    Ogre::Vector3 pre_position = mBody->getPosition(); 
   
    Ogre::Vector3 dv = nxt_area_position - pre_position; 
    dv.y = 0; 
    
    expect_degree = asin((double) ( dy.crossProduct(dv).y / (dy.length() * dv.length()) )) * 360 / PI;
    
    double d_degree = expect_degree - pre_degree;
    if (fabs(d_degree) > 180 + eps) {
        if (d_degree < 0) d_degree = 360 + d_degree;
        else d_degree = d_degree - 360;
    }

    //当前帧如果已经在角度幅度内，则开始走动。
    if (fabs(d_degree) < ROTATE_FLOAT) {
        emit(sMove(Entity::FORWARD, true)); 
        Ogre::Vector3 cur_position = mBody->getPosition();  
        //已经到达目标，mOnWay状态取消。
        if (nxt_area_position.distance(cur_position) < ENTER_SCOPE) {
            mOnWay = false;             
        }
    } else {        
        emit(sMove(Entity::STOP, true));
        if (d_degree > 0) 
              emit(sLookAround(Ogre::Quaternion(Ogre::Degree(pre_degree + MOVE_ROTATE_SPEED * time_diff),
                                                Ogre::Vector3(0,1,0))));
        else  emit(sLookAround(Ogre::Quaternion(Ogre::Degree(pre_degree - MOVE_ROTATE_SPEED * time_diff),
                                                Ogre::Vector3(0,1,0))));
    }
    
}
void MonsterAIAgent::onUpdate(double time_diff) {
    if (mThreat) {
        findAndAttack(time_diff);
    } else if (mType == PATROL) 
            if (mOnWay) walk(time_diff); 
            else decision(time_diff); 
}
void MonsterAIAgent::decision(double time_diff) {
    
}


void MonsterAIAgent::__onTrigger(dt::TriggerAreaComponent* tac, dt::Component* c) {
    Alien* enemy = dynamic_cast<Alien*>(c->getNode());
    if (enemy != nullptr) {
        mThreat = true; 
        return; 
    }
}
void MonsterAIAgent::__onFind(dt::PhysicsBodyComponent* pbc) {
    Alien* enemy = dynamic_cast<Alien*>(pbc->getNode());
     if (enemy != nullptr) {
        emit(sMove(Entity::FORWARD, true));    
        mHasEnemy = true; 
     } else {
        emit(sMove(Entity::FORWARD, false));    
     }
}