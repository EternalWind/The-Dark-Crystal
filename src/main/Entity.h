#ifndef THE_DARK_CRYSTAL_ENTITY
#define THE_DARK_CRYSTAL_ENTITY

#include <Scene/Node.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Graphics/MeshComponent.hpp>

#include <OgreVector3.h>

#include <QString>

#include <cstdint>

/**
  * ��Ϸ�������ܽ����ƶ��������ȶ�����ʵ������ࡣ
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
      * ���캯����
      * @param mesh_handle ģ�;��
      * @param collison_shape_type ��������
      * @param mass ��ʵ�������
      */
    Entity(const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass);

    /**
      * ��ȡ��ǰ����ֵ��
      * @returns ��ǰ������ֵ
      */
    uint16_t getCurHealth() const;

    /**
      * ���õ�ǰ����ֵ��
      * @param current_health Ҫ���õĵ�ǰ����ֵ
      */
    void setCurHealth(const uint16_t current_health);

    /**
      * ��ȡ�������ֵ��
      * @returns �������ֵ
      */
    uint16_t getMaxHealth() const;

    /**
      * �����������ֵ��
      * @param max_health Ҫ���õ��������ֵ
      */
    void setMaxHealth(const uint16_t max_health);

    /**
      * ��ȡ��ǰ�ٶ�ֵ��
      * @returns ��ʵ��ĵ�ǰ�ٶ�ֵ
      */
    uint16_t getCurSpeed() const;

    /**
      * ���õ�ǰ�ٶ�ֵ��
      * @param cur_speed Ҫ���õĵ�ǰ�ٶ�ֵ
      */
    void setCurSpeed(const uint16_t cur_speed);

    /**
      * ��ȡ����ٶ�ֵ��
      * @returns ��ʵ�������ٶ�ֵ
      */
    uint16_t getMaxSpeed() const;

    /**
      * ��������ٶ�ֵ��
      * @param max_speed Ҫ���õ�����ٶ�ֵ
      */
    void setMaxSpeed(const uint16_t max_speed);

    /**
      * ��ȡ��ʵ���ID��
      * @returns ��ʵ���ID
      */
    QString getID() const;

    /**
      * ���ø�ʵ���ID��
      * @param id Ҫ���õ�ID
      */
    void setID(const QString id);

    /**
      * ��ȡ��ʵ����۾�λ�á�
      * @returns ��ʵ����۾�λ��
      */
    Ogre::Vector3 getEyePosition() const;

    /**
      * ���ø�ʵ����۾�λ�á�
      * @param eye_position Ҫ���õ��۾�λ��
      */
    void setEyePosition(const Ogre::Vector3 eye_position);

    virtual void onInitialize();

    virtual void onDeinitialize();

signals:
    /**
      * һ������ʵ�������ֵ���ı�ʱ�ᷢ�͵�signal��
      * @param pre_health �ı�ǰ������ֵ
      * @param cur_health �ı�������ֵ
      */
    void sHealthChanged(uint16_t pre_health, uint16_t cur_health);

    /**
      * һ������ʵ����ٶ�ֵ���ı�ʱ�ᷢ�͵�signal��
      * @param pre_speed �ı�ǰ���ٶ�ֵ
      * @param cur_speed �ı����ٶ�ֵ
      */
    void sSpeedChanged(uint16_t pre_speed, uint16_t cur_speed);

    /**
      * һ������ʵ��ĵ�ҩ�����ı�ʱ�ᷢ�͵�signal��
      * @param pre_ammo �ı�ǰ�ĵ�ҩ��
      * @param cur_ammo �ı��ĵ�ҩ��
      */
    void sAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo);

    /**
      * һ������ʵ��ĵ����������ı�ʱ�ᷢ�͵�signal��
      * @param pre_ammo �ı�ǰ�ĵ�������
      * @param cur_ammo �ı��ĵ�������
      */
    void sClipNumChanged(uint16_t pre_num, uint16_t cur_num);

    /**
      * һ���ڳɹ���ȡ��һ��ˮ����ᷢ�͵�signal��
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
    uint16_t mCurHealth;                                              //!< ��ʵ��ĵ�ǰ����ֵ��
    uint16_t mMaxHealth;                                              //!< ��ʵ����������ֵ��
    uint16_t mCurSpeed;                                               //!< ��ʵ��ĵ�ǰ�ٶ�ֵ��
    uint16_t mMaxSpeed;                                               //!< ��ʵ�������ٶ�ֵ��
    Ogre::Vector3 mEyePosition;                                       //!< ��ʵ����۾�λ�á�
    QString mID;                                                      //!< ��ʵ���ID��
    QString mMeshHandle;                                              //!< ��ʵ���ģ�;����
    dt::PhysicsBodyComponent::CollisionShapeType mCollisionShapeType; //!< ��ʵ����������Ρ�
    btScalar mMass;                                                   //!< ��ʵ���������
    const static QString MESH_COMPONENT;                              //!< ����ʵ�����MeshComponent�����֡�
    const static QString PHYSICS_BODY_COMPONENT;                      //!< ����ʵ�����PhysicsBodyComponent�����֡�
};

#endif