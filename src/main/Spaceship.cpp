#include "Spaceship.h"
#include "Agent.h"
#include "Alien.h"

#include "ConfigurationManager.h"
#include "AdvanceCollisionComponent.h"

#include <OgreProcedural.h>
#include <Logic/RaycastComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/ParticleSystemComponent.hpp>

const QString Spaceship::FLYING_SOUND_COMPONENT = "flying_sound";
const QString Spaceship::RISE_SOUND_COMPONENT = "rise_sound";
const QString Spaceship::FALL_SOUND_COMPONENT = "fall_sound";

Spaceship::Spaceship(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString attack_sound_handle,
	const QString flying_sound_handle,
	const QString rise_sound_handle,
	const QString fall_sound_handle,
	const float max_speed,
	const float speed_per_frame,
	const float max_lean_angle,
	const float angle_per_frame,
	const float parallel_move_speed, 
	const float up_down_speed,
	const ParticleInfo& ammo_fire_back,
	const ParticleInfo& ammo_bomb,
	const std::vector<FlameInfo>& flame_effect,
	const QString& bullet_handle)
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, 
	attack_value, attack_range, attack_interval, attack_sound_handle),
	mFlyingSoundHandle(flying_sound_handle),
	mRiseSoundHandle(rise_sound_handle),
	mFallSoundHandle(fall_sound_handle),
	mCurAngle(0),
	mMaxSpeed(max_speed),
	mSpeedPerFrame(speed_per_frame),
	mMaxLeanAngle(max_lean_angle),
	mAnglePerFrame(angle_per_frame),
	mParallelMoveSpeed(parallel_move_speed),
	mUpDownSpeed(up_down_speed),
	mAmmoFireBack(ammo_fire_back),
	mAmmoBomb(ammo_fire_back),
	mFlameEffect(flame_effect),
	mBulletHandle(bullet_handle) {
		Entity::mEyePosition = Ogre::Vector3(0, 2, 0);
}

void Spaceship::onInitialize() {
	Vehicle::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

	auto flying_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mFlyingSoundHandle, FLYING_SOUND_COMPONENT));
	auto rise_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRiseSoundHandle, RISE_SOUND_COMPONENT));
	auto fall_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mFallSoundHandle, FALL_SOUND_COMPONENT));

	flying_sound->setVolume((float)sound_setting.getSoundEffect());
	rise_sound->setVolume((float)sound_setting.getSoundEffect());
	fall_sound->setVolume((float)sound_setting.getSoundEffect());

	//��Ϊ��̬����
	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->setMass(0);
	this->resetPhysicsBody();

	this->setCurSpeed(0.0f);

	//��������ͷλ��
	this->setEyePosition(Ogre::Vector3(0, 2, 0));	

	//��ӹ���Ч��
	if (mBulletHandle == "") {
		OgreProcedural::SphereGenerator().setRadius(0.02f).setUTile(.5f).realizeMesh("Bullet");
		mBulletHandle = "Bullet";
	}

	auto fire_node = this->addChildNode(new dt::Node("fire_node"));
	fire_node->setPosition(this->getEyePosition());
	
	// �����interator������Ϊ��Ա�����˰�������
	mInteractor = fire_node->addComponent(new AdvanceCollisionComponent(mBulletHandle, mAmmoFireBack, mAmmoBomb, false, "interactor"));
	mInteractor->setOffset(20.0f);

	if (!connect(mInteractor.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), this, SLOT(__onHit(dt::PhysicsBodyComponent*)))) {
		dt::Logger::get().error("Cannot connect the sHit signal with the OnHit slot.");
	}

	// ��ʼ��β��
	for (auto iter = mFlameEffect.begin(); iter != mFlameEffect.end(); ++iter) {
		this->addFlame(*iter);
	}
}

