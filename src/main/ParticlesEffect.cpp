#include "ParticlesEffect.h"
#include "ConfigurationManager.h"
#include "SoundSetting.h"

#include <OgreParticleAffector.h>
#include <Graphics/ParticleSystemComponent.hpp>
#include <Audio/SoundComponent.hpp>


ParticlesEffect::ParticlesEffect(const QString node_name, const QString &material_handle, const QString &sound_handle, const ParticleInfo &part_parm)
	: dt::Node(node_name),
      mMaterialHandle(material_handle),
      mSoundHandle(sound_handle),
      mRuntime(0),
      mPartParm(part_parm) {
	  }

ParticlesEffect::~ParticlesEffect() {
}

void ParticlesEffect::onInitialize() {
    auto conf_mgr = ConfigurationManager::getInstance() ;
    SoundSetting sound_setting = conf_mgr->getSoundSetting();
    auto sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mSoundHandle, "bomb_sound"));
    sound->setVolume((float)sound_setting.getSoundEffect());
    sound->getSound().setLoop(false);
    sound->playSound();  

	// create the particle system
	
    auto p_sys = this->addComponent(new dt::ParticleSystemComponent(this->getName() + QUuid::createUuid().toString() + "_p_sys"));
    p_sys->setMaterialName(mPartParm.MaterialHandle);
	p_sys->setParticleCountLimit(mPartParm.ParticleCountLimit);
    p_sys->getOgreParticleSystem()->setDefaultDimensions(mPartParm.DefaultDimensionsWidth, mPartParm.DefaultDimensionsHeight);

	//粒子发射器
	Ogre::ParticleEmitter* e1 = p_sys->addEmitter("emit1", mPartParm.EmitterType);
    e1->setDirection(Ogre::Vector3(5, 0, 0));
    e1->setAngle(Ogre::Degree(360));
    e1->setColour(mPartParm.EmitterColorStart, mPartParm.EmitterColorEnd);
	e1->setEmissionRate(mPartParm.EmissionRate);
    e1->setParticleVelocity(10.f, 12.f);
    e1->setTimeToLive(mPartParm.TimeToLiveL, mPartParm.TimeToLiveR);
    e1->setDuration(0.2,0.2);

	p_sys->addScalerAffector("scaler", mPartParm.ScalerAffector);

	//设置颜色变化效果
    Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", dt::Utils::toStdString(dt::Utils::toString(mPartParm.time0)));
    a->setParameter("colour0", dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour0.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour0.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour0.z)) + " 1");
    a->setParameter("time1", dt::Utils::toStdString(dt::Utils::toString(mPartParm.time1)));
    a->setParameter("colour1", dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour1.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour1.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour1.z)) + " 1");
    a->setParameter("time2", dt::Utils::toStdString(dt::Utils::toString(mPartParm.time2)));
    a->setParameter("colour2", dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour2.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour2.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(mPartParm.colour2.z)) + " 0");


}

void ParticlesEffect::onDeinitialize() {
}

void ParticlesEffect::onUpdate(double time_diff) {
	//自动销毁
	mRuntime += time_diff;
	if(mRuntime > 2.0) {
		this->kill();
	}
}