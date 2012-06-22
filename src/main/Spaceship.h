#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"

#include <map>

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
	  * @param max_speed �ɴ�����ֱ���ٶ� (ע����2-Base�Ա��⾫����ʧ)
	  * @param speed_per_frame �ɻ�����ʱÿһ֡���ٶȵ�����(���ٶ�)
	  * @param parallel_move_speed �ɻ�ƽ��ʱ���ٶ�
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
		  const float parallel_move_speed
		  );

	void onInitialize();

	void onDeinitialize();

	void onUpdate(double time_diff);

	/**
	  * ���ӻ���Ч��
	  * @param name ��������
	  * @param flame_name ������ϵ�����
	  * @param position ����λ��
	  * @param rotation ������ת
	  */
	void addFlame(const QString& name, const QString& flame_name, Ogre::Vector3 position, Ogre::Vector3 direction);

	/**
	  * �������Ч��
	  * @param name ���������
	  */
	void playFlame(const QString& name);

	/**
	  * ֹͣ�������Ч��
	  * @param name ���������
	  */
	void stopFlame(const QString& name);

protected slots:
	void __onMove(MoveType type, bool is_pressed);

	/**
	  * �����SpeedUp������Ϊ�ɻ����½�
	  */
    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

	void __onJump(bool is_pressed);


protected:
	QString mFlyingSoundHandle;                    //!< �ɻ������������
	QString mRiseSoundHandle;                      //!< �ɻ������������
	QString mFallSoundHandle;                      //!< �ɻ��½��������

	const static QString FLYING_SOUND_COMPONENT;   //!< �ɻ���������Component������
	const static QString RISE_SOUND_COMPONENT;     //!< �ɻ���������Component������
	const static QString FALL_SOUND_COMPONENT;     //!< �ɻ��½�����Component������
	const static float MAX_LEAN_ANGLE;             //!< �ɻ�ƽ��ʱ������б��
	const static float ANGLE_PER_MOVE;             //!< �ɻ�ÿһ֡ת���ĽǶ�

private:
	float mCurAngle;                               //!< �ɻ���ǰ��б�Ƕ�      
	float mMaxSpeed;                               //!< �ɻ���������ٶ�
	float mSpeedPerFrame;                          //!< �ɻ�����ʱÿһ֡���ٶȵ�����(���ٶ�)
	float mParallelMoveSpeed;                      //!< �ɻ�ƽ��ʱ���ٶ�
};

#endif