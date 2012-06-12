#ifndef THE_DARK_CRYSTAL_VEHICLE
#define THE_DARK_CRYSTAL_VEHICLE

#include "Entity.h"

/**
  * �ؾ���, ��Ϸ�г��ֵ��ؾ�
  */
class Vehicle : public Entity {

	Q_OBJECT

public:
	
	/**
	  * �����ؾ߹�����
	  * @returns �ؾ߹�����
	  */
	uint16_t getAttackValue();

	/**
	  * �����ؾ߹�����
	  * @param attack_value �ؾ߹�����
	  */
	void setAttackValue(uint16_t attack_value);

	/** 
	  * �����ؾ߹�����Χ
	  * @returns �ؾ߹�����Χ
	  */
	float getAttackRange();

	/**
	  * �����ؾ߹�����Χ
	  * @param attack_range �ؾ߹�����Χ
	  */
	void setAttackRange(float attack_range);


protected:
	uint16_t mAttackValue;        //!< ������
	float mAttackRange;           //!< ������Χ

};

#endif