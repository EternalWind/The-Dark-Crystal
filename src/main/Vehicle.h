#ifndef THE_DARK_CRYSTAL_VEHICLE
#define THE_DARK_CRYSTAL_VEHICLE

#include "Entity.h"

/**
  * 载具类, 游戏中出现的载具
  */
class Vehicle : public Entity {

	Q_OBJECT

public:
	
	/**
	  * 返回载具攻击力
	  * @returns 载具攻击力
	  */
	uint16_t getAttackValue();

	/**
	  * 设置载具攻击力
	  * @param attack_value 载具攻击力
	  */
	void setAttackValue(uint16_t attack_value);

	/** 
	  * 返回载具攻击范围
	  * @returns 载具攻击范围
	  */
	float getAttackRange();

	/**
	  * 设置载具攻击范围
	  * @param attack_range 载具攻击范围
	  */
	void setAttackRange(float attack_range);


protected:
	uint16_t mAttackValue;        //!< 攻击力
	float mAttackRange;           //!< 攻击范围

};

#endif