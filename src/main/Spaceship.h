#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"
#include "ParticlesEffect.h"

#include <map>

/**
  * 飞船尾焰信息
  */
struct FlameInfo {
	QString mName;                 //!< 尾焰名称
	Ogre::Vector3 mPosition;       //!< 尾焰位置
	Ogre::Vector3 mDirection;      //!< 尾焰旋转/朝向
	ParticleInfo mParticle;        //!< 尾焰粒子信息
};

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
	  * @param max_speed 飞船最大的直线速度 
	  * @param speed_per_frame 飞船加速时每一帧的速度的增量(加速度)
      * @param max_lean_angle 飞船平移时最大倾斜角
      * @param angle_per_frame 飞船平移时每一帧角度的增量
	  * @param parallel_move_speed 飞船平移时的速度
	  * @param up_down_speed 飞船上升下降的速度
      * @param ammo_fire_back 飞船攻击效果
      * @param ammo_bomb 飞船攻击爆炸效果
      * @param bullet_handle 飞船子弹句柄
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
		  const float max_lean_angle,
		  const float angle_per_frame,
		  const float parallel_move_speed,
		  const float up_down_speed,
		  const ParticleInfo& ammo_fire_back,
		  const ParticleInfo& ammo_bomb,
		  const std::vector<FlameInfo>& flame_effect,
		  const QString& bullet_handle
		  );

	/**
	  * 设置攻击Offset
	  * @param offset
	  */
	void setAttackOffset(const float& offset);

	/**
	  * 获取攻击Offset
	  * @returns 攻击Offset
	  */
	float getAttackOffset() const;

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
	//void addFlame(const QString& name, const QString& flame_name, Ogre::Vector3 position, Ogre::Vector3 direction);
	void addFlame(const FlameInfo& flame);

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
	  * 这里的SpeedUp用来作为飞船的下降
	  */
    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

	void __onJump(bool is_pressed);

	void __onGetOffVehicle();

    void __onAttack(bool is_pressed); 

protected:
	QString mFlyingSoundHandle;                    //!< 飞船飞行声音句柄
	QString mRiseSoundHandle;                      //!< 飞船上升声音句柄
	QString mFallSoundHandle;                      //!< 飞船下降声音句柄

public:
	const static QString FLYING_SOUND_COMPONENT;   //!< 飞船飞行声音Component的名字
	const static QString RISE_SOUND_COMPONENT;     //!< 飞船上升声音Component的名字
	const static QString FALL_SOUND_COMPONENT;     //!< 飞船下降声音Component的名字

private:
	const static float MAX_LEAN_ANGLE;             //!< 飞船平移时最大的倾斜角
	const static float ANGLE_PER_MOVE;             //!< 飞船每一帧转动的角度
	float mCurAngle;                               //!< 飞船当前倾斜角度
	float mMaxLeanAngle;                           //!< 飞船最大倾斜角度
	float mAnglePerFrame;                          //!< 飞船平移的时候每帧倾斜角度增量
	float mMaxSpeed;                               //!< 飞船飞行最大速度
	float mSpeedPerFrame;                          //!< 飞船加速时每一帧的速度的增量(加速度)
	float mParallelMoveSpeed;                      //!< 飞船平移时的速度
	float mUpDownSpeed;                            //!< 飞船上升下降的速度
	std::vector<FlameInfo> mFlameEffect;           //!< 飞船尾焰的效果
	ParticleInfo mAmmoFireBack;                    //!< 飞船子弹效果
	ParticleInfo mAmmoBomb;                        //!< 飞船攻击爆炸效果
	QString mBulletHandle;                         //!< 飞船子弹句柄
	std::shared_ptr<dt::InteractionComponent> mInteractor; //!< 飞船攻击时的交互Component

};

#endif