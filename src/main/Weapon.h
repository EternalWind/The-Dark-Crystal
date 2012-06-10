#ifndef THE_DARK_CRYSTAL_WEAPON_STATE
#define THE_DARK_CRYSTAL_WEAPON_STATE

#include "Prop.h"
#include <Logic\InteractionComponent.hpp>
#include <Graphics\MeshComponent.hpp>
#include <Physics\PhysicsBodyComponent.hpp>
#include <Audio\SoundComponent.hpp>

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
	  * 射击
	  */	
	void attack();

	/**
	  * 装弹
	  */	
	void reload();

signals:
    /**
      * 一个当该实体的弹药量被改变时会发送的signal。
      * @param pre_ammo 改变前的弹药量
      * @param cur_ammo 改变后的弹药量
      */
    void sAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo);

    /**
      * 一个当该实体的弹夹数量被改变时会发送的signal。
      * @param pre_ammo 改变前的弹夹数量
      * @param cur_ammo 改变后的弹夹数量
      */
    void sClipNumChanged(uint16_t pre_num, uint16_t cur_num);

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

	float mInterval;					         //!< 子弹射击间隔
	float mReloadTime;				             //!< 补充子弹时间
	QString mMaterialHandle;					 //!< 材质文件名
	QString mFiringSoundHandle;					 //!< 枪声文件名
	QString mReloadingBeginSoundHandle;		     //!< 重装弹药开始声音文件名
	QString mReloadingDoneSoundHandle;		     //!<
	QString mMeshHandle;					     //!<
	dt::InteractionComponent* mInteractor;       //!<
	dt::SoundComponent* mFiringSound;		     //!<
	dt::SoundComponent* mReloadingBeginSound;    //!<
	dt::SoundComponent* mReloadingDoneSound;     //!<
	dt::PhysicsBodyComponent* mPhysicsBody;      //!<
};

#endif
