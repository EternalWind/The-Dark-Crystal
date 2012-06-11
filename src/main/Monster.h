#ifndef THE_DARK_CRYSTAL_MONSTER 
#define THE_DARK_CRYSTAL_MONSTER 

#include "Entity.h"

/**
  * ����ʵ����
  */
class Monster : public Entity {
	Q_OBJECT

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

	/**
	  * ���ع��޹�����Χ
	  * @returns ���޹�����Χ
	  */
	float getAttackRange();

	/**
	  * ���ù��޹�����Χ
	  * @param attack_range ���޹�����Χ
	  */
	void setAttackRange(float attack_range);

	virtual void onInitialize();

	virtual void onDeinitialize();

	/**
	  * Monster�๹�캯��
	  * @param node_name Node�ڵ�����
	  * @param mesh_handle ģ�;��
	  * @param collision_shape_type ��������
	  * @param mass ��������
	  * @param walk_sound_handle �������ߵ���Ч���
	  * @param jump_sound_handle ������Ծ����Ч���
	  * @param run_sound_handle �����ܲ�����Ч���
	  * @param attck_sound_handle ���޹�������Ч���
	  * @param attack_value ���޹�����
	  * @param attack_range ���޹�����Χ
	  */
	Monster(const QString node_name,
			const QString mesh_handle,
			const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
			const btScalar mass,
			const QString walk_sound_handle, 
			const QString jump_sound_handle, 
			const QString run_sound_handle,
			const QString attack_sound_handle,
			uint16_t attack_value,
			float attack_range
		);


protected slots:	   

	void __onMove(MoveType type, bool is_pressed);

    void __onJump(bool is_pressed);

    void __onAttack(bool is_pressed);

    void __onSpeedUp(bool is_pressed);

    //void __onChangeWeapon(Weapon::WeaponType type);

    //void __onRemoveWeapon();

    //void __onRemoveWeapon(Weapon::WeaponType type);

    //void __onAddEquipment(bool is_pressed);

    //void __onEquiped(dt::PhysicsBodyComponent* object);

    //void __onGetOffVehicle();

    void __onLookAround(Ogre::Quaternion quaternion);

    //void __onReload();

	void __onHit(dt::PhysicsBodyComponent* component);


protected:
	uint16_t mAttackValue;      //!< ���޹�����
	float	mAttackRange;       //!< ���޹�����Χ
	QString mWalkSoundHandle;   //!< ������·�������
	QString mJumpSoundHandle;   //!< ������Ծ�������
	QString mRunSoundHandle;    //!< �����ܲ���Ч���
	QString mAttackSoundHandle; //!< ���﹥���������

	const static QString WALK_SOUND_COMPONENT;   //!< ���Ź���������Ч��SoundComponent������
	const static QString JUMP_SOUND_COMPONENT;   //!< ���Ź�����Ծ��Ч��SoundComponent������
	const static QString RUN_SOUND_COMPONENT;    //!< ���Ź����ܶ���Ч��SoundComponent������
	const static QString ATTACK_SOUND_COMPONENT; //!< ���Ź��޹�����Ч��SoundComponent������
	const static QString INTERACTOR_COMPONENT;   //!< ���޹���������Component����

};

#endif