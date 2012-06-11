#include "Ammo.h"

Ammo::Ammo() {
}

Ammo::Ammo(uint16_t clip_num, Weapon::WeaponType weapon_type)
		: mClipNum(clip_num), mWeaponType(weapon_type) {
}

Ammo::~Ammo() {
}

uint16_t Ammo::getClipNum() const {
	return mClipNum;
}

void Ammo::setClipNum(uint16_t clip_num) {
	mClipNum = clip_num;
}

Weapon::WeaponType Ammo::getWeaponType() const {
	return mWeaponType;
}

void Ammo::setWeaponType(Weapon::WeaponType weapon_type) {
	mWeaponType = weapon_type;
}