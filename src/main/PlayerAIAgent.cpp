#include "PlayerAIAgent.h"
#include "AIDivideAreaManager.h"
#include "Monster.h"
#include <Logic/RaycastComponent.hpp>

const QString PlayerAIAgent::INTERACTOR_COMPONENT = "Player_AI_Agent_Interactor";
const QString PlayerAIAgent::TRIGGER_AREA_COMPONENT = "Player_AI_TRIGGER_AREA_COMPONENT";
const double  PlayerAIAgent::THREAT_COOL_TIME = 2.0;
const double  PlayerAIAgent::eps = 1e-4;
const double  PlayerAIAgent::MOVE_ROTATE_SPEED = 9;
const double  PlayerAIAgent::GUARD_ROTATE_SPEED = 90;
const double  PlayerAIAgent::PI = acos(-1.0);
const double  PlayerAIAgent::ROTATE_FLOAT = 180 / 24.0; 
const double  PlayerAIAgent::ENTER_SCOPE = 1.0;

PlayerAIAgent::PlayerAIAgent(QString name, uint16_t cur_area): Agent(name) {    
    //初始化无任何状态
    mFollow = mThreat = mOnWay = false; 
    mCurArea = cur_area; 
    pre_degree = 0; 
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
void PlayerAIAgent::walk(double time_diff) {   
    static bool flag = 0; 
   //[-180,180]
    
    double cur_degree;
    double expect_degree;
    Ogre::Vector3 tmp;
    Ogre::Degree td; 
    mBody->getRotation().ToAngleAxis(td, tmp);    
     
    while (pre_degree < -180.0) pre_degree += 360.0; 
    while (pre_degree > 180.0) pre_degree -= 360.0; 

    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 
    Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);    
    Ogre::Vector3 pre_position = mBody->getPosition(); 
   
    Ogre::Vector3 dv = nxt_area_position - pre_position; 
    dv.y = 0; 
    
    expect_degree = asin((double) ( dy.crossProduct(dv).y / (dy.length() * dv.length()) )) * 180 / PI;
    
   /* std::cout << expect_degree << std::endl;
    std::cout << pre_degree << std::endl; 
    std::cout << pre_position.x << " " << pre_position.y << " " << pre_position.z << std::endl; */
    double d_degree = expect_degree - pre_degree;
    if (fabs(d_degree) > 180 + eps) {
        if (d_degree < 0) d_degree = 360 + d_degree;
        else d_degree = d_degree - 360;
    }

    //当前帧如果已经在角度幅度内，则开始走动。
    if (fabs(d_degree) < ROTATE_FLOAT) {
      
      

        mBody->setCurSpeed(5);
        emit(sMove(Entity::STOP, true));
        emit(sMove(Entity::BACKWARD, true)); 
            Ogre::Vector3 cur_position = mBody->getPosition(); 
         /*   std::cout << cur_position.x << " " << cur_position.y << " " << cur_position.z << std::endl; 
            std::cout << d_degree << std::endl; 
            std::cout << expect_degree << std::endl;
            std::cout << pre_degree << std::endl; */
        //已经到达目标，mOnWay状态取消。
        if (nxt_area_position.distance(cur_position) < ENTER_SCOPE) {
            mOnWay = false;             
             emit(sMove(Entity::STOP, true));
        }
      

        
    } else{        
         emit(sMove(Entity::STOP, true));
       // std::cout << pre_degree << std::endl; 
        if (d_degree > 0) 
              emit(sLookAround(Ogre::Quaternion(Ogre::Degree(pre_degree + MOVE_ROTATE_SPEED * time_diff),
                                                Ogre::Vector3(0,1,0)), Ogre::Quaternion())), pre_degree +=MOVE_ROTATE_SPEED * time_diff;
        else  emit(sLookAround(Ogre::Quaternion(Ogre::Degree(pre_degree - MOVE_ROTATE_SPEED * time_diff),
                                                Ogre::Vector3(0,1,0)), Ogre::Quaternion())), pre_degree -= MOVE_ROTATE_SPEED * time_diff;
      
    }
    

}

void PlayerAIAgent::guard(double time_diff) {
    //瞧瞧前面有没有异类，有就统统消灭！
    this->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();

    if (!mHasEnemy) {
        Ogre::Vector3 tmp;
        Ogre::Degree td; 
        mBody->getRotation().ToAngleAxis(td, tmp);
        double cur_degree = td.valueDegrees() * tmp.y;
        emit(sLookAround(Ogre::Quaternion(Ogre::Degree(cur_degree + time_diff * GUARD_ROTATE_SPEED),
                                          Ogre::Vector3(0,1,0)), Ogre::Quaternion()));

    }
    mHasEnemy = false; 
}
void PlayerAIAgent::decision(double time_diff) {
    static bool flag = 1; 
    if (flag) {
        mNxtArea = 0; 
        mOnWay = true;
        flag = 0; 
    }
}
void PlayerAIAgent::onUpdate(double time_diff) {
    
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
    //只要没有障碍物，就可以攻击。
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
    Monster* enemy = dynamic_cast<Monster*>(c->getNode());
    if (enemy != nullptr) {
        mThreat = true; 
        mThreatTime = THREAT_COOL_TIME; 
        return; 
    }
    //炮友!
    Alien * gun_friend = dynamic_cast<Alien *>(c->getNode());
    if (gun_friend != nullptr) {
       // gun_friend->get
    }
    
}
