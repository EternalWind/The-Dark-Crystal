#include "Weapon.h"
#include "Entity.h"
#include <Logic/CollisionComponent.hpp>
#include <Logic/RaycastComponent.hpp>

Weapon::Weapon(){
}

Weapon::Weapon( const QString &name, 
				WeaponType type, 
				uint16_t power, 
				uint16_t cur_clip, 
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
				float hitting_range, 
				const QString &material_handle)
			  : mWeaponType(type),
				mPower(power), 
				mCurClip(cur_clip), 
				mMaxClip(max_clip), 
				mWeight(weight), 
				mAmmoPerClip(ammo_per_clip), 
				mCurAmmo(cur_ammo),
				mIsOneShot(is_one_shot),
				mIsPhysicsBodyEnabled(false),
				mReloadTimer(nullptr),
				mInterval(interval),
				mFiringSoundHandle(firing_sound_handle),
				mReloadingBeginSoundHandle(reloading_begin_sound_handle),
				mReloadingDoneSoundHandle(reloading_done_sound_handle),
				mMeshHandle(mesh_handle),
				mInteractor(nullptr),     
				mFiringSound(nullptr),		  
				mReloadingBeginSound(nullptr),    
				mReloadingDoneSound(nullptr),  
				mPhysicsBody(nullptr),    
				mHittingRange(hitting_range),
				mMaterialHandle(material_handle) { 
	Prop(name, WEAPON);
}

Weapon::~Weapon(){
}

void Weapon::setWeaponType(WeaponType type) {
	mWeaponType = type;
}

Weapon::WeaponType Weapon::getWeaponType() const {
	return mWeaponType;
}

void Weapon::setPower(uint16_t power) {
	mPower = power;
}

uint16_t Weapon::getPower() const {
	return mPower;
}

void Weapon::setCurClip(uint16_t cur_clip) {
	if (cur_clip <= mMaxClip)
		mCurClip = cur_clip;
	else
		mCurClip = mMaxClip;
	
	emit sClipNumChanged(mCurClip);
}

uint16_t Weapon::getCurClip() const { 
	return mCurClip;
}

void Weapon::setMaxClip(uint16_t max_clip) {
	mMaxClip = max_clip;

	if (mCurClip > mMaxClip)
		this->setCurClip(mMaxClip);
}

uint16_t Weapon::getMaxClip() const { 
	return mMaxClip;
}

void Weapon::setWeight(uint16_t weight){
	mWeight = weight;
}

uint16_t Weapon::getWeight() const {
	return mWeight;
}

void Weapon::setAmmoPerClip(uint16_t ammo_per_clip){
	mAmmoPerClip = ammo_per_clip;
	if (mCurAmmo > mAmmoPerClip)
		this->setCurAmmo(mAmmoPerClip);
}

uint16_t Weapon::getAmmoPerClip() const {
	return mAmmoPerClip;
}

void Weapon::setCurAmmo(uint16_t cur_ammo) {
	if (cur_ammo <= mAmmoPerClip)
		mCurAmmo = cur_ammo;
	else
		mCurAmmo = mAmmoPerClip;
	emit sAmmoChanged(mCurAmmo);
}

uint16_t Weapon::getCurAmmo() const {
	return mCurAmmo;
}

void Weapon::setIsOneShot(bool is_one_shot) {
	mIsOneShot = is_one_shot; 
}

bool Weapon::getIsOneShot() const { 
	return mIsOneShot;
}

void Weapon::setIsPressed(bool is_pressed) {
	mIsPressed = is_pressed;
}

bool Weapon::getIsPressed() const {
	return mIsPressed;
}

void Weapon::setHittingRange(float hitting_range) {
	mHittingRange = hitting_range;
}

float Weapon::getHittingRange() const {
	return mHittingRange;
}

void Weapon::setIsPhysicsBodyEnabled(bool is_enabled) {
	mIsPhysicsBodyEnabled = is_enabled;
	
	if (is_enabled)
		mPhysicsBody->enable();
	else
		mPhysicsBody->disable();
}

bool Weapon::getIsPhysicsBodyEnabled() {
	return mIsPhysicsBodyEnabled;
}

