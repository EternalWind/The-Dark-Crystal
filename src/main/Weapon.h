#ifndef THE_DARK_CRYSTAL_WEAPON_STATE
#define THE_DARK_CRYSTAL_WEAPON_STATE

#include "Prop.h"
#include <Logic\InteractionComponent.hpp>
#include <Graphics\MeshComponent.hpp>
#include <Physics\PhysicsBodyComponent.hpp>
#include <Audio\SoundComponent.hpp>

/**
  * ������
  */
class Weapon : public Prop {
	Q_OBJECT
public:

	enum WeaponType {
		PRIMARY,
		SECONDARY,
		THROWABLE
	};

	Weapon();

	//������Ͷ��������ʱ���Բ���hitting_range
	Weapon( const QString &name, 
			WeaponType type, 
			uint16_t power, 
			uint16_t CurClip, 
			uint16_t max_clip, 
			uint16_t weight, 
			uint16_t ammo_per_clip, 
			uint16_t cur_ammo, 
			bool is_one_shot, 
			const QString &firing_sound_handle, 
			const QString &reloading_begin_sound_handle, 
			const QString &reloading_done_sound_handle, 
			const QString &mesh_handle,
			float hitting_range = 0.0f, 
			const QString &material_handle = "");

	~Weapon();

	void onInitialize();

	void onDeinitialize();

	void setWeaponType(WeaponType type);

	WeaponType getWeaponType();

	void setPower(uint16_t power);

	uint16_t getPower();

	void setCurClip(uint16_t cur_clip);

	uint16_t getCurClip();

	void setMaxClip(uint16_t max_clip);

	uint16_t getMaxClip();

	void setWeight(uint16_t weight);

	uint16_t getWeight();

	void setAmmoPerClip(uint16_t ammo_per_clip);

	uint16_t getAmmoPerClip();

	void setCurAmmo(uint16_t cur_ammo);

	uint16_t getCurAmmo();

	void setIsOneShot(bool is_one_shot);

	bool getIsOneShot();

	void setHittingRange(float hitting_range);

	float getHittingRange();

	/**
	  * ���
	  */	
	void attack();

	/**
	  * װ��
	  */	
	void reload();

signals:
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

private:

	WeaponType mWeaponType;					     //!< ��������
	uint16_t mPower;						     //!< ����������
	uint16_t mCurClip;						     //!< ��ǰ������
	uint16_t mMaxClip;						     //!< ��󵯼���
	uint16_t mWeight;						     //!< ����������
	uint16_t mAmmoPerClip;						 //!< ÿ�����е��ӵ���
	uint16_t mCurAmmo;						     //!< ��ǰ�����ڵ��ӵ���
	bool mIsOneShot;							 //!< �Ƿ������
	float mHittingRange;				         //!< ը��Ͷ������

	float mInterval;					         //!< �ӵ�������
	float mReloadTime;				             //!< �����ӵ�ʱ��
	QString mMaterialHandle;					 //!< �����ļ���
	QString mFiringSoundHandle;					 //!< ǹ���ļ���
	QString mReloadingBeginSoundHandle;		     //!< ��װ��ҩ��ʼ�����ļ���
	QString mReloadingDoneSoundHandle;		     //!<
	QString mMeshHandle;					     //!<
	dt::InteractionComponent* mInteractor;       //!<
	dt::SoundComponent* mFiringSound;		     //!<
	dt::SoundComponent* mReloadingBeginSound;    //!<
	dt::SoundComponent* mReloadingDoneSound;     //!<
	dt::PhysicsBodyComponent* mPhysicsBody;      //!<
};

#endif
