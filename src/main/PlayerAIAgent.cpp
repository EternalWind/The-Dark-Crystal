#include "PlayerAIAgent.h"
#include "AIDivideAreaManager.h"
#include "EntityManager.h"
#include "Monster.h"
#include <Logic/RaycastComponent.hpp>

const QString PlayerAIAgent::INTERACTOR_COMPONENT = "Player_AI_Agent_Interactor";
const QString PlayerAIAgent::TRIGGER_AREA_COMPONENT = "Player_AI_TRIGGER_AREA_COMPONENT";
const double  PlayerAIAgent::THREAT_COOL_TIME = 2.0;
const double  PlayerAIAgent::eps = 1e-4;

const double  PlayerAIAgent::MOVE_ROTATE_SPEED = 270;
const double  PlayerAIAgent::GUARD_ROTATE_SPEED = 90;
const double  PlayerAIAgent::PI = acos(-1.0);
const double  PlayerAIAgent::ROTATE_FLOAT = 6.0; 
const double  PlayerAIAgent::ENTER_SCOPE = 3;


PlayerAIAgent::PlayerAIAgent(QString name): Agent(name) {    
    //��ʼ�����κ�״̬
    mFollow = mThreat = mOnWay = false;     
    mPreDegree = 0;        
    mOnMovePress = false; 
    mFollow = true; 
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

void PlayerAIAgent::lookAround(double d_degree, double time_diff, double speed) {
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
double PlayerAIAgent::clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre) {
    nxt.y = pre.y = 0;
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 
    //��Ŀ��λ�ú͵�ǰλ��������������������
    Ogre::Vector3 dv = pre - nxt; 
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

void PlayerAIAgent::walk(double time_diff) {      
   //[-180,180]    
    double cur_degree;    
   //����һ�½Ƕȣ�ʹ��һֱ��[-180,180]��
    fixDegree(mPreDegree);
    //ָ��z���������������
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 

    Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);    
    Ogre::Vector3 pre_position = mBody->getPosition(); 
    pre_position.y = nxt_area_position.y = 0;
    //���Ŀ���Ѿ������ֹͣ�߶����˳���
    if (nxt_area_position.distance(pre_position) < ENTER_SCOPE) {
            mOnWay = false;     
            if (mOnMovePress) {
                emit(sMove(Entity::STOP, true));                 
                mOnMovePress = 0; 
            }
            return;
    }

    mExpectDegree = clacDegree(nxt_area_position, pre_position);
   
    
    /*std::cout << pre_position.x << ' ' << pre_position.y << ' ' << pre_position.z << endl; 
    std::cout << nxt_area_position.x << ' ' << nxt_area_position.y << ' ' << nxt_area_position.z << endl; 
    std::cout << mExpectDegree << endl; 
    std::cout << mPreDegree << endl; */
    double d_degree = mExpectDegree - mPreDegree;

   

    fixTurn(d_degree);
  
  /*  std::cout << mExpectDegree << endl; 
    std::cout << mPreDegree << endl; 
    std::cout << d_degree << endl; */

    //��ǰ֡����Ѿ��ڽǶȷ����ڣ���ʼ�߶���
    if (fabs(d_degree) < ROTATE_FLOAT) { 
        if (!mOnMovePress) { 
            //emit(sMove(Entity::STOP, true));
           // emit(sSpeedUp(true));
            mBody->setCurSpeed(12.0);
            emit(sMove(Entity::FORWARD, true)); 
            mOnMovePress = 1; 
        }
      } else{        
      //  emit(sMove(Entity::STOP, true));   
          lookAround(d_degree, time_diff, MOVE_ROTATE_SPEED);      
    }
}

void PlayerAIAgent::guard(double time_diff) {
    //����ǰ����û�����࣬�о�ͳͳ����
    this->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();

    if (!mHasEnemy) {        
       fixDegree(mPreDegree);
       double d_degree = mExpectDegree - mPreDegree;
       fixTurn(d_degree);      
       lookAround(d_degree, time_diff, GUARD_ROTATE_SPEED);   
       /*std::cout << d_degree << endl; 
       std::cout << mPreDegree << endl; 
       std::cout << mExpectDegree << endl; */
    }
    mHasEnemy = false; 
}
void PlayerAIAgent::decision(double time_diff) {   
    
    
    //������Ƿ����˸������
    if (mFollow) {
        Ogre::Vector3 cur_pos = mBody->getPosition(); 
        uint16_t cur_id = AIDivideAreaManager::get()->getIdByPosition(cur_pos); 
        Ogre::Vector3 human_pos = EntityManager::get()->getHuman()->getPosition(); 
        
        uint16_t des_id = AIDivideAreaManager::get()->getIdByPosition(human_pos); 
        uint16_t diff = AIDivideAreaManager::get()->getAreaNumBetween(cur_id, des_id); 
        if (diff > 0) {
            uint16_t nxt_id = AIDivideAreaManager::get()->getNxtClosestId(cur_id, des_id);                
                
            std::pair<uint16_t, uint16_t> tmp = AIDivideAreaManager::get()->randomPosition(nxt_id);

          /*  std::cout << cur_id << endl; 
            std::cout << des_id << endl; 
            std::cout << nxt_id << endl; 
            std::cout << cur_pos.x << ' ' << cur_pos.z << endl; 
            std::cout << AIDivideAreaManager::get()->getPositionById(tmp).x <<  ' ' <<
                AIDivideAreaManager::get()->getPositionById(tmp).z << endl;
*/
            dt::Logger().get().debug(this->getBody()->getName());
            if (tmp.first != -1) {     
                    AIDivideAreaManager::get()->destroy(mNxtArea);
                    mNxtArea = tmp;
                    mOnWay = true; 
                    return; 
            }
        }        
    }
    
    //����ԭ�ؾ��䡣
    mThreat = true; 
    mThreatTime = THREAT_COOL_TIME;   
    
}
void PlayerAIAgent::onUpdate(double time_diff) {
      
    if (time_diff == 0.0)  {
        return; 
    }

    //����״̬�£�����״̬����Ϊ�е��˳����ھ�������
    //�������ж����ھ�������Ϊ�˷�ֹ������ײ�����ò�ͬ�ľ���ʱ�䡣
    if (mThreat) {
        mThreatTime -= time_diff; 
        if (mThreatTime <= eps) {
            mThreat = false; 
        }
        guard(time_diff); 
    } else if (mOnWay) { //�����ߣ�����֮��
        walk(time_diff); 
    } else decision(time_diff);  // ���򣬾���֮��

    dt::Node::onUpdate(time_diff);
}

void PlayerAIAgent::onInitialize() {

    setBody(dynamic_cast<Alien *>(this->getParent()));

    mIteractor = this->addComponent<dt::InteractionComponent>(
        new dt::RaycastComponent(INTERACTOR_COMPONENT)).get();
    
    mIteractor->setRange(3000.0f);    
    connect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
            this, SLOT(__onFire(dt::PhysicsBodyComponent*)));

    mTrigger = this->addComponent<dt::TriggerAreaComponent>(new dt::TriggerAreaComponent(
        new btBoxShape(btVector3(20.0f, 20.0f, 20.0f)), TRIGGER_AREA_COMPONENT)).get();
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
       // std::cout << "mHasEnemy " << mHasEnemy << endl; 
     } else {
        emit(sAttack(false));        
     }
}

