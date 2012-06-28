#ifndef THE_DARK_CRYSTAL_CHARACTER
#define THE_DARK_CRYSTAL_CHARACTER

#include "Entity.h"

class Character : public Entity {

    Q_OBJECT

public:
    /**
      * ���캯����
      * @param node_name Node���������
      * @param mesh_handle ģ�;��
      * @param collison_shape_type ��������
      * @param mass ��ʵ�������
      * @param walk_sound_handle ��������ʱ����Ч
      * @param jump_sound_handle ������Ծʱ����Ч
      * @param run_sound_handle �����ܲ�ʱ����Ч
      * @param jump_speed ��Ծʱ�ĳ��ٶȴ�С
      */
    Character(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass, 
        const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle, const float jump_speed);

    virtual void onInitialize();

    virtual void onDeinitialize();

    virtual void onUpdate(double time_diff);

    /**
      * ������Ծ���ٶȴ�С��
      * @param jump_speed ��Ծʱ�ĳ��ٶȴ�С
      */
    void setJumpSpeed(const float jump_speed);

    /**
      * ��ȡ��Ծ���ٶȴ�С��
      * @returns ��Ծʱ�ĳ��ٶȴ�С
      */
    float getJumpSpeed() const;

protected:

	bool __canJump();

protected slots:
    virtual void __onMove(Entity::MoveType type, bool is_pressed);

    virtual void __onJump(bool is_pressed);

    virtual void __onSpeedUp(bool is_pressed);

    virtual void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

protected:
    /**
      * ����ܷ��ƶ���һ��������λ�ã������ܣ������һ���÷��������ƶ�����һ����ԭĿ��λ�������λ�á�
      * @param position Ҫ����Ŀ��λ��
      * @param closest_position ��ԭĿ��λ�������λ�ã������޷��ƶ���Ŀ��λ�õ�ʱ����Ч
      * @returns �Ƿ����ƶ���Ŀ��λ��
      */
    bool __canMoveTo(const btTransform& position, btTransform& closest_position);

protected:
    QString mWalkSoundHandle;                       //!< ������Ч�����
    QString mJumpSoundHandle;                       //!< ��Ծ��Ч�����
    QString mRunSoundHandle;                        //!< �ܲ���Ч�����
    btVector3 mVelocity;                            //!< ��ǰ���ٶ�������
    float mJumpSpeed;                               //!< ��Ծʱ�ĳ��ٶȴ�С��
	double mJumpingInterval;                        //!< ��Ծ��ȴʱ��
	double mTimeElapseAfterJumping;                 //!< ��Ծ�����ŵ�ʱ��

public:
    const static QString WALK_SOUND_COMPONENT;      //!< �������ﲥ��������Ч��SoundComponent�����֡�
    const static QString JUMP_SOUND_COMPONENT;      //!< �������ﲥ����Ծ��Ч��SoundComponent�����֡�
    const static QString RUN_SOUND_COMPONENT;       //!< �������ﲥ���ܶ���Ч��SoundComponent�����֡�
};

#endif