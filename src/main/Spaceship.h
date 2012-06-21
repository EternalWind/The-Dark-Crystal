#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"

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
          const float min_speed,
          const float acceleration
		  );

	void onInitialize();

	void onDeinitialize();

    void onUpdate(double time_diff);


protected slots:
	void __onMove(MoveType type, bool is_pressed);

	/**
	  * 这里的SpeedUp用来作为飞机的下降
	  */
    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion quaternion);

	void __onJump(bool is_pressed);


private:

    void __moveAround();


protected:
    Ogre::Quaternion mLookAroundQuaternion; //!< 飞机转动方向
    Ogre::Vector3 mCurDirection;            //!<  机头方向
    float mMaxSpeed;                        //!< 飞船最大速度
    float mMinSpeed;                        //!< 战车最低速度/向后最大速度
    float mAcceleration;                    //!< 飞船的加速度
    QString mAttackSoundHandle;             //!< 飞机攻击声音句柄
	QString mFlyingSoundHandle;             //!< 飞机飞行声音句柄
	QString mRiseSoundHandle;               //!< 飞机上升声音句柄
	QString mFallSoundHandle;               //!< 飞机下降声音句柄

    const static QString ATTACK_SOUND_COMPONENT;   //!< 飞机飞行声音Component的名字
	const static QString FLYING_SOUND_COMPONENT;   //!< 飞机飞行声音Component的名字
	const static QString RISE_SOUND_COMPONENT;     //!< 飞机上升声音Component的名字
	const static QString FALL_SOUND_COMPONENT;     //!< 飞机下降声音Component的名字
};

#endif