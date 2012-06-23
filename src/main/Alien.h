#ifndef THE_DARK_CRYSTAL_ALIEN
#define THE_DARK_CRYSTAL_ALIEN

#include "Entity.h"
#include "Character.h"

//#include <BulletCollision/CollisionDispatch/btGhostObject.h>
//#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <map>

/**
  * 外星人的实体类。
  */
class Alien : public Character {

    Q_OBJECT

public:
    /**
      * 构造函数。
      * @param node_name Node对象的名字
      * @param mesh_handle 模型句柄
      * @param collison_shape_type 物理外形
      * @param mass 该实体的质量
      * @param walk_sound_handle 外星人行走时的音效
      * @param jump_sound_handle 外星人跳跃时的音效
      * @param run_sound_handle 外星人跑步时的音效
      */
    Alien(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass,
        const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle);

    /**
      * 获取当前武器。
      * @returns 当前武器
      */
    Weapon* getCurWeapon() const;

    /**
      * 改变当前武器。
      * @param type 要设置的武器的类型
      */
    void changeCurWeapon(const Weapon::WeaponType type);

    /**
      * 添加武器到武器列表。
      * @param weapon 要添加的武器
      */
    void addWeapon(Weapon* weapon);

    /**
      * 从武器列表中移除某一武器。
      * @param type 要移除的武器类型
      */
    void removeWeapon(const Weapon::WeaponType type);

    void onInitialize();

    void onDeInitialize();

    void onUpdate(double time_diff);

protected slots:
    //void __onMove(Entity::MoveType type, bool is_pressed);

    //void __onJump(bool is_pressed);

    void __onAttack(bool is_pressed);

    //void __onSpeedUp(bool is_pressed);

    void __onChangeWeapon(Weapon::WeaponType type);

    void __onRemoveWeapon();

    void __onRemoveWeapon(Weapon::WeaponType type);

    void __onAddEquipment(bool is_pressed);

    void __onEquiped(dt::PhysicsBodyComponent* object);

    void __onGetOffVehicle();

    //void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot);

    void __onReload();

protected:
    //QString mWalkSoundHandle;                       //!< 行走音效句柄。
    //QString mJumpSoundHandle;                       //!< 跳跃音效句柄。
    //QString mRunSoundHandle;                        //!< 跑步音效句柄。
    std::map<Weapon::WeaponType, Weapon*> mWeapons; //!< 武器列表。
    Weapon* mCurWeapon;                             //!< 当前武器。
    //btVector3 mVelocity;                            //!< 该外星人当前的速度向量。
    //std::shared_ptr<btPairCachingGhostObject> mBtGhostObject;
    //std::shared_ptr<btKinematicCharacterController> mBtController;
    //const static QString WALK_SOUND_COMPONENT;      //!< 所有外星人类播放行走音效的SoundComponent的名字。
    //const static QString JUMP_SOUND_COMPONENT;      //!< 所有外星人类播放跳跃音效的SoundComponent的名字。
    //const static QString RUN_SOUND_COMPONENT;       //!< 所有外星人类播放跑动音效的SoundComponent的名字。
    const static QString INTERACTOR_COMPONENT;      //!< 所有外星人类与场景进行交互的InteractorComponent的名字。

};

#endif