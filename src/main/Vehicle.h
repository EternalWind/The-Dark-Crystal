#ifndef THE_DARK_CRYSTAL_VEHICLE
#define THE_DARK_CRYSTAL_VEHICLE

#include "Entity.h"
#include "Prop.h"

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
	uint16_t getAttackValue() const;

	/**
	  * 设置载具攻击力
	  * @param attack_value 载具攻击力
	  */
	void setAttackValue(uint16_t attack_value);

	/** 
	  * 返回载具攻击范围
	  * @returns 载具攻击范围
	  */
	float getAttackRange() const;

	/**
	  * 设置载具攻击范围
	  * @param attack_range 载具攻击范围
	  */
	void setAttackRange(float attack_range);

	/**
	  * 构造函数
	  * @param node_name Node节点名字
	  * @param mesh_handle 模型句柄
	  * @param collision_shape_type 物理外形
	  * @param mass 载具质量
	  * @param attack_value 载具攻击力
	  * @param attack_range 载具攻击范围
	  * @param attack_interval 载具攻击时间间隔
	  * @param attack_sound_handle 载具攻击声音句柄
	  */
	Vehicle(const QString node_name, 
			const QString mesh_handle, 
			const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
			const btScalar mass,
			const uint16_t attack_value,
			const float attack_range,
			const float attack_interval,
			const QString attack_sound_handle);

	virtual void onInitialize();

    virtual void onDeinitialize();

protected slots:

	void __onAttack(bool is_pressed);

	void __onGetOffVehicle();

	void __onHit(dt::PhysicsBodyComponent* hit);

	void __onJump(bool is_pressed);

    void __onChangeWeapon(Weapon::WeaponType type);

    void __onRemoveWeapon();

    void __onRemoveWeapon(Weapon::WeaponType type);

    void __onAddEquipment(bool is_pressed);

    void __onEquiped(dt::PhysicsBodyComponent* object);

    void __onReload();

protected:
	uint16_t mAttackValue;                       //!< 攻击力
	float mAttackRange;                          //!< 攻击范围
	float mAttackInterval;                       //!< 攻击间隔时间
	QString mAttackSoundHandle;                  //!< 载具攻击声音句柄

public:
	static const QString ATTACK_SOUND_COMPONENT; //!< 载具攻击声音Component的名字
	static const QString INTERATOR_COMPONENT;    //!< 载具攻击交互Component的名字
};

#endif