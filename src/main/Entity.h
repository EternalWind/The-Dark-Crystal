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
  * ��Ϸ�������ܽ����ƶ��������ȶ�����ʵ������ࡣ
  */
class Entity : public dt::Node {

    Q_OBJECT

public:
    /**
      * �ƶ������͡�
      */
    enum MoveType {
        FORWARD,
        BACKWARD,
        LEFTWARD,
        RIGHTWARD,
        STOP
    };

    /**
      * ���캯����
      * @param node_name Node���������
      * @param mesh_handle ģ�;��
      * @param collison_shape_type ��������
      * @param mass ��ʵ�������
      */
    Entity(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass);

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
      * ��ȡԭ���ٶ�ֵ��
      * @returns ��ʵ���ԭ���ٶ�ֵ
      */
    uint16_t getOrigSpeed() const;

    /**
      * ����ԭ���ٶ�ֵ��
      * @param max_speed Ҫ���õ�ԭ���ٶ�ֵ
      */
    void setOrigSpeed(const uint16_t original_speed);

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

    /**
      * ����ʵ���Ƿ��ڵ����ϡ�
      * @returns ��ʵ���Ƿ��ڵ�����
      */
    bool isOnGround();

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
      * һ������ǰ�����ı�ʱ������signal��
      * @param pre_weapon �ı�ǰ������
      * @param cur_weapon �ı�������
      */
    void sWeaponChanged(Weapon* pre_weapon, Weapon* cur_weapon);

    /**
      * һ�����������ʱ������signal��
      * @param weapon ����ӵ�����
      */
    void sWeaponAdded(Weapon* weapon);

    /**
      *��һ����ĳһ�����������б��б��Ƴ��󷢳���signal��
      * @param weapon ���Ƴ�������
      */
    void sWeaponRemoved(Weapon* weapon);

    /**
      * һ���ڳɹ���ȡ��һ��ˮ����ᷢ�͵�signal��
      */
    void sGetCrystal(Entity* entity);

protected slots:
    virtual void __onMove(MoveType type, bool is_pressed) = 0;

    virtual void __onJump(bool is_pressed) = 0;

    virtual void __onAttack(bool is_pressed) = 0;

    virtual void __onSpeedUp(bool is_pressed) = 0;

    virtual void __onChangeWeapon(Weapon::WeaponType type) = 0;

    virtual void __onRemoveWeapon() = 0;

    virtual void __onRemoveWeapon(Weapon::WeaponType type) = 0;

    virtual void __onAddEquipment(bool is_pressed) = 0;

    virtual void __onEquiped(dt::PhysicsBodyComponent* object) = 0;

    virtual void __onGetOffVehicle() = 0;

    virtual void __onLookAround(Ogre::Quaternion quaternion) = 0;

    virtual void __onReload() = 0;

protected:
    uint16_t mCurHealth;                                              //!< ��ʵ��ĵ�ǰ����ֵ��
    uint16_t mMaxHealth;                                              //!< ��ʵ����������ֵ��
    uint16_t mCurSpeed;                                               //!< ��ʵ��ĵ�ǰ�ٶ�ֵ��
    uint16_t mOrigSpeed;                                              //!< ��ʵ���ԭ���ٶ�ֵ��
    Ogre::Vector3 mEyePosition;                                       //!< ��ʵ����۾�λ�á�
    QString mID;                                                      //!< ��ʵ���ID��
    QString mMeshHandle;                                              //!< ��ʵ���ģ�;����
    dt::PhysicsBodyComponent::CollisionShapeType mCollisionShapeType; //!< ��ʵ����������Ρ�
    btScalar mMass;                                                   //!< ��ʵ���������
    Ogre::Vector3 mMoveVector;                                        //!< һ������������¼�ƶ����ٶȻ������ķ��������,�����Ը�ʵ��Ϊԭ�������ϵ�������÷���μ�Alien::__onMove����
    bool mIsAddingEquipment;                                          //!< ����Ƿ����ڽ��г��������������ˮ���ȣ���
    bool mIsMoving;                                                   //!< ��Ǹ�ʵ���Ƿ������ƶ���
    bool mHasSpeededUp;                                               //!< ��Ǹ�ʵ���Ƿ����������ٶȵ�״̬��
    const static QString MESH_COMPONENT;                              //!< ����ʵ�����MeshComponent�����֡�
    const static QString PHYSICS_BODY_COMPONENT;                      //!< ����ʵ�����PhysicsBodyComponent�����֡�
};

#endif