#include "ParticlesEffect.h"
#include "ConfigurationManager.h"
#include "SoundSetting.h"

#include <OgreParticleAffector.h>
#include <Graphics/ParticleSystemComponent.hpp>
#include <Audio/SoundComponent.hpp>


ParticlesEffect::ParticlesEffect(const QString node_name, const QString &material_handle, const QString &sound_handle)
	: dt::Node(node_name),
	  mMaterialHandle(material_handle),
      mSoundHandle(sound_handle),
      mRuntime(0) {}

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
	auto p_sys = this->addComponent(new dt::ParticleSystemComponent("p_sys"));
	p_sys->setMaterialName(mMaterialHandle);
	p_sys->setParticleCountLimit(1000);
	p_sys->getOgreParticleSystem()->setDefaultDimensions(1, 1);

	//粒子发射器
	Ogre::ParticleEmitter* e1 = p_sys->addEmitter("emit1", "Point");
	e1->setDirection(Ogre::Vector3(5, 0, 0));
	e1->setAngle(Ogre::Degree(360));
	e1->setColour(Ogre::ColourValue(1.f, 0.6f, 0.f), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
	e1->setEmissionRate(1000);
	e1->setParticleVelocity(10.f, 12.f);
	e1->setTimeToLive(0.2f, 0.2f);
	e1->setDuration(0.2,0.2);

	p_sys->addScalerAffector("scaler", 1.05);

	//设置颜色变化效果
	Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
	a->setParameter("time0", "0");
	a->setParameter("colour0", "1 1 0 1");
	a->setParameter("time1", "0.5");
	a->setParameter("colour1", "1 0.3 0 1");
	a->setParameter("time2", "1");
	a->setParameter("colour2", "1 0 0 0");


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