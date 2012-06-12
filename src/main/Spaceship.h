#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"

class Plain : public Vehicle {
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
	  */
	Plain(const QString node_name, 
		  const QString mesh_handle, 
	      const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
		  const btScalar mass,
		  const uint16_t attack_value,
		  const float attack_range,
		  const float attack_interval);

	void onInitialize();

	void onDeinitialize();

protected slots:
	void __onMove(MoveType type, bool is_pressed);

    void __onAttack(bool is_pressed);

    void __onSpeedUp(bool is_pressed);

    void __onGetOffVehicle();

    void __onLookAround(Ogre::Quaternion quaternion);

	void __onHit(dt::PhysicsBodyComponent* hit);

protected:

};

#endif