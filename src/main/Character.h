#ifndef THE_DARK_CRYSTAL_CHARACTER
#define THE_DARK_CRYSTAL_CHARACTER

#include "Entity.h"

class Character : public Entity {

    Q_OBJECT

public:
    /**
      * 构造函数。
      * @param node_name Node对象的名字
      * @param mesh_handle 模型句柄
      * @param collison_shape_type 物理外形
      * @param mass 该实体的质量
      * @param walk_sound_handle 人物行走时的音效
      * @param jump_sound_handle 人物跳跃时的音效
      * @param run_sound_handle 人物跑步时的音效
      * @param jump_speed 跳跃时的初速度大小
      */
    Character(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass, 
        const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle, const float jump_speed);

    virtual void onInitialize();

    virtual void onDeinitialize();

    virtual void onUpdate(double time_diff);

    /**
      * 设置跳跃初速度大小。
      * @param jump_speed 跳跃时的初速度大小
      */
    void setJumpSpeed(const float jump_speed);

    /**
      * 获取跳跃初速度大小。
      * @returns 跳跃时的初速度大小
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
      * 检查能否移动到一个给定的位置，若不能，则输出一个该方向上能移动到的一个离原目标位置最近的位置。
      * @param position 要检查的目标位置
      * @param closest_position 离原目标位置最近的位置，仅在无法移动到目标位置的时候有效
      * @returns 是否能移动到目标位置
      */
    bool __canMoveTo(const btTransform& position, btTransform& closest_position);

protected:
    QString mWalkSoundHandle;                       //!< 行走音效句柄。
    QString mJumpSoundHandle;                       //!< 跳跃音效句柄。
    QString mRunSoundHandle;                        //!< 跑步音效句柄。
    btVector3 mVelocity;                            //!< 当前的速度向量。
    float mJumpSpeed;                               //!< 跳跃时的初速度大小。
	double mJumpingInterval;                        //!< 跳跃冷却时间
	double mTimeElapseAfterJumping;                 //!< 跳跃后流逝的时间

public:
    const static QString WALK_SOUND_COMPONENT;      //!< 所有人物播放行走音效的SoundComponent的名字。
    const static QString JUMP_SOUND_COMPONENT;      //!< 所有人物播放跳跃音效的SoundComponent的名字。
    const static QString RUN_SOUND_COMPONENT;       //!< 所有人物播放跑动音效的SoundComponent的名字。
};

#endif