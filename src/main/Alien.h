#ifndef THE_DARK_CRYSTAL_ALIEN
#define THE_DARK_CRYSTAL_ALIEN

#include "Entity.h"
#include "Character.h"

//#include <BulletCollision/CollisionDispatch/btGhostObject.h>
//#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <map>

/**
  * �����˵�ʵ���ࡣ
  */
class Alien : public Character {

    Q_OBJECT

public:
    /**
      * ���캯����
      * @param node_name Node���������
      * @param mesh_handle ģ�;��
      * @param collison_shape_type ��������
      * @param mass ��ʵ�������
      * @param walk_sound_handle ����������ʱ����Ч
      * @param jump_sound_handle ��������Ծʱ����Ч
      * @param run_sound_handle �������ܲ�ʱ����Ч
      */
    Alien(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass,
        const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle);

    /**
      * ��ȡ��ǰ������
      * @returns ��ǰ����
      */
    Weapon* getCurWeapon() const;

    /**
      * �ı䵱ǰ������
      * @param type Ҫ���õ�����������
      */
    void changeCurWeapon(const Weapon::WeaponType type);

    /**
      * ��������������б�
      * @param weapon Ҫ��ӵ�����
      */
    void addWeapon(Weapon* weapon);

    /**
      * �������б����Ƴ�ĳһ������
      * @param type Ҫ�Ƴ�����������
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
    //QString mWalkSoundHandle;                       //!< ������Ч�����
    //QString mJumpSoundHandle;                       //!< ��Ծ��Ч�����
    //QString mRunSoundHandle;                        //!< �ܲ���Ч�����
    std::map<Weapon::WeaponType, Weapon*> mWeapons; //!< �����б�
    Weapon* mCurWeapon;                             //!< ��ǰ������
    //btVector3 mVelocity;                            //!< �������˵�ǰ���ٶ�������
    //std::shared_ptr<btPairCachingGhostObject> mBtGhostObject;
    //std::shared_ptr<btKinematicCharacterController> mBtController;
    //const static QString WALK_SOUND_COMPONENT;      //!< �����������ಥ��������Ч��SoundComponent�����֡�
    //const static QString JUMP_SOUND_COMPONENT;      //!< �����������ಥ����Ծ��Ч��SoundComponent�����֡�
    //const static QString RUN_SOUND_COMPONENT;       //!< �����������ಥ���ܶ���Ч��SoundComponent�����֡�
    const static QString INTERACTOR_COMPONENT;      //!< �������������볡�����н�����InteractorComponent�����֡�

};

#endif