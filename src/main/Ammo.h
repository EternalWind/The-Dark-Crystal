#ifndef THE_DARK_CRYSTAL_AMMO
#define THE_DARK_CRYSTAL_AMMO

#include "Prop.h"
#include "Weapon.h"
/**
  * ��ҩ��
  */
class Ammo : public Prop {
	Q_OBJECT
public:
	Ammo();

	Ammo(const QString &prop_name, const QString &node_name, uint16_t clip_num, Weapon::WeaponType weapon_type);

	~Ammo();

	void onInitialize();

	uint16_t getClipNum() const;

	void setClipNum(uint16_t clip_num);

	Weapon::WeaponType getWeaponType() const;

	void setWeaponType(Weapon::WeaponType weapon_type);

private:
	uint16_t mClipNum;                  //!< ���ֵ�ҩ������
	Weapon::WeaponType mWeaponType;     //!< ���ֵ�ҩ����
};
#endif
