#ifndef THE_DARK_CRYSTAL_MONSTERAIAGENT
#define THE_DARK_CRYSTAL_MONSTERAIAGENT

#include "Agent.h"
/**
  *  ע��
  */
class MonsterAIAgent : public Agent {
Q_OBJECT
public:
	/**
  * ע��
  * @param ����
  * @returns ����ֵ
  */	
	bool isThreat();
	void setThreat(bool type); 
	 /* ������в״̬�µ�ʱ��Ѱ��һ����в��λ�������˹�ȥ���ڹ�����Χ��ʱ��͹���
  * @param 
  * @returns 
  */
	void findAndAttack();  
private:
	
	bool mThreat;    //!< �Ƿ��ܵ���в  ����Χһ����Χ�ڳ��ֶԷ�ʱ����thickComponent����	

};
	

#endif