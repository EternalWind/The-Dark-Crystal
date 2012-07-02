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
    p_sys->setMaterialName(mMaterialHandle);
    p_sys->setParticleCountLimit(1000);
    p_sys->getOgreParticleSystem()->setDefaultDimensions(1, 1);

	//粒子发射器
    Ogre::ParticleEmitter* e1 = p_sys->addEmitter("emit1", "Point");
    e1->setDirection(Ogre::Vector3(5, 0, 0));
    e1->setAngle(Ogre::Degree(360));
    e1->setColour(Ogre::ColourValue(0, 0.3f, 1), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
    e1->setEmissionRate(1000);
    e1->setParticleVelocity(10.f, 12.f);
    e1->setTimeToLive(mPartParm.TimeToLiveL, mPartParm.TimeToLiveR);
    e1->setDuration(0.2,0.2);

    p_sys->addScalerAffector("scaler", 1.05);

	//设置颜色变化效果
    Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", dt::Utils::toString(mPartParm.time0).toStdString());
    a->setParameter("colour0", dt::Utils::toString(mPartParm.colour0.x).toStdString() + " " + dt::Utils::toString(mPartParm.colour0.y).toStdString() + " " + dt::Utils::toString(mPartParm.colour0.z).toStdString() + " 1");
    a->setParameter("time1", dt::Utils::toString(mPartParm.time1).toStdString());
    a->setParameter("colour1", dt::Utils::toString(mPartParm.colour1.x).toStdString() + " " + dt::Utils::toString(mPartParm.colour1.y).toStdString() + " " + dt::Utils::toString(mPartParm.colour1.z).toStdString() + " 1");
    a->setParameter("time2", dt::Utils::toString(mPartParm.time2).toStdString());
    a->setParameter("colour2", dt::Utils::toString(mPartParm.colour2.x).toStdString() + " " + dt::Utils::toString(mPartParm.colour2.y).toStdString() + " " + dt::Utils::toString(mPartParm.colour2.z).toStdString() + " 0");


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