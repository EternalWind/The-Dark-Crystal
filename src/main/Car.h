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
		const QString rush_sound_handle,
		const float width,
		const float length,
		const float max_speed,
		const float straight_acce,
		const float angular_acce);

	void onInitialize();

	void onDeinitialize();

	void onUpdate(double time_diff);

	/**
	  * 返回车身宽度
	  * @returns 车身宽度
	  */
	float getWidth() const;

	/**
	  * 返回车身长度
	  * @returns 车身长度
	  */
	float getLength() const;

	/** 
	  * 返回车轮最大转角
	  * @returns 车轮最大转角
	  */
	float getMaxTheta() const;

	/** 
	  * 返回车轮当前转角
	  * @returns 车轮当前转角
	  */
	float getCurTheta() const;
	
  	/**
	  * 设置车轮当前转角
	  * @param cur_theta 车轮当前转角
	  */
	void setCurTheta(const float& cur_theta);

	/** 
	  * 返回最高速度
	  * @returns 最高速度
	  */
	float getMaxSpeed() const ;

	/**
	  * 返回最低速度/后退最高速度
	  * @returns 最低速度
	  */
	float getMinSpeed() const;


protected slots:
	void __onMove(MoveType type, bool is_pressed);

    void __onSpeedUp(bool is_pressed);

	void __onLookAround(Ogre::Quaternion quaternion);


private:

	void __moveAround();	       

	    
	/**
	  * 角度转弧度
	  * @param degree 角度
	  * @returns 弧度
	  */
	float __getRadian(const float& degree);

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
	float mMaxTheta;                               //!< 战车车轮最大转角
	float mCurTheta;                               //!< 战车目前车轮转角
	Ogre::Vector3 mAcceleration;                   //!< 战车的加速度
	
	static const QString MOVE_SOUND_COMPONENT;     //!< 战车移动声音Component的名字
	static const QString RUSH_SOUND_COMPONENT;     //!< 战车加速声音Component的名字
};

#endif