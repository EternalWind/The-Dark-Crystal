#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"

/**
  * ��Ϸ�ؾ�֮һ������ɴ���
  */
class Spaceship : public Vehicle {

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
	  * @param attack_sound_handle �ؾ߹����������
	  * @param flying_sound_handle �ɴ������������
	  * @param rise_sound_handle �ɴ������������
	  * @param fall_sound_handle �ɴ��½��������
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
	  * �����SpeedUp������Ϊ�ɻ����½�
	  */
    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion quaternion);

	void __onJump(bool is_pressed);


private:

    void __moveAround();


protected:
    Ogre::Quaternion mLookAroundQuaternion; //!< �ɻ�ת������
    Ogre::Vector3 mCurDirection;            //!<  ��ͷ����
    float mMaxSpeed;                        //!< �ɴ�����ٶ�
    float mMinSpeed;                        //!< ս������ٶ�/�������ٶ�
    float mAcceleration;                    //!< �ɴ��ļ��ٶ�
    QString mAttackSoundHandle;             //!< �ɻ������������
	QString mFlyingSoundHandle;             //!< �ɻ������������
	QString mRiseSoundHandle;               //!< �ɻ������������
	QString mFallSoundHandle;               //!< �ɻ��½��������

    const static QString ATTACK_SOUND_COMPONENT;   //!< �ɻ���������Component������
	const static QString FLYING_SOUND_COMPONENT;   //!< �ɻ���������Component������
	const static QString RISE_SOUND_COMPONENT;     //!< �ɻ���������Component������
	const static QString FALL_SOUND_COMPONENT;     //!< �ɻ��½�����Component������
};

#endif