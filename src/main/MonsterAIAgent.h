#ifndef THE_DARK_CRYSTAL_MONSTERAIAGENT
#define THE_DARK_CRYSTAL_MONSTERAIAGENT

#include "Agent.h"
#include "Monster.h"
#include <Logic/TriggerAreaComponent.hpp>
#include <Logic/InteractionComponent.hpp>

/**
  * Monster的AI实现类
  */
class MonsterAIAgent : public Agent {
Q_OBJECT
public:
     /**
       * 怪物类型，由场景编辑器载入的怪物类型一律是GUARD.        
       */	    
     enum MonsterType {
        GUARD,
        PATROL
        };
     MonsterAIAgent(QString name, MonsterAIAgent::MonsterType type = GUARD);

     void fixTurn(double & d_degree);
     void fixDegree(double & degree);
     void lookAround(double d_degree, double time_diff, double speed);
     double clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre); 
     bool isThreat();
     void setThreat(bool type); 
     void setBody(Monster * body);
     /** 
       *当在威胁状态下的时候，寻找一个威胁方位并向它扑过去，在攻击范围的时候就攻击
       */
     void findAndAttack(double time_diff);  
     void onInitialize();
     void onDeinitialize();
     void onUpdate(double time_diff);
     void walk(double time_diff); 
     void decision(double time_diff);
     void test();
     void onTriggerr(Character * c);   
     double getPreDegree();
private slots:
     void __onFind(dt::PhysicsBodyComponent* pbc);
private:
    bool mSpeedUpPress;
    double mPreDegree;
    bool   mOnMovePress;
    bool   mAttackPress;
    double mExpectDegree;
    double mThreatTime;   
    std::pair<uint16_t, uint16_t> mNxtArea;                  //<! 若有在路上这个状态，则代表目标地所在ID。 
    MonsterType mType;                                       //!< 怪物类型
    Monster * mBody;                                         //!< 怪物身体
    bool mThreat;                                            //!< 是否受到威胁  当周围一定范围内出现对方时，由thickComponent触发	
    bool mOnWay;                                             //!< 是否在路上
    uint16_t mColli;
    bool mHasEnemy;                                          //<! 当前方向是否有敌人。    
    
    dt::InteractionComponent * mIteractor;                   //<! 检验前方是否有怪物的component。
    dt::TriggerAreaComponent * mTrigger;                     //<! 检验是否触发trigger的component。  
    const static double  THREAT_COOL_TIME;
    const static QString INTERACTOR_COMPONENT;               //!< 检测前方是否有攻击物体的InteractorComponent的名字。
    const static QString TRIGGER_AREA_COMPONENT;             //!< TRIGGER_AREA_COMPONENT的名字。
    const static double eps;                                 //!< 精度控制。
    const static double MOVE_ROTATE_SPEED;                   //!< 行走旋转速度
    const static double GUARD_ROTATE_SPEED;                  //!< 警戒旋转速度
    const static double PI;                                  //!< PI
    const static double ROTATE_FLOAT;                        //!< 只要在旋转浮动之内则可以向前走。
    const static double ENTER_SCOPE;                         //!< 当当前位置离目标位置的距离小于该值，则可以认为已经走进了该区域。
    const static double GUARD_RANGE;
    const static double AVOID_COLLI_RANGE;
};
	

#endif