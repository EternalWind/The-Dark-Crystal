#ifndef THE_DARK_CRYSTAL_CAR
#define THE_DARK_CRYSTAL_CAR

#include "Vehicle.h"

/**
  * 游戏载具之一：战车类
  */
class Car : public Vehicle {

	Q_OBJECT

public:

	/**
	  * 构造函数
	  * @param node_name Node节点名字
	  * @param mesh_handle 模型句柄
	  * @param collision_shape_type 物理外形
	  * @param mass 载具质量
	  * @param attack_value 载具攻击力
	  * @param attack_range 载具攻击范围
	  * @param attack_interval 载具攻击时间间隔
	  * @param move_sound_handle 战车移动声音句柄
	  * @param rush_sound_handle 战车加速声音句柄
	  * @param attack_sound_handle 战车攻击声音句柄
	  */
	Car(const QString node_name, 
		const QString mesh_handle, 
		const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
		const btScalar mass,
		const uint16_t attack_value,
		const float attack_range,
		const float attack_interval,
		const QString attack_sound_handle,
		const QString move_sound_handle,
		const QString rush_sound_handle);

	void onInitialize();

	void onDeinitialize();

protected slots:
	void __onMove(MoveType type, bool is_pressed);

    void __onAttack(bool is_pressed);

    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion quaternion);


protected:
	QString mMoveSoundHandle;                      //!< 战车移动声音句柄
	QString mRushSoundHandle;                      //!< 战车加速声音句柄
	
	static const QString MOVE_SOUND_COMPONENT;     //!< 战车移动声音Component的名字
	static const QString RUSH_SOUND_COMPONENT;     //!< 战车加速声音Component的名字
};

#endif