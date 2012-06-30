#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"

#include <map>

/**
  * 游戏载具之一：宇宙飞船类
  */
class Spaceship : public Vehicle {

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
	  * @param attack_sound_handle 载具攻击声音句柄
	  * @param flying_sound_handle 飞船飞行声音句柄
	  * @param rise_sound_handle 飞船上升声音句柄
	  * @param fall_sound_handle 飞船下降声音句柄
	  * @param max_speed 飞船最大的直线速度 (注意用2-Base以避免精度损失)
	  * @param speed_per_frame 飞机加速时每一帧的速度的增量(加速度)
	  * @param parallel_move_speed 飞机平移时的速度
	  * @param up_down_speed 飞机上升下降的速度
	  */
	Spaceship(const QString node_name, 
		  const QString mesh_handle, 
	      const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
		  const btScalar mass,
		  const uint16_t attack_value,
		  const float attack_range,
		  const float attack_interval,
		  const QString attack_sound_handle,
		  const QString flying_sound_handle,
		  const QString rise_sound_handle,
		  const QString fall_sound_handle,
		  const float max_speed,
		  const float speed_per_frame,
		  const float parallel_move_speed,
		  const float up_down_speed
		  );

	void onInitialize();

	void onDeinitialize();

	void onUpdate(double time_diff);

	/**
	  * 增加火焰效果
	  * @param name 火焰名字
	  * @param flame_name 火焰材料的名字
	  * @param position 火焰位置
	  * @param rotation 火焰旋转
	  */
	void addFlame(const QString& name, const QString& flame_name, Ogre::Vector3 position, Ogre::Vector3 direction);

	/**
	  * 发射火焰效果
	  * @param name 火焰的名字
	  */
	void playFlame(const QString& name);

	/**
	  * 停止发射火焰效果
	  * @param name 火焰的名字
	  */
	void stopFlame(const QString& name);


protected slots:
	void __onMove(MoveType type, bool is_pressed);

	/**
	  * 这里的SpeedUp用来作为飞机的下降
	  */
    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

	void __onJump(bool is_pressed);

	void __onGetOffVehicle();

protected:
	QString mFlyingSoundHandle;                    //!< 飞机飞行声音句柄
	QString mRiseSoundHandle;                      //!< 飞机上升声音句柄
	QString mFallSoundHandle;                      //!< 飞机下降声音句柄

public:
	const static QString FLYING_SOUND_COMPONENT;   //!< 飞机飞行声音Component的名字
	const static QString RISE_SOUND_COMPONENT;     //!< 飞机上升声音Component的名字
	const static QString FALL_SOUND_COMPONENT;     //!< 飞机下降声音Component的名字

private:
	const static float MAX_LEAN_ANGLE;             //!< 飞机平移时最大的倾斜角
	const static float ANGLE_PER_MOVE;             //!< 飞机每一帧转动的角度
	float mCurAngle;                               //!< 飞机当前倾斜角度      
	float mMaxSpeed;                               //!< 飞机飞行最大速度
	float mSpeedPerFrame;                          //!< 飞机加速时每一帧的速度的增量(加速度)
	float mParallelMoveSpeed;                      //!< 飞机平移时的速度
	float mUpDownSpeed;                            //!< 飞机上升下降的速度
};

#endif