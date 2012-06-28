#include "MonsterAIAgent.h"
#include "AIDivideAreaManager.h"
#include "Monster.h"
#include "EntityManager.h"

#include <Logic/RaycastComponent.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include "Alien.h"
#include <Utils/Utils.hpp>
const double MonsterAIAgent::ENTER_SCOPE = 6.0;
const double  MonsterAIAgent::THREAT_COOL_TIME = 2.0;
const QString MonsterAIAgent::INTERACTOR_COMPONENT = "Monster_INTERACTOR_COMPONENT";
const QString MonsterAIAgent::TRIGGER_AREA_COMPONENT = "Monster_TRIGGER_AREA_COMPONENT";
const double  MonsterAIAgent::eps = 1e-4;
const double  MonsterAIAgent::MOVE_ROTATE_SPEED = 180;
const double  MonsterAIAgent::GUARD_ROTATE_SPEED = 90;
const double  MonsterAIAgent::PI = acos(-1.0);
const double  MonsterAIAgent::ROTATE_FLOAT = 90.0; 

MonsterAIAgent::MonsterAIAgent(QString name, MonsterAIAgent::MonsterType type) : mType(type), Agent(name){
    this->mHasEnemy = this->mThreat = this->mOnWay = false;   
    mPreDegree = 0;
    mPrePosition.z = -10000;
    mOnMovePress = false;
}

bool MonsterAIAgent::isThreat() {
    return mThreat;
}
void MonsterAIAgent::setThreat(bool type) {
    mThreat = type; 
}
void MonsterAIAgent::findAndAttack(double time_diff) {
    //this->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();

     if (!mHasEnemy) {        
       fixDegree(mPreDegree);
       double d_degree = mExpectDegree - mPreDegree;
       fixTurn(d_degree);      
       lookAround(d_degree, time_diff, GUARD_ROTATE_SPEED);   

       //std::cout << mPreDegree << endl; 
    }
    mHasEnemy = false; 

}
void MonsterAIAgent::onInitialize() {   
     
    setBody(dynamic_cast<Monster *>(this->getParent()));    
    
    //mIteractor = this->addComponent<dt::InteractionComponent>(
    //    new dt::RaycastComponent(INTERACTOR_COMPONENT)).get();
    //
    //mIteractor->setRange(3000.0f);    
    //if (!QObject::connect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
    //        this, SLOT(__onFind(dt::PhysicsBodyComponent*))) ) {
    //            dt::Logger::get().error("can't connect interactionComponent to MonsterAIAgent's __Onfind");
    //}    

    //mTrigger = this->addComponent<dt::TriggerAreaComponent>(new dt::TriggerAreaComponent(
    //    new btBoxShape(btVector3(20.0f, 20.0f, 20.0f)), TRIGGER_AREA_COMPONENT)).get();
    //dt::Logger::get().debug(mTrigger->getFullName());
    //if (!QObject::connect(mTrigger, SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
    //        this, SLOT(onTriggerr(dt::TriggerAreaComponent*, dt::Component*))) ) {
    //         dt::Logger::get().error("can't connect triggerAreaComponent to MonsterAIAgent's __onTrigger");
    //}
}
void MonsterAIAgent::onDeinitialize() {
   /* disconnect(mIteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)),
            this, SLOT(__onFind(dt::PhysicsBodyComponent*)));    
    disconnect(mTrigger, SIGNAL(triggered(dt::TriggerAreaComponent*, dt::Component*)), 
            this, SLOT(onTriggerr(dt::TriggerAreaComponent*, dt::Component*)));*/
}
void MonsterAIAgent::setBody(Monster* body) {
	mBody = body;
}
void MonsterAIAgent::walk(double time_diff) {   
   //[-180,180]    
    double cur_degree;    
   //����һ�½Ƕȣ�ʹ��һֱ��[-180,180]��
    fixDegree(mPreDegree);
    //ָ��z���������������
    Ogre::Vector3 dy = Ogre::Vector3(0, 0, 1); 

     
    Ogre::Vector3 pre_position = mBody->getPosition(); 
   
    pre_position.y =0;


    /* if (fabs(mPrePosition.x - pre_position.x)== 0.0 && fabs(mPrePosition.z - pre_position.z) == 0.0) {
        
        uint16_t pre_id = AIDivideAreaManager::get()->getIdByPosition(mPrePosition);
        std::pair<uint16_t, uint16_t> tmp = AIDivideAreaManager::get()->randomPosition(pre_id);
         AIDivideAreaManager::get()->destroy(mNxtArea);
         mNxtArea = tmp;
          
                    
    }*/
     Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);   
     nxt_area_position.y = 0;
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
            emit(sSpeedUp(true));
            mBody->setCurSpeed(4.0);
            emit(sMove(Entity::BACKWARD, true)); 
            mOnMovePress = 1; 
            mPrePosition = pre_position;
        }
      } else{        
      //  emit(sMove(Entity::STOP, true));   
          lookAround(d_degree, time_diff, MOVE_ROTATE_SPEED);      
    }
}
void MonsterAIAgent::onUpdate(double time_diff) {

   
    // update�����ӽڵ��update������component��    
    dt::Node::onUpdate(time_diff);  
    
    if (time_diff == 0.0) return; 
    
     //����״̬�£�����״̬����Ϊ�е��˳����ھ�������
    //�������ж����ھ�������Ϊ�˷�ֹ������ײ�����ò�ͬ�ľ���ʱ�䡣
    if (mThreat) {
        mThreatTime -= time_diff; 
        if (mThreatTime <= eps) {
            mThreat = false; 
        }
        findAndAttack(time_diff); 
    } else if (mOnWay) { //�����ߣ�����֮��
        walk(time_diff); 
    } else decision(time_diff);  // ���򣬾���֮��
            
            
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

           /* std::cout << cur_id << endl; 
            std::cout << des_id << endl; 
            std::cout << nxt_id << endl; 
            std::cout << cur_pos.x << ' ' << cur_pos.z << endl; 
            std::cout << AIDivideAreaManager::get()->getPositionById(tmp).x <<  ' ' <<
                AIDivideAreaManager::get()->getPositionById(tmp).z << endl;*/

            
            if (tmp.first != -1) {     
                    AIDivideAreaManager::get()->destroy(mNxtArea);
                    mNxtArea = tmp;
                    mOnWay = true; 
                    mPrePosition.z = -100000000000;
                    return; 
            }
        }  

          //����ԭ�ؾ��䡣
    mThreat = true; 
    mThreatTime = THREAT_COOL_TIME;   
}


void MonsterAIAgent::onTriggerr(dt::TriggerAreaComponent* trigger_area, dt::Component* component) {
    Alien* enemy = dynamic_cast<Alien*>(component->getNode());    
    if (enemy != nullptr) {
        mThreat = true; 
        return; 
    }
}
void MonsterAIAgent::__onFind(dt::PhysicsBodyComponent* pbc) {
    //Alien* enemy = dynamic_cast<Alien*>(pbc->getNode());
    //static bool fla = 1;
    // if (enemy != nullptr && fla) {
    //    emit(sMove(Entity::FORWARD, true));    
    //    mHasEnemy = true;
    //    fla = 0; 
    // } else {
    //    // fla = 1;
    //    // emit(sMove(Entity::FORWARD, false));    
    // }
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
    //��Ŀ��λ�ú͵�ǰλ��������������������
    Ogre::Vector3 dv = nxt - pre; 
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