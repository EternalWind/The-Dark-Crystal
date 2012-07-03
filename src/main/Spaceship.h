#ifndef THE_DARK_CRYSTAL_PLAIN
#define THE_DARK_CRYSTAL_PLAIN

#include "Vehicle.h"
#include "ParticlesEffect.h"

#include <map>

/**
  * �ɴ�β����Ϣ
  */
struct FlameInfo {
	QString mName;                 //!< β������
	Ogre::Vector3 mPosition;       //!< β��λ��
	Ogre::Vector3 mDirection;      //!< β����ת/����
	ParticleInfo mParticle;        //!< β��������Ϣ
};

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
	  * @param max_speed �ɴ�����ֱ���ٶ� 
	  * @param speed_per_frame �ɴ�����ʱÿһ֡���ٶȵ�����(���ٶ�)
      * @param max_lean_angle �ɴ�ƽ��ʱ�����б��
      * @param angle_per_frame �ɴ�ƽ��ʱÿһ֡�Ƕȵ�����
	  * @param parallel_move_speed �ɴ�ƽ��ʱ���ٶ�
	  * @param up_down_speed �ɴ������½����ٶ�
      * @param ammo_fire_back �ɴ�����Ч��
      * @param ammo_bomb �ɴ�������ըЧ��
      * @param bullet_handle �ɴ��ӵ����
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
	  * ���ù���Offset
	  * @param offset
	  */
	void setAttackOffset(const float& offset);

	/**
	  * ��ȡ����Offset
	  * @returns ����Offset
	  */
	float getAttackOffset() const;

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
	//void addFlame(const QString& name, const QString& flame_name, Ogre::Vector3 position, Ogre::Vector3 direction);
	void addFlame(const FlameInfo& flame);

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
	  * �����SpeedUp������Ϊ�ɴ����½�
	  */
    void __onSpeedUp(bool is_pressed);

    void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

	void __onJump(bool is_pressed);

	void __onGetOffVehicle();

    void __onAttack(bool is_pressed); 

protected:
	QString mFlyingSoundHandle;                    //!< �ɴ������������
	QString mRiseSoundHandle;                      //!< �ɴ������������
	QString mFallSoundHandle;                      //!< �ɴ��½��������

public:
	const static QString FLYING_SOUND_COMPONENT;   //!< �ɴ���������Component������
	const static QString RISE_SOUND_COMPONENT;     //!< �ɴ���������Component������
	const static QString FALL_SOUND_COMPONENT;     //!< �ɴ��½�����Component������

private:
	const static float MAX_LEAN_ANGLE;             //!< �ɴ�ƽ��ʱ������б��
	const static float ANGLE_PER_MOVE;             //!< �ɴ�ÿһ֡ת���ĽǶ�
	float mCurAngle;                               //!< �ɴ���ǰ��б�Ƕ�
	float mMaxLeanAngle;                           //!< �ɴ������б�Ƕ�
	float mAnglePerFrame;                          //!< �ɴ�ƽ�Ƶ�ʱ��ÿ֡��б�Ƕ�����
	float mMaxSpeed;                               //!< �ɴ���������ٶ�
	float mSpeedPerFrame;                          //!< �ɴ�����ʱÿһ֡���ٶȵ�����(���ٶ�)
	float mParallelMoveSpeed;                      //!< �ɴ�ƽ��ʱ���ٶ�
	float mUpDownSpeed;                            //!< �ɴ������½����ٶ�
	std::vector<FlameInfo> mFlameEffect;           //!< �ɴ�β���Ч��
	ParticleInfo mAmmoFireBack;                    //!< �ɴ��ӵ�Ч��
	ParticleInfo mAmmoBomb;                        //!< �ɴ�������ըЧ��
	QString mBulletHandle;                         //!< �ɴ��ӵ����
	std::shared_ptr<dt::InteractionComponent> mInteractor; //!< �ɴ�����ʱ�Ľ���Component

};

#endif