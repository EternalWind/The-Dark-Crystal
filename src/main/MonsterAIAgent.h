#ifndef THE_DARK_CRYSTAL_MONSTERAIAGENT
#define THE_DARK_CRYSTAL_MONSTERAIAGENT

#include "Agent.h"
#include "Monster.h"

/**
  * Monster��AIʵ����
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
	/** 
      *������в״̬�µ�ʱ��Ѱ��һ����в��λ�������˹�ȥ���ڹ�����Χ��ʱ��͹���
      */
	void findAndAttack();  
private:

	Monster * mBody;        //!< ��������
	bool mThreat;           //!< �Ƿ��ܵ���в  ����Χһ����Χ�ڳ��ֶԷ�ʱ����thickComponent����	

};
	

#endif