void PlayerAIAgent::__onTrigger(dt::TriggerAreaComponent* tac, dt::Component* c) {
    //����
    Monster* enemy = dynamic_cast<Monster*>(c->getNode());
    if (enemy != nullptr) {
        mThreat = true; 
        mThreatTime = THREAT_COOL_TIME; 

        mExpectDegree = clacDegree(enemy->getPosition(), mBody->getPosition());
        if (mOnMovePress) {
            mOnMovePress = 0; 
            emit(sMove(Entity::STOP, true));
        }
       /* Ogre::Vector3 ep = enemy->getPosition(); 
        Ogre::Vector3 ap = mBody->getPosition();
        std::cout << "enemy " << ep.x << ' ' << ep.z << endl; 
        std::cout << "alien " << ap.x << ' ' << ap.z << endl; 
        std::cout << "degree " << mExpectDegree << endl; */
        return; 
    }
    //����!
    Alien * gun_friend = dynamic_cast<Alien*>(c->getNode());
  //  dt::Logger().get().debug(c->getNode()->getName());
    
    if (gun_friend != nullptr) {
       // gun_friend->get
       // std::cout << "RJ" << mThreat << endl; 
       //if (!mThreat)
       // {
       //     mThreat = true; 
       //     if (mOnMovePress) {
       //         mOnMovePress = 0; 
       //         emit(sMove(Entity::STOP, true)); 
       //         mExpectDegree = mPreDegree + 180;
       //     }
       //     //�������Ѿ������ȴʱ���ֹ��ײ��
       //     mThreatTime = (rand()) % 10;
       // }
    }    
}
