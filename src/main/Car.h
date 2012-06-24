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
	  * @param max_speed ս������ٶ�
	  * @param speed_per_frame ս��ÿ֡�ٶ�����(���ٶȣ�
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
	float mSpeedPerFrame;                          //!< ս���ٶ�����
	float mCurTheta;                               //!< ս��Ŀǰ����ת��
	dt::Node::NodeSP mLauncher;                    //!< ����̨/����
	
	static const QString MOVE_SOUND_COMPONENT;     //!< ս���ƶ�����Component������
	static const QString RUSH_SOUND_COMPONENT;     //!< ս����������Component������

	static const float MAX_THETA;                  //!< �������ת��
	static const float THETA_PER_FRAME;            //!< ����ÿһ֡ת������

};

#endif