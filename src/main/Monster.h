#ifndef THE_DARK_CRYSTAL_MONSTER 
#define THE_DARK_CRYSTAL_MONSTER 

#include "Entity.h"

/**
  * ����ʵ����
  */
class Monster : public Entity {
public:
	/**
	  * ���ع��޵Ĺ�����
	  * @returns ���޵Ĺ�����
	  */
	uint16_t getAttackValue(); 

	/**
	  * ���ù��޵Ĺ�����
	  * @param attack_value ���޵Ĺ�����
	  */
	void setAttackValue(uint16_t attack_value);

	virtual void onInitialize();

	virtual void onDeinitialize();


protected slots:	   

	void __onMove(MoveType type, bool is_pressed);

    void __onJump(bool is_pressed);

    void __onAttack(bool is_pressed);

    void __onSpeedUp(bool is_pressed);

    void __onChangeWeapon(Weapon::WeaponType type);

    void __onRemoveWeapon();

    void __onRemoveWeapon(Weapon::WeaponType type);

    void __onAddEquipment(bool is_pressed);

    void __onEquiped(dt::PhysicsBodyComponent* object);

    void __onGetOffVehicle();

    void __onLookAround(Ogre::Quaternion quaternion);

    void __onReload();


protected:
	uint16_t mAttackValue;	//!< ���޹�����
};

#endif