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
	  * @param max_speed 战车最大速度
	  * @param speed_per_frame 战车每帧速度增量(加速度）
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
		const QString rush_sound_handle,
		const float max_speed,
		const float speed_per_frame
		);

	void onInitialize();

	void onDeinitialize();

	void onUpdate(double time_diff);

	void setCurTheta(float theta);

	float getCurTheta();

protected slots:
	void __onMove(MoveType type, bool is_pressed);

    void __onSpeedUp(bool is_pressed);

	void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);


private:

	/**
	  * 获取相对位移，相对转角
	  * @param dx 相对位移x(引用)
	  * @param dy 相对位移y(引用)
	  * @param alpha 相对转角(引用)
	  */
	void __getDelta(float &dx, float &dy, float &alpha);
	

protected:
	QString mMoveSoundHandle;                      //!< 战车移动声音句柄
	QString mRushSoundHandle;                      //!< 战车加速声音句柄
	float mWidth;                                  //!< 车身宽度
	float mLength;                                 //!< 车身长度

	float mMaxSpeed;                               //!< 战车最高速度
	float mMinSpeed;                               //!< 战车最低速度/向后最大速度
	float mSpeedPerFrame;                          //!< 战车速度增量
	float mCurTheta;                               //!< 战车目前车轮转角
	dt::Node::NodeSP mLauncher;                    //!< 发射台/大炮
	
	static const QString MOVE_SOUND_COMPONENT;     //!< 战车移动声音Component的名字
	static const QString RUSH_SOUND_COMPONENT;     //!< 战车加速声音Component的名字

	static const float MAX_THETA;                  //!< 车轮最大转角
	static const float THETA_PER_FRAME;            //!< 车轮每一帧转角增量

};

#endif