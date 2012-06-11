#ifndef THE_DARK_CRYSTAL_AGENT
#define THE_DARK_CRYSTAL_AGENT

#include <Scene/Node.hpp>

#include <QString>
#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <QObject>

//#include "Entity.h"
//#include "Weapon.h"

/**
  *  ע��
  */
class Agent : public dt::Node {
Q_OBJECT

signals:
/**
  * ע��
  * @param ����
  * @returns ����ֵ
  */	
	//void sMove(Entity::MoveType type); 
	void sAttack(bool is_pressed);
	void sJump(bool is_pressed);
	void sSpeedUp(bool is_pressed);
	void sGetOffVehicle();
	void sAddEquipment(); 
	//void sChangeWeapon(Weapon::WeaponType type); 
	void sRemoveWeapon(); 
	//void sRemoveWeapon(Weapon::WeaponType type);
	//void sLookAround(dt::Quaterion quaterion);
	void sReload(); 	
}

#endif