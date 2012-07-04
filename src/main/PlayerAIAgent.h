#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"
#include "Alien.h"
#include <Logic/TriggerAreaComponent.hpp>
#include <QString>
#include <Logic/InteractionComponent.hpp>

class PlayerAIAgent : public Agent {
Q_OBJECT
public: 
    PlayerAIAgent(QString name); 
    bool isOnWay(); 
    void setOnWay(bool type); 	
    Alien* getBody(); 
    void fixTurn(double & d_degree);
    void fixDegree(double & degree);
    void lookAround(double d_degree, double time_diff, double speed);
    double clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre); 
    void setBody(Alien* body);       
    /**
      * 若是mOnWay这个状态为true，则行走。
      */	
    void walk(double time_diff); 
    /**
      * 若mThreat状态为true，则在原地警戒，击杀怪物。        
      */	
    void guard(double time_diff); 
    /**
      * 当没有任何状态为true的时候，要做出决策选择移动还是原地警戒。     
      */	
    void decision(double time_diff); 
    void onUpdate(double time_diff);
    void onInitialize();
    void onDeinitialize();
    dt::PhysicsBodyComponent*  test();
    void __onTrigger(Character * c);
private slots:
    void __onFire(dt::PhysicsBodyComponent* pbc); 
    
private:
    bool mSpeedUp; 
    double mPreDegree;                                       //<! 存储上一帧身体的朝向。
    double mExpectDegree;                                    //<! 期望的朝向。
    bool mAttack;                                            //<! 是否处于攻击状态。
    bool mFollow;                                            //<! 若为true，则快速行走到玩家身边
    Alien* mBody;                                            //<! 外星人身体，为agent的父节点，agent充当着控制外星人的大脑。
    Alien* mHumanBody;                                       //<! 玩家指针
    bool mOnWay;                                             //<! 是否在路上。若做出了移动决策，在没有到达目的地之前，这个状态都为true。
    bool mThreat;                                            //<! 由thickComponent触发，当受到威胁的时候，进入guard状态
    double mThreatTime;                                      //<! 当受到威胁开始每次update，威胁系数不断冷却。
    bool mHasEnemy;                                          //<! 警戒状态下正前方是否有敌人。
    std::pair<uint16_t, uint16_t> mNxtArea;                                       //<! 若有在路上这个状态，则代表目标地所在ID,否则为当前所在地的ID。  
    dt::InteractionComponent * mIteractor;                   //<! 检验前方是否有怪物的component。
    dt::TriggerAreaComponent * mTrigger;                     //<! 检验是否触发trigger的component。
    bool mOnMovePress;                                       
    const static QString TRIGGER_AREA_COMPONENT;             //!< TRIGGER_AREA_COMPONENT的名字。
    const static QString INTERACTOR_COMPONENT;               //!< 检测前方是否有攻击物体的InteractorComponent的名字。
    const static double THREAT_COOL_TIME;                    //!< 威胁解除时间。
    const static double eps;                                 //!< 精度控制。
    const static double MOVE_ROTATE_SPEED;                   //!< 行走旋转速度
    const static double GUARD_ROTATE_SPEED;                  //!< 警戒旋转速度
    const static double PI;                                  //!< PI
    const static double ROTATE_FLOAT;                        //!< 只要在旋转浮动之内则可以向前走。
    const static double ENTER_SCOPE;                         //!< 当当前位置离目标位置的距离小于该值，则可以认为已经走进了该区域。

};
#endif