#include "Weapon.h"
#include "Alien.h"
#include "ConfigurationManager.h"
#include <Logic/CollisionComponent.hpp>
#include <Logic/RaycastComponent.hpp>
#include <OgreProcedural.h>
#include "AdvanceCollisionComponent.h"
#include "Monster.h"
#include "ParticlesEffect.h"
#include <Graphics/ParticleSystemComponent.hpp>
Weapon::Weapon(){
}

Weapon::Weapon( const QString &prop_name,
                const QString &node_name, 
				WeaponType type, 
				uint16_t power, 
				uint16_t cur_clip, 
				uint16_t max_clip, 
				uint16_t weight, 
				uint16_t ammo_per_clip, 
				uint16_t cur_ammo, 
				bool is_one_shot, 
				float interval,
				float reload_time,
				const QString &firing_sound_handle, 
				const QString &reloading_begin_sound_handle, 
				const QString &reloading_done_sound_handle, 
				float hitting_range,
				const ParticleInfo &ammo_fire_back,
				const ParticleInfo &ammo_part_parm)
			  : mWeaponType(type),
				mPower(power), 
				mCurClip(cur_clip), 
				mMaxClip(max_clip), 
				mWeight(weight), 
				mAmmoPerClip(ammo_per_clip), 
				mCurAmmo(cur_ammo),
				mIsOneShot(is_one_shot),
				mIsPhysicsBodyEnabled(false),
				mIsPressed(false),
				mReloadTimer(nullptr),
				mInterval(interval),
				mReloadTime(reload_time),
				mFiringSoundHandle(firing_sound_handle),
				mReloadingBeginSoundHandle(reloading_begin_sound_handle),
				mReloadingDoneSoundHandle(reloading_done_sound_handle),
				mInteractor(nullptr),     
				mFiringSound(nullptr),		  
				mReloadingBeginSound(nullptr),    
				mReloadingDoneSound(nullptr),  
				mHittingRange(hitting_range),
				mAmmoFireBack(ammo_fire_back),
				mAmmoBomb(ammo_part_parm),
				mHasMuzzle(false),
				mMuzzlePos(Ogre::Vector3(0, 0, 0)),
				Prop(prop_name, node_name, WEAPON) { 
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

void Weapon::loadMuzzleInfo(const ParticleInfo &muzzle_particle, const Ogre::Vector3& muzzle_pos) {
	mHasMuzzle = true;
	mMuzzlePos = muzzle_pos;
	auto muzzle_node = this->findChildNode(this->getName() + "_muzzle_node");
	muzzle_node->setPosition(mMuzzlePos);
	auto p_sys = muzzle_node->addComponent(new dt::ParticleSystemComponent(this->getName() + "_muzzle_p_sys"));
    p_sys->setMaterialName(muzzle_particle.MaterialHandle);
	p_sys->setParticleCountLimit(muzzle_particle.ParticleCountLimit);
	p_sys->getOgreParticleSystem()->setDefaultDimensions(muzzle_particle.DefaultDimensionsWidth, muzzle_particle.DefaultDimensionsHeight);

    Ogre::ParticleEmitter* e = p_sys->addEmitter("emit1", muzzle_particle.EmitterType);
    e->setAngle(Ogre::Degree(muzzle_particle.degree));
	e->setColour(muzzle_particle.EmitterColorStart, muzzle_particle.EmitterColorEnd);
	e->setEmissionRate(muzzle_particle.EmissionRate);
    e->setParticleVelocity(6.0f, 8.0f);
    e->setTimeToLive(muzzle_particle.TimeToLiveL, muzzle_particle.TimeToLiveR);

	p_sys->addScalerAffector("scaler", muzzle_particle.ScalerAffector);
    p_sys->addLinearForceAffector("force", Ogre::Vector3(0, 0, 0));

    Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.time0)));
    a->setParameter("colour0", dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour0.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour0.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour0.z)) + " 1");
    a->setParameter("time1", dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.time1)));
    a->setParameter("colour1", dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour1.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour1.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour1.z)) + " 1");
    a->setParameter("time2", dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.time2)));
    a->setParameter("colour2", dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour2.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour2.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(muzzle_particle.colour2.z)) + " 0");
    
	muzzle_node->disable();
}

