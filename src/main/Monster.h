#ifndef THE_DARK_CRYSTAL_MONSTER 
#define THE_DARK_CRYSTAL_MONSTER 

#include "Character.h"

/**
  * 怪兽实体类
  */
class Monster : public Character {
	Q_OBJECT

public:
	/**
	  * 返回怪兽的攻击力
	  * @returns 怪兽的攻击力
	  */
	uint16_t getAttackValue(); 

	/**
	  * 设置怪兽的攻击力
	  * @param attack_value 怪兽的攻击力
	  */
	void setAttackValue(uint16_t attack_value);

	/**
	  * 返回怪兽攻击范围
	  * @returns 怪兽攻击范围
	  */
	float getAttackRange();

	/**
	  * 设置怪兽攻击范围
	  * @param attack_range 怪兽攻击范围
	  */
	void setAttackRange(float attack_range);

	virtual void onInitialize();

	virtual void onDeinitialize();

	/**
	  * Monster类构造函数
	  * @param node_name Node节点名字
	  * @param mesh_handle 模型句柄
	  * @param collision_shape_type 物理外形
	  * @param mass 怪兽质量
	  * @param walk_sound_handle 怪兽行走的音效句柄
	  * @param jump_sound_handle 怪兽跳跃的音效句柄
	  * @param run_sound_handle 怪兽跑步的音效句柄
	  * @param attck_sound_handle 怪兽攻击的音效句柄
	  * @param attack_value 怪兽攻击力
	  * @param attack_range 怪兽攻击范围
	  * @param attack_interval 怪兽攻击间隔时间
	  */
	Monster(const QString node_name,
			const QString mesh_handle,
			const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
			const btScalar mass,
			const QString walk_sound_handle, 
			const QString jump_sound_handle, 
			const QString run_sound_handle,
			const QString attack_sound_handle,
			const uint16_t attack_value,
			const float attack_range, 
			const float attack_interval
		);

	void onUpdate(double time_diff);

	void onKilled();

protected slots:	   

	//void __onMove(MoveType type, bool is_pressed);

    //void __onJump(bool is_pressed);

    void __onAttack(bool is_pressed);

    //void __onSpeedUp(bool is_pressed);

    void __onChangeWeapon(Weapon::WeaponType type);

    void __onRemoveWeapon();

    void __onRemoveWeapon(Weapon::WeaponType type);

    void __onAddEquipment(bool is_pressed);

    void __onEquiped(dt::PhysicsBodyComponent* object);

    void __onGetOffVehicle();

    void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

    void __onReload();

	void __onHit(dt::PhysicsBodyComponent* hit);


protected:
	uint16_t mAttackValue;      //!< 叫兽攻击力
	float mAttackRange;         //!< 叫兽攻击范围
	float mAttackInterval;      //!< 叫兽攻击间隔时间
	QString mAttackSoundHandle; //!< 怪物攻击声音句柄
	bool mIsAttacking;          //!< 怪兽是否正在攻击
public:
	const static QString ATTACK_SOUND_COMPONENT; //!< 播放怪兽攻击音效的SoundComponent的名字
	const static QString INTERACTOR_COMPONENT;   //!< 怪兽攻击交互的Component名字

};

#endif