void Weapon::onInitialize() {
	if (mWeaponType == THROWABLE) {
		mInteractor = new dt::CollisionComponent("bullet", "interactor");
	} else {
		mInteractor = new dt::RaycastComponent("interactor");
	}
	mInteractor->setIntervalTime(mInterval);
	this->addComponent(mInteractor);

	this->addComponent(new dt::MeshComponent(mMeshHandle, mMaterialHandle, "weapon-mesh"));
	mPhysicsBody = this->addComponent(new dt::PhysicsBodyComponent("weapon-mesh", "weapon-body",
        dt::PhysicsBodyComponent::BOX)).get();

	mIsPhysicsBodyEnabled = true;

	if (!QObject::connect(mInteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
                         this,        SLOT(_onHit(dt::PhysicsBodyComponent*)))) {
            dt::Logger::get().error("Cannot connect the sHit signal with the OnHit slot.");
    }

	if (mFiringSoundHandle != "") {
        mFiringSound = this->addComponent(new dt::SoundComponent(mFiringSoundHandle,
            this->getName() + "_firing_sound")).get();
        mFiringSound->setVolume(100.0f);
    }

    if (mReloadingBeginSoundHandle != "") {
        mReloadingBeginSound = this->addComponent(new dt::SoundComponent(mReloadingBeginSoundHandle,
            this->getName() + "_reloading_begin_sound")).get();
        mReloadingBeginSound->setVolume(100.0f);
    }

    if (mReloadingDoneSoundHandle != "") {
        mReloadingDoneSound = this->addComponent(new dt::SoundComponent(mReloadingDoneSoundHandle,
            this->getName() + "_reloading_done_sound")).get();
        mReloadingDoneSound->setVolume(100.0f);
    }
}

void Weapon::onDeinitialize() {
	if (mInteractor != nullptr)
		delete mInteractor;
	if (mFiringSound != nullptr)
		delete mFiringSound;
	if (mReloadingBeginSound != nullptr)
		delete mReloadingBeginSound;
	if (mReloadingDoneSound != nullptr)
		delete mReloadingDoneSound;
	if (mPhysicsBody != nullptr)
		delete mPhysicsBody;
}

void Weapon::fire() {
	if (mCurAmmo > 0) {
		if (mInteractor->isReady()) {
			if(mFiringSound != nullptr) {
				mFiringSound->stopSound();
				mFiringSound->playSound();
			}

			this->mInteractor->check();
			setCurAmmo(mCurAmmo - 1);
		}
	} else {
		this->reload();
	}
}

void Weapon::attack(bool is_press) {
	mIsPressed = is_press;
	if (!mIsOneShot) {
		if (mIsPressed)
		{
			fire();
			mIsPressed = 0;
		}
	} else {
		if (mIsPressed) {
			fire();
		}
	}
}

void Weapon::reload() {
	if (mCurClip > 0 && mReloadTimer == nullptr && mCurAmmo < mAmmoPerClip) {
		mReloadTimer = new dt::Timer("Weapon reloaded", mReloadTime, false);
		mReloadingBeginSound->playSound();
		if (!QObject::connect(mReloadTimer, SIGNAL(timerStoped()),
								this,      SLOT(_onReloadCompleted()))) {
			dt::Logger::get().error("Cannot connect weapon " + this->getName() +
                    "'s _OnReloadCompleted slot with its reload timer's signal");
		}
	}
}

void Weapon::_onHit(dt::PhysicsBodyComponent* hit) {
    Entity* obj = dynamic_cast<Entity*>(hit->getNode());

    if (obj != nullptr)
    {
		uint16_t curHealth = obj->getCurHealth();
		if (curHealth > mPower)	
			obj->setCurHealth(curHealth - mPower);
		else
			obj->setCurHealth(0);
	}
}

void Weapon::_onReloadCompleted() {
    if (mCurClip > 0 && mCurAmmo < mAmmoPerClip) {
        delete mReloadTimer;
        mReloadTimer = nullptr;

        mReloadingDoneSound->playSound();
        this->setCurClip(mCurClip - 1);
        this->setCurAmmo(mAmmoPerClip);
    }
}
