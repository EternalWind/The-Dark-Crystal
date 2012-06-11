#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"

class PlayerAIAgent : public Agent {
Q_OBJECT
public:
	bool isOnWay(); 
	void setOnWay(bool type); 
	/**
  * 若是在路上这个状态，则行走
  * @param 参数
  * @returns 返回值
  */	
	void walk(); 
	/**
  * 若触发警戒状态则在原地警戒，击杀怪物
  * @param 参数
  * @returns 返回值
  */	
	void guard(); 
	/**
  * 当没有任何状态的时候，要做出决策选择移动还是原地警戒
  * @param 参数
  * @returns 返回值
  */	
	void decision(); 
private:
	bool mOnWay; //<! 是否在路上。若做出了移动决策，在没有到达目的地之前，都是在路上这个状态
	bool mThreat; //<! 由thickComponent触发，当受到威胁的时候，进入guard状态
	uint16_t mThreatCoolNum;  //<! 当受到威胁开始每次update，威胁系数不断冷却。
	uint16_t mCurArea;  //<! 当前所在区域ID
	uint16_t mNxtArea;  //<! 若有在路上这个状态，则代表目标地所在ID。
};
#endif