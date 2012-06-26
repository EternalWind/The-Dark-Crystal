#ifndef THE_DARK_CRYSTAL_ENTITY
#define THE_DARK_CRYSTAL_ENTITY

#include "Weapon.h"

#include <Scene/Node.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Graphics/MeshComponent.hpp>

#include <OgreVector3.h>

#include <QString>

#include <cstdint>

#include "Weapon.h"

/**
  * 游戏场景中能进行移动、攻击等动作的实体类基类。
  */
class Entity : public dt::Node {

    Q_OBJECT

public:
    /**
      * 移动的类型。
      */
    enum MoveType {
        FORWARD,
        BACKWARD,
        LEFTWARD,
        RIGHTWARD,
        STOP
    };

    /**
      * 构造函数。
      * @param node_name Node对象的名字
      * @param mesh_handle 模型句柄
      * @param collison_shape_type 物理外形
      * @param mass 该实体的质量
      */
    Entity(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass);

    /**
      * 获取当前生命值。
      * @returns 当前的生命值
      */
    uint16_t getCurHealth() const;

    /**
      * 设置当前生命值。
      * @param current_health 要设置的当前生命值
      */
    void setCurHealth(const uint16_t current_health);

    /**
      * 获取最大生命值。
      * @returns 最大生命值
      */
    uint16_t getMaxHealth() const;

    /**
      * 设置最大生命值。
      * @param max_health 要设置的最大生命值
      */
    void setMaxHealth(const uint16_t max_health);

    /**
      * 获取当前速度值。
      * @returns 该实体的当前速度值
      */
    float getCurSpeed() const;

    /**
      * 设置当前速度值。
      * @param cur_speed 要设置的当前速度值
      */
    void setCurSpeed(const float cur_speed);

    /**
      * 获取原有速度值。
      * @returns 该实体的原有速度值
      */
    float getOrigSpeed() const;

    /**
      * 设置原有速度值。
      * @param original_speed 要设置的原有速度值
      */
    void setOrigSpeed(const float original_speed);

    /**
      * 获取该实体的ID。
      * @returns 该实体的ID
      */
    QString getID() const;

    /**
      * 设置该实体的ID。
      * @param id 要设置的ID
      */
    void setID(const QString id);

    /**
      * 获取该实体的眼睛位置。
      * @returns 该实体的眼睛位置
      */
    Ogre::Vector3 getEyePosition() const;

    /**
      * 设置该实体的眼睛位置。
      * @param eye_position 要设置的眼睛位置
      */
    void setEyePosition(const Ogre::Vector3 eye_position);

    /**
      * 检查该实体是否在地面上。
      * @returns 该实体是否在地面上
      */
    bool isOnGround();

    /**
      * 设置该实体的朝向。
      * @param direction 表示朝向的四元组
      */
    //void setEntityDirection(const Ogre::Quaternion direction);

    /**
      * 获取该实体的朝向。
      * @returns 表示朝向的四元组
      */
    //Ogre::Quaternion getEntityDirection() const;

    virtual void onInitialize();

    virtual void onDeinitialize();

signals:
    /**
      * 一个当该实体的生命值被改变时会发送的signal。
      * @param pre_health 改变前的生命值
      * @param cur_health 改变后的生命值
      */
    void sHealthChanged(uint16_t pre_health, uint16_t cur_health);

    /**
      * 一个当该实体的速度值被改变时会发送的signal。
      * @param pre_speed 改变前的速度值
      * @param cur_speed 改变后的速度值
      */
    void sSpeedChanged(float pre_speed, float cur_speed);

    /**
      * 一个当当前武器改变时发出的signal。
      * @param pre_weapon 改变前的武器
      * @param cur_weapon 改变后的武器
      */
    void sWeaponChanged(Weapon* pre_weapon, Weapon* cur_weapon);

    /**
      * 一个添加新武器时发出的signal。
      * @param weapon 被添加的武器
      */
    void sWeaponAdded(Weapon* weapon);

    /**
      *　一个当某一武器从武器列表中被移除后发出的signal。
      * @param weapon 被移除的武器
      */
    void sWeaponRemoved(Weapon* weapon);

    /**
      * 一个在成功获取到一个水晶后会发送的signal。
      */
    void sGetCrystal(Entity* entity);

    /**
      * 一个在变更屏幕上的显示时发出的signal
      * @param cur_ammo 变换后的弹药数
      * @param cur_clip 变换后的弹夹数
      */
    void sAmmoClipChange(uint16_t cur_ammo, uint16_t cur_clip);

protected slots:
    virtual void __onMove(Entity::MoveType type, bool is_pressed) = 0;

    virtual void __onJump(bool is_pressed) = 0;

    virtual void __onAttack(bool is_pressed) = 0;

    virtual void __onSpeedUp(bool is_pressed) = 0;

    virtual void __onChangeWeapon(Weapon::WeaponType type) = 0;

    virtual void __onRemoveWeapon() = 0;

    virtual void __onRemoveWeapon(Weapon::WeaponType type) = 0;

    virtual void __onAddEquipment(bool is_pressed) = 0;

    virtual void __onEquiped(dt::PhysicsBodyComponent* object) = 0;

    virtual void __onGetOffVehicle() = 0;

    virtual void __onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) = 0;

    virtual void __onReload() = 0;

protected:
    uint16_t mCurHealth;                                              //!< 该实体的当前生命值。
    uint16_t mMaxHealth;                                              //!< 该实体的最大生命值。
    float mCurSpeed;                                                  //!< 该实体的当前速度值。
    float mOrigSpeed;                                                 //!< 该实体的原有速度值。
    Ogre::Vector3 mEyePosition;                                       //!< 该实体的眼睛位置。
    QString mID;                                                      //!< 该实体的ID。
    QString mMeshHandle;                                              //!< 该实体的模型句柄。
    dt::PhysicsBodyComponent::CollisionShapeType mCollisionShapeType; //!< 该实体的物理外形。
    btScalar mMass;                                                   //!< 该实体的质量。
    Ogre::Vector3 mMoveVector;                                        //!< 一个用来辅助记录移动（速度或力）的方向的向量,采用以该实体为原点的坐标系（具体用法请参见Alien::__onMove）。
    bool mIsAddingEquipment;                                          //!< 标记是否正在进行场景交互（如解锁水晶等）。
    bool mIsMoving;                                                   //!< 标记该实体是否正在移动。
	bool mIsJumping;                                                  //!< 标记该实体是否正在跳跃中
    bool mHasSpeededUp;                                               //!< 标记该实体是否处于提升了速度的状态。
    const static QString MESH_COMPONENT;                              //!< 所有实体类的MeshComponent的名字。
    const static QString PHYSICS_BODY_COMPONENT;                      //!< 所有实体类的PhysicsBodyComponent的名字。
};

#endif