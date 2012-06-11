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
  *  ��Ϸ�еĴ����࣬�̳и���Ĵ�����Է��͸��ֶ�����signal������Ϸ�е�ʵ�塣
  */
class Agent : public dt::Node {
Q_OBJECT

signals:
	void sMove(Entity::MoveType type, bool is_pressed); 
	void sAttack(bool is_pressed);
	void sJump(bool is_pressed);
	void sSpeedUp(bool is_pressed);
	void sGetOffVehicle();
	void sAddEquipment(); 
	void sChangeWeapon(Weapon::WeaponType type); 
	void sRemoveWeapon(); 
	void sRemoveWeapon(Weapon::WeaponType type);
	void sLookAround(Ogre::Quaternion quaternion);
	void sReload(); 	
};

#endif