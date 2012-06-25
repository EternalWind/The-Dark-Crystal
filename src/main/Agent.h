#ifndef THE_DARK_CRYSTAL_AGENT
#define THE_DARK_CRYSTAL_AGENT

#include <Scene/Node.hpp>

#include <QString>
#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <QObject>

#include "Entity.h"
#include "Weapon.h"

/**
  *  游戏中的代理类，继承改类的代理可以发送各种动作的signal控制游戏中的实体。
  */
class Agent : public dt::Node {

Q_OBJECT

public:
    /**
      * 构造函数。
      * @param name 该代理的名字
      */
    Agent(const QString name);

    /**
      * 将该Agent附加到某一个Entity上。
      * @param entity 要附加到的Entity
      */
    void attachTo(Entity* entity);

    /**
      * 将该Agent从已添加到的Entity上分离出来。
      */
    void detach();

signals:
	void sMove(Entity::MoveType type, bool is_pressed); 
	void sAttack(bool is_pressed);
	void sJump(bool is_pressed);
	void sSpeedUp(bool is_pressed);
	void sGetOffVehicle();
	void sAddEquipment(bool is_pressed); 
	void sChangeWeapon(Weapon::WeaponType type); 
	void sRemoveWeapon(); 
	void sRemoveWeapon(Weapon::WeaponType type);
	void sLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot = Ogre::Quaternion::IDENTITY);
	void sReload();

protected:
    QString mName;

public:
    const static QString AGENT;    //!< Agent_Name
};

#endif