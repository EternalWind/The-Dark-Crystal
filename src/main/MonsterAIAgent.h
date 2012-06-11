#ifndef THE_DARK_CRYSTAL_MONSTERAIAGENT
#define THE_DARK_CRYSTAL_MONSTERAIAGENT

#include "Agent.h"
/**
  *  注释
  */
class MonsterAIAgent : public Agent {
Q_OBJECT
public:
	/**
  * 注释
  * @param 参数
  * @returns 返回值
  */	
	bool isThreat();
	void setThreat(bool type); 
	 /* 当在威胁状态下的时候，寻找一个威胁方位并向它扑过去，在攻击范围的时候就攻击
  * @param 
  * @returns 
  */
	void findAndAttack();  
private:
	
	bool mThreat;    //!< 是否受到威胁  当周围一定范围内出现对方时，由thickComponent触发	

};
	

#endif