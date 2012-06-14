#ifndef THE_DARK_CRYSTAL_CAR
#define THE_DARK_CRYSTAL_CAR

#include "Vehicle.h"

/**
  * ��Ϸ�ؾ�֮һ��ս����
  */
class Car : public Vehicle {

	Q_OBJECT

public:

	/**
	  * ���캯��
	  * @param node_name Node�ڵ�����
	  * @param mesh_handle ģ�;��
	  * @param collision_shape_type ��������
	  * @param mass �ؾ�����
	  * @param attack_value �ؾ߹�����
	  * @param attack_range �ؾ߹�����Χ
	  * @param attack_interval �ؾ߹���ʱ����
	  * @param move_sound_handle ս���ƶ��������
	  * @param rush_sound_handle ս�������������
	  * @param attack_sound_handle ս�������������
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
	  * ���س�����
	  * @returns ������
	  */
	float getWidth() const;

	/**
	  * ���س�����
	  * @returns ������
	  */
	float getLength() const;

	/** 
	  * ���س������ת��
	  * @returns �������ת��
	  */
	float getMaxTheta() const;

	/** 
	  * ���س��ֵ�ǰת��
	  * @returns ���ֵ�ǰת��
	  */
	float getCurTheta() const;
	
  	/**
	  * ���ó��ֵ�ǰת��
	  * @param cur_theta ���ֵ�ǰת��
	  */
	void setCurTheta(const float& cur_theta);

	/** 
	  * ��������ٶ�
	  * @returns ����ٶ�
	  */
	float getMaxSpeed() const ;

	/**
	  * ��������ٶ�/��������ٶ�
	  * @returns ����ٶ�
	  */
	float getMinSpeed() const;


protected slots:
	void __onMove(MoveType type, bool is_pressed);

    void __onSpeedUp(bool is_pressed);

	void __onLookAround(Ogre::Quaternion quaternion);


private:

	void __moveAround();	       

	    
	/**
	  * �Ƕ�ת����
	  * @param degree �Ƕ�
	  * @returns ����
	  */
	float __getRadian(const float& degree);

	/**
	  * ��ȡ���λ�ƣ����ת��
	  * @param dx ���λ��x(����)
	  * @param dy ���λ��y(����)
	  * @param alpha ���ת��(����)
	  */
	void __getDelta(float &dx, float &dy, float &alpha);
	

protected:
	QString mMoveSoundHandle;                      //!< ս���ƶ��������
	QString mRushSoundHandle;                      //!< ս�������������
	float mWidth;                                  //!< ������
	float mLength;                                 //!< ������
	float mMaxSpeed;                               //!< ս������ٶ�
	float mMinSpeed;                               //!< ս������ٶ�/�������ٶ�
	float mMaxTheta;                               //!< ս���������ת��
	float mCurTheta;                               //!< ս��Ŀǰ����ת��
	Ogre::Vector3 mAcceleration;                   //!< ս���ļ��ٶ�
	
	static const QString MOVE_SOUND_COMPONENT;     //!< ս���ƶ�����Component������
	static const QString RUSH_SOUND_COMPONENT;     //!< ս����������Component������
};

#endif