void Weapon::onInitialize() {
    Prop::onInitialize();
    auto node = this->addChildNode(new Node("ammo_node"));
    if (mWeaponType == PRIMARY) {
		OgreProcedural::SphereGenerator().setRadius(0.01f).setUTile(.5f).realizeMesh("Bullet1");
        mInteractor = node->addComponent(new AdvanceCollisionComponent("Bullet1", mAmmoFireBack, mAmmoBomb, 0, "interactor")).get(); 
	
        auto muzzle_node = this->addChildNode(new Node(this->getName() + "_muzzle_node"));
        muzzle_node->setPosition(mMuzzlePos);
	//muzzle_node->setRotation(Ogre::Quaternion(1, 0, -0.44, 0));

    } else if(mWeaponType == SECONDARY){
		OgreProcedural::SphereGenerator().setRadius(0.01f).setUTile(.5f).realizeMesh("Bullet2");
        mInteractor = node->addComponent(new AdvanceCollisionComponent("Bullet2", mAmmoFireBack, mAmmoBomb, 0, "interactor")).get(); 
        auto muzzle_node = this->addChildNode(new Node(this->getName() + "_muzzle_node"));
        muzzle_node->setPosition(mMuzzlePos);
	} else {
		OgreProcedural::SphereGenerator().setRadius(0.2f).setUTile(.5f).realizeMesh("Bullet3");
		//mInteractor = node->addComponent(new dt::RaycastComponent("interactor")).get();
        mInteractor = node->addComponent(new AdvanceCollisionComponent("Bullet3", mAmmoFireBack, mAmmoBomb, 1, "interactor")).get();
        node->setRotation(Ogre::Quaternion(0.9f, 0.35f, 0, 0));
	}
    mInteractor->setIntervalTime(mInterval);
    mInteractor->setRange(mHittingRange);
    mInteractor->setOffset(5.0);

    mIsPhysicsBodyEnabled = true;

	auto conf_mgr = ConfigurationManager::getInstance() ;
    SoundSetting sound_setting = conf_mgr->getSoundSetting();

    if (!QObject::connect(mInteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
                         this,        SLOT(_onHit(dt::PhysicsBodyComponent*)))) {
        dt::Logger::get().error("Cannot connect the sHit signal with the OnHit slot.");
    }

    if (mFiringSoundHandle != "") {
        mFiringSound = this->addComponent(new dt::SoundComponent(mFiringSoundHandle,
            this->getName() + "_firing_sound")).get();
        mFiringSound->setVolume((float)sound_setting.getSoundEffect());
    }

    if (mReloadingBeginSoundHandle != "") {
        mReloadingBeginSound = this->addComponent(new dt::SoundComponent(mReloadingBeginSoundHandle,
            this->getName() + "_reloading_begin_sound")).get();
        mReloadingBeginSound->setVolume((float)sound_setting.getSoundEffect());
    }

    if (mReloadingDoneSoundHandle != "") {
        mReloadingDoneSound = this->addComponent(new dt::SoundComponent(mReloadingDoneSoundHandle,
            this->getName() + "_reloading_done_sound")).get();
        mReloadingDoneSound->setVolume((float)sound_setting.getSoundEffect());
    }
}

void Weapon::onDeinitialize() {
}

void Weapon::fire() {
	auto node = this->getParent()->findChildNode(this->getName() + "_muzzle_node");
	if (mCurAmmo > 0) {
		if (mInteractor->isReady()) {
			if(mFiringSound != nullptr) {
				mFiringSound->stopSound();
				mFiringSound->playSound();
			}
			if (node) {
				node->enable();
			}
			this->mInteractor->check();
			setCurAmmo(mCurAmmo - 1);
			return ;
		}
	} else {
		this->reload();
		if (node) {
			node->disable();
		}
	}
	
}

void Weapon::attack(bool is_pressed) {
	mIsPressed = is_pressed;
}

void Weapon::onUpdate(double time_diff) {
	Node::onUpdate(time_diff);
	auto node = this->getParent()->findChildNode(this->getName() + "_muzzle_node");
	if (mIsPressed) {
		if (mIsOneShot) {
			fire();
			mIsPressed = 0;
			
		} else {
			fire();
		}
	} else {
		if (node) {
			node->disable();
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
    Monster* obj = dynamic_cast<Monster*>(hit->getNode());

    if (obj != nullptr)
    {
		uint16_t curHealth = obj->getCurHealth();
		if (curHealth > mPower)	
			obj->setCurHealth(curHealth - mPower);
		else
			obj->setCurHealth(0);
		if (obj->getCurHealth() == 0)
			obj->onKilled();
	}
}

void Weapon::_onReloadCompleted() {
    if (mCurClip > 0 && mCurAmmo < mAmmoPerClip) {
        delete mReloadTimer;
        mReloadTimer = nullptr;
		mReloadingBeginSound->stopSound();
        mReloadingDoneSound->playSound();
        this->setCurClip(mCurClip - 1);
        this->setCurAmmo(mAmmoPerClip);
    }
}