void Spaceship::__onGetOffVehicle() {
	// �ٶ�̫����߸߶�̫�߾Ͳ����·ɴ�������ͻᱻˤ����������ݺݵ�ˤ������
	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	if (physics_body != nullptr && 
		physics_body->getRigidBody()->getWorldTransform().getOrigin().y() < 30.0f &&
		physics_body->getRigidBody()->getLinearVelocity().length() < 20.0f) {
		Alien* alien;
		alien = dynamic_cast<Alien*>(this->findChildNode("alien", false).get());

		Agent* agent;
		agent = dynamic_cast<Agent*>(this->findChildNode(Agent::AGENT, false).get());

		agent->detach();

		alien->setParent((dt::Node*)this->getScene());
		alien->setPosition(this->getPosition() + Ogre::Vector3(mWidth * 2 + 0.5, 0, 0));
		alien->findComponent<dt::MeshComponent>(Alien::MESH_COMPONENT)->enable();
		alien->findComponent<dt::PhysicsBodyComponent>(Alien::PHYSICS_BODY_COMPONENT)->enable();

		agent->attachTo(alien);

		// �´�֮��Ϊ��̬����
		physics_body->setMass(0);
		this->resetPhysicsBody();
	}
}

void Spaceship::onDeinitialize() {
	disconnect(mInteractor.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), this, SLOT(__onHit(dt::PhysicsBodyComponent*)));

	Vehicle::onDeinitialize();
}

void Spaceship::onUpdate(double time_diff) {	
	mIsUpdatingAfterChange = (time_diff == 0);

	auto p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
		
	float moving = 0.0f;
	//������ת
	if (mMoveVector.x > 0) {
		if (mCurAngle < mMaxLeanAngle) {
			mCurAngle += mAnglePerFrame;
			moving += mAnglePerFrame;
		}
	} else if (mMoveVector.x < 0) {
		if (mCurAngle > -mMaxLeanAngle) {
			mCurAngle -= mAnglePerFrame;
			moving -= mAnglePerFrame;
		}
	} else {
		if (mCurAngle > 0) {
			mCurAngle -= mAnglePerFrame;
			moving -= mAnglePerFrame;
		} else if (mCurAngle < 0) {
			mCurAngle += mAnglePerFrame;
			moving += mAnglePerFrame;
		}
	}

	Ogre::Quaternion pre_rot = this->getRotation(dt::Node::SCENE);

	if (mCurAngle != 0) {
		btTransform trans = p->getRigidBody()->getWorldTransform();
		p->activate();

		Ogre::Quaternion rot = pre_rot * Ogre::Quaternion(Ogre::Radian(-moving), Ogre::Vector3(0, 0, 1));
		trans.setRotation(BtOgre::Convert::toBullet(rot));
		p->getRigidBody()->setWorldTransform(trans);
	}

	mCurSpeed += mMoveVector.z * mSpeedPerFrame;
	if (mCurSpeed > mMaxSpeed) {
		mCurSpeed = mMaxSpeed;
	}
	if (mCurSpeed < -mMaxSpeed / 2) {
		mCurSpeed = -mMaxSpeed / 2;
	}

	Ogre::Vector3 direction(
		mMoveVector.x * mParallelMoveSpeed, 
		mMoveVector.y * mParallelMoveSpeed,
		-mCurSpeed
		);
	
	auto velocity = BtOgre::Convert::toBullet(pre_rot * direction);

	if (velocity != p->getRigidBody()->getLinearVelocity()) {
		p->activate();
		p->getRigidBody()->setLinearVelocity(velocity);
	}

	// ����ɴ��Ĺ���
	if (mIsAttacking) {
		if (mInteractor->isReady()) {
			mInteractor->check();
		}
	}

	dt::Node::onUpdate(time_diff);
}

void Spaceship::addFlame(const FlameInfo& flame) {
	QString name = flame.mName;
	ParticleInfo particle = flame.mParticle;

	auto node = this->addChildNode(new dt::Node(name));
	node->setPosition(flame.mPosition);	

	auto p_sys = node->addComponent(new dt::ParticleSystemComponent(name + QUuid::createUuid().toString()));
	p_sys->setMaterialName(particle.MaterialHandle);
	p_sys->setParticleCountLimit(particle.ParticleCountLimit);
	p_sys->getOgreParticleSystem()->setDefaultDimensions(particle.DefaultDimensionsWidth, particle.DefaultDimensionsHeight);

	auto e = p_sys->addEmitter(name + "emitter", "Point");
	e->setDirection(flame.mDirection);
	e->setAngle(Ogre::Degree(particle.degree));
	e->setColour(particle.EmitterColorStart, particle.EmitterColorEnd);
	e->setEmissionRate(particle.EmissionRate);
	e->setParticleVelocity(2.0f, 3.0f);	
	e->setTimeToLive(particle.TimeToLiveL, particle.TimeToLiveR);	
	
	p_sys->addScalerAffector("scaler", particle.ScalerAffector);
	Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");

    a->setParameter("time0", dt::Utils::toString(particle.time0).toStdString());
    a->setParameter("colour0", dt::Utils::toString(particle.colour0.x).toStdString() + " " + dt::Utils::toString(particle.colour0.y).toStdString() + " " + dt::Utils::toString(particle.colour0.z).toStdString() + " 1");
    a->setParameter("time1", dt::Utils::toString(particle.time1).toStdString());
    a->setParameter("colour1", dt::Utils::toString(particle.colour1.x).toStdString() + " " + dt::Utils::toString(particle.colour1.y).toStdString() + " " + dt::Utils::toString(particle.colour1.z).toStdString() + " 1");
    a->setParameter("time2", dt::Utils::toString(particle.time2).toStdString());
    a->setParameter("colour2", dt::Utils::toString(particle.colour2.x).toStdString() + " " + dt::Utils::toString(particle.colour2.y).toStdString() + " " + dt::Utils::toString(particle.colour2.z).toStdString() + " 0");
}

void Spaceship::playFlame(const QString& name) {
	this->findChildNode(name)->findComponent<dt::ParticleSystemComponent>(name)->enable();
}

void Spaceship::stopFlame(const QString& name) {
	this->findChildNode(name)->findComponent<dt::ParticleSystemComponent>(name)->disable();
}

// slots

void Spaceship::__onMove(MoveType type, bool is_pressed) {
	bool is_stopped = false;

	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	switch (type) {
	case FORWARD:
		if (is_pressed)
			mMoveVector.z += 1.0f; 
		else
			mMoveVector.z -= 1.0f;

		break;

	case BACKWARD:
		if (is_pressed)
			mMoveVector.z -= 1.0f;
		else
			mMoveVector.z += 1.0f;

		break;

	case LEFTWARD:
		if (is_pressed) {
			mMoveVector.x -= 1.0f;
		} else {
			mMoveVector.x += 1.0f;
		}

		break;

	case RIGHTWARD:
		if (is_pressed) {
			mMoveVector.x += 1.0f;
		} else {
			mMoveVector.x -= 1.0f;		
		}

		break;

	case STOP:
		is_stopped = true;

		break;

	default:
		dt::Logger::get().debug("Not processed MoveType!");
	}


	mIsMoving = !is_stopped;
}

/* �ɻ��½� -_- */
void Spaceship::__onSpeedUp(bool is_pressed) {

	if (is_pressed) {
		mMoveVector.y -= 1.0f;

		this->findComponent<dt::SoundComponent>(RISE_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::SoundComponent>(FLYING_SOUND_COMPONENT)->playSound();

	} else {
		mMoveVector.y += 1.0f;

		this->findComponent<dt::SoundComponent>(FLYING_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::SoundComponent>(RISE_SOUND_COMPONENT)->playSound();
	}

	mHasSpeededUp = is_pressed;
}

void Spaceship::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
	auto p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	p->activate();

	btTransform trans = p->getRigidBody()->getWorldTransform();
	trans.setRotation(BtOgre::Convert::toBullet(this->getRotation() * body_rot * agent_rot));
	p->getRigidBody()->setWorldTransform(trans);
}

void Spaceship::__onJump(bool is_pressed) {
	if (is_pressed) {
		mMoveVector.y += 1.0f;

		this->findComponent<dt::SoundComponent>(FLYING_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::SoundComponent>(RISE_SOUND_COMPONENT)->playSound();
	} else {
		mMoveVector.y -= 1.0f;

		this->findComponent<dt::SoundComponent>(RISE_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::SoundComponent>(FLYING_SOUND_COMPONENT)->playSound();
	}
}


void Spaceship::setAttackOffset(const float& offset) {
	mInteractor->setOffset(offset);
}

float Spaceship::getAttackOffset() const {
	return mInteractor->getOffset();
}