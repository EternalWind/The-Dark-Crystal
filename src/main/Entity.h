#ifndef THE_DARK_CRYSTAL_ENTITY
#define THE_DARK_CRYSTAL_ENTITY

#include <Scene/Node.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Graphics/MeshComponent.hpp>

#include <OgreVector3.h>

#include <QString>

#include <cstdint>

/**
  * 游戏场景中能进行移动、攻击等动作的实体类基类。
  */
class Entity : public dt::Node {

    Q_OBJECT

public:
    enum MoveType {
        FORWARD,
        BACKWARD,
        LEFTWARD,
        RIGHTWARD,
        STOP
    };

    /**
      * 构造函数。
      * @param mesh_handle 模型句柄
      * @param collison_shape_type 物理外形
      * @param mass 该实体的质量
      */
    Entity(const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass);

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
    uint16_t getCurSpeed() const;

    /**
      * 设置当前速度值。
      * @param cur_speed 要设置的当前速度值
      */
    void setCurSpeed(const uint16_t cur_speed);

    /**
      * 获取最大速度值。
      * @returns 该实体的最大速度值
      */
    uint16_t getMaxSpeed() const;

    /**
      * 设置最大速度值。
      * @param max_speed 要设置的最大速度值
      */
    void setMaxSpeed(const uint16_t max_speed);

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
    void sSpeedChanged(uint16_t pre_speed, uint16_t cur_speed);

    /**
      * 一个当该实体的弹药量被改变时会发送的signal。
      * @param pre_ammo 改变前的弹药量
      * @param cur_ammo 改变后的弹药量
      */
    void sAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo);

    /**
      * 一个当该实体的弹夹数量被改变时会发送的signal。
      * @param pre_ammo 改变前的弹夹数量
      * @param cur_ammo 改变后的弹夹数量
      */
    void sClipNumChanged(uint16_t pre_num, uint16_t cur_num);

    /**
      * 一个在成功获取到一个水晶后会发送的signal。
      */
    void sGetCrystal();

protected slots:
    virtual void __onMove(MoveType type, bool is_pressed) = 0;

    virtual void __onJump(bool is_pressed) = 0;

    virtual void __onAttack(bool is_pressed) = 0;

    virtual void __onSpeedUp(bool is_pressed) = 0;

    virtual void __onChangeWeapon(Weapon::WeaponType type) = 0;

    virtual void __onRemoveWeapon() = 0;

    virtual void __onRemoveWeapon(Weapon::WeaponType type) = 0;

    virtual void __onAddEquipment() = 0;

    virtual void __onEquiped(dt::PhysicsBodyComponent* object) = 0;

    virtual void __onGetOffVehicle() = 0;

    virtual void __onLookAround(Ogre::Quaternion quaternion) = 0;

    virtual void __onReload() = 0;

protected:
    uint16_t mCurHealth;                                              //!< 该实体的当前生命值。
    uint16_t mMaxHealth;                                              //!< 该实体的最大生命值。
    uint16_t mCurSpeed;                                               //!< 该实体的当前速度值。
    uint16_t mMaxSpeed;                                               //!< 该实体的最大速度值。
    Ogre::Vector3 mEyePosition;                                       //!< 该实体的眼睛位置。
    QString mID;                                                      //!< 该实体的ID。
    QString mMeshHandle;                                              //!< 该实体的模型句柄。
    dt::PhysicsBodyComponent::CollisionShapeType mCollisionShapeType; //!< 该实体的物理外形。
    btScalar mMass;                                                   //!< 该实体的质量。
    const static QString MESH_COMPONENT;                              //!< 所有实体类的MeshComponent的名字。
    const static QString PHYSICS_BODY_COMPONENT;                      //!< 所有实体类的PhysicsBodyComponent的名字。
};

#endif