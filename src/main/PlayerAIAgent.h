#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"
#include "Alien.h"

class PlayerAIAgent : public Agent {
Q_OBJECT
public:
	bool isOnWay(); 
	void setOnWay(bool type); 	
	Alien* getBody(); 
	void setBody(Alien* body); 
	/**
      * 若是mOnWay这个状态为true，则行走。
      */	
	void walk(); 
	/**
      * 若mThreat状态为true，则在原地警戒，击杀怪物。        
      */	
	void guard(); 
	/**
      * 当没有任何状态为true的时候，要做出决策选择移动还是原地警戒。     
      */	
	void decision(); 
	void onUpdate(double time_diff);
	void initialize();
private:
	Alien* mBody;                          //<! 外星人身体，为agent的父节点，agent充当着控制外星人的大脑。
	bool mOnWay;                            //<! 是否在路上。若做出了移动决策，在没有到达目的地之前，这个状态都为true。
	bool mThreat;                           //<! 由thickComponent触发，当受到威胁的时候，进入guard状态
	uint16_t mThreatCoolNum;                //<! 当受到威胁开始每次update，威胁系数不断冷却。
	uint16_t mCurArea;                      //<! 当前所在区域ID
	uint16_t mNxtArea;                      //<! 若有在路上这个状态，则代表目标地所在ID。
};
#endif