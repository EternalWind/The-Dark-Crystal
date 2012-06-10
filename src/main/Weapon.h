#ifndef THE_DARK_CRYSTAL_WEAPON_STATE
#define THE_DARK_CRYSTAL_WEAPON_STATE

#include "Prop.h"
#include <Logic\InteractionComponent.hpp>
#include <Graphics\MeshComponent.hpp>
#include <Physics\PhysicsBodyComponent.hpp>
#include <Audio\SoundComponent.hpp>
#include <Utils\Timer.hpp>

/**
  * 武器类
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

	//当不是投掷型武器时忽略参数hitting_range
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
	  * 射击
	  */	
	void attack();

	/**
	  * 装弹
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

	WeaponType mWeaponType;					     //!< 武器类型
	uint16_t mPower;						     //!< 武器攻击力
	uint16_t mCurClip;						     //!< 当前弹夹数
	uint16_t mMaxClip;						     //!< 最大弹夹数
	uint16_t mWeight;						     //!< 武器的重量
	uint16_t mAmmoPerClip;						 //!< 每个弹夹的子弹数
	uint16_t mCurAmmo;						     //!< 当前弹夹内的子弹数
	bool mIsOneShot;							 //!< 是否可连击
	float mHittingRange;				         //!< 炸弹投掷距离

	bool mIsPhysicsBodyEnabled;					 //!< 是否物理实体
	dt::Timer* mReloadTimer;					 //!< 控制时间
	float mInterval;					         //!< 子弹射击间隔
	float mReloadTime;				             //!< 补充子弹时间
	QString mMaterialHandle;					 //!< 材质文件名
	QString mFiringSoundHandle;					 //!< 枪声文件名
	QString mReloadingBeginSoundHandle;		     //!< 重装弹药开始声音文件名
	QString mReloadingDoneSoundHandle;		     //!< 重装弹药完成声音文件名
	QString mMeshHandle;					     //!< mesh文件名
	dt::InteractionComponent* mInteractor;       //!< InteractionComponent指针
	dt::SoundComponent* mFiringSound;		     //!< 枪声指针
	dt::SoundComponent* mReloadingBeginSound;    //!< 重装弹药开始声音指针
	dt::SoundComponent* mReloadingDoneSound;     //!< 重装弹药完成声音指针
	dt::PhysicsBodyComponent* mPhysicsBody;      //!< PhysicsBodyComponent指针
};

#endif
