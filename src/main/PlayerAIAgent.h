#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"
#include "Alien.h"
#include <Logic/TriggerAreaComponent.hpp>

class PlayerAIAgent : public Agent {
Q_OBJECT
public:
    PlayerAIAgent(); 
	bool isOnWay(); 
	void setOnWay(bool type); 	
	Alien* getBody(); 
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
	void initialize();
private slots:
    void __onFire(dt::PhysicsBodyComponent* pbc); 
    void __onTrigger(dt::TriggerAreaComponent* tac, dt::Component* c);
private:
    bool mFollow;                                            //<! 若为true，则快速行走到玩家身边
	Alien* mBody;                                            //<! 外星人身体，为agent的父节点，agent充当着控制外星人的大脑。
	bool mOnWay;                                             //<! 是否在路上。若做出了移动决策，在没有到达目的地之前，这个状态都为true。
	bool mThreat;                                            //<! 由thickComponent触发，当受到威胁的时候，进入guard状态
	double mThreatTime;                                      //<! 当受到威胁开始每次update，威胁系数不断冷却。
    bool mHasEnemy;                                          //<! 警戒状态下当前方向是否有敌人。
	uint16_t mCurArea;                                       //<! 当前所在区域ID。
	uint16_t mNxtArea;                                       //<! 若有在路上这个状态，则代表目标地所在ID。   

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