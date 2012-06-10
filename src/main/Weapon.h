#ifndef THE_DARK_CRYSTAL_WEAPON_STATE
#define THE_DARK_CRYSTAL_WEAPON_STATE

#include "Prop.h"
#include <Logic\InteractionComponent.hpp>
#include <Graphics\MeshComponent.hpp>
#include <Physics\PhysicsBodyComponent.hpp>
#include <Audio\SoundComponent.hpp>
#include <Utils\Timer.hpp>

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
			float interval,
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

	WeaponType getWeaponType() const;

	void setPower(uint16_t power);

	uint16_t getPower() const;

	void setCurClip(uint16_t cur_clip);

	uint16_t getCurClip() const;

	void setMaxClip(uint16_t max_clip);

	uint16_t getMaxClip() const;

	void setWeight(uint16_t weight);

	uint16_t getWeight() const;

	void setAmmoPerClip(uint16_t ammo_per_clip);

	uint16_t getAmmoPerClip() const;

	void setCurAmmo(uint16_t cur_ammo);

	uint16_t getCurAmmo() const;

	void setIsOneShot(bool is_one_shot);

	bool getIsOneShot() const;

	void setHittingRange(float hitting_range);

	float getHittingRange() const;

	void setIsPhysicsBodyEnabled(bool is_enabled);

	bool getIsPhysicsBodyEnabled();

	const dt::InteractionComponent* getInterator() const;

	/**
	  * ���
	  */	
	void attack();

	/**
	  * װ��
	  */	
	void reload();

	void onEnable();

signals:
	void sAmmoChanged(uint16_t cur_ammo);

	void sClipChanged(uint16_t cur_clip);

private slots:
	void _onHit(dt::PhysicsBodyComponent *hit);

	void _onReloadCompleted();

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

	bool mIsPhysicsBodyEnabled;					 //!< �Ƿ�����ʵ��
	dt::Timer* mReloadTimer;					 //!< ����ʱ��
	float mInterval;					         //!< �ӵ�������
	float mReloadTime;				             //!< �����ӵ�ʱ��
	QString mMaterialHandle;					 //!< �����ļ���
	QString mFiringSoundHandle;					 //!< ǹ���ļ���
	QString mReloadingBeginSoundHandle;		     //!< ��װ��ҩ��ʼ�����ļ���
	QString mReloadingDoneSoundHandle;		     //!< ��װ��ҩ��������ļ���
	QString mMeshHandle;					     //!< mesh�ļ���
	dt::InteractionComponent* mInteractor;       //!< InteractionComponentָ��
	dt::SoundComponent* mFiringSound;		     //!< ǹ��ָ��
	dt::SoundComponent* mReloadingBeginSound;    //!< ��װ��ҩ��ʼ����ָ��
	dt::SoundComponent* mReloadingDoneSound;     //!< ��װ��ҩ�������ָ��
	dt::PhysicsBodyComponent* mPhysicsBody;      //!< PhysicsBodyComponentָ��
};

#endif
