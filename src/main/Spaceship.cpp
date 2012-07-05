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
        mAlienRider = nullptr;
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

	//设为静态物体
	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->setMass(0);
	this->resetPhysicsBody();

	this->setCurSpeed(0.0f);

	//添加攻击效果
	if (mBulletHandle == "") {
		OgreProcedural::SphereGenerator().setRadius(0.01f).setUTile(.5f).realizeMesh("Bullet");
		mBulletHandle = "Bullet";
	}

	auto fire_node = this->addChildNode(new dt::Node("fire_node"));
	fire_node->setPosition(this->getEyePosition());
    //fire_node->setRotation(this->getRotation() * Ogre::Quaternion(Ogre::Degree(-30), Ogre::Vector3(0, 1, 0)));
	
	// 这里的interator必须作为成员变量了啊！！！
	mInteractor = fire_node->addComponent(new AdvanceCollisionComponent(mBulletHandle, mAmmoFireBack, mAmmoBomb, false, "interactor"));
	mInteractor->setOffset(20.0f);
    mInteractor->setRange(mAttackRange);
    mInteractor->setIntervalTime(mAttackInterval);

	if (!connect(mInteractor.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), this, SLOT(__onHit(dt::PhysicsBodyComponent*)))) {
		dt::Logger::get().error("Cannot connect the sHit signal with the OnHit slot.");
	}

	// 初始化尾焰
	for (auto iter = mFlameEffect.begin(); iter != mFlameEffect.end(); ++iter) {
		this->addFlame(*iter);
	}

}

void Spaceship::__onGetOffVehicle() {
	// 速度太快或者高度太高就不能下飞船，否则就会被摔死！！！恶狠狠地摔！！！
	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	if (physics_body != nullptr && 
		physics_body->getRigidBody()->getWorldTransform().getOrigin().y() < 30.0f &&
		physics_body->getRigidBody()->getLinearVelocity().length() < 20.0f) {
        // 无人驾驶飞船，直接返回
        if (mAlienRider == nullptr) {
            return;
        }

		Alien* alien = mAlienRider;

		Agent* agent;
		agent = dynamic_cast<Agent*>(this->findChildNode(Agent::AGENT, false).get());

		agent->detach();
		alien->setParent((dt::Node*)this->getScene());

		alien->setPosition(this->getPosition() + Ogre::Vector3(mWidth * 2 + 0.5, 0, 0));        
		alien->findComponent<dt::MeshComponent>(Alien::MESH_COMPONENT)->enable();
		alien->findComponent<dt::PhysicsBodyComponent>(Alien::PHYSICS_BODY_COMPONENT)->enable();        

		agent->attachTo(alien);  

        this->setAlienRider(nullptr);

		// 下船之后船为静态物体
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
	//处理旋转
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

        if (std::fabs(mCurAngle) < 1e-3) {
            mCurAngle = 0.0f;
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
    if (std::fabs(mCurSpeed) < 1e-3) {
        mCurSpeed = 0.0f;
    }
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

	// 处理飞船的攻击
	if (mIsAttacking) {
		if (mInteractor->isReady()) {
			mInteractor->check();
		}
	}

	Entity::onUpdate(time_diff);
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

    a->setParameter("time0", dt::Utils::toStdString(dt::Utils::toString(particle.time0)));
    a->setParameter("colour0", dt::Utils::toStdString(dt::Utils::toString(particle.colour0.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(particle.colour0.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(particle.colour0.z)) + " 1");
    a->setParameter("time1", dt::Utils::toStdString(dt::Utils::toString(particle.time1)));
    a->setParameter("colour1", dt::Utils::toStdString(dt::Utils::toString(particle.colour1.x)) + " " + dt::Utils::toStdString(dt::Utils::toString(particle.colour1.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(particle.colour1.z)) + " 1");
    a->setParameter("time2", dt::Utils::toStdString(dt::Utils::toString(particle.time2)));
    a->setParameter("colour2", dt::Utils::toString(particle.colour2.x).toStdString() + " " + dt::Utils::toStdString(dt::Utils::toString(particle.colour2.y)) + " " + dt::Utils::toStdString(dt::Utils::toString(particle.colour2.z)) + " 0");
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

/* 飞机下降 -_- */
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
	//auto p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	//btTransform trans = p->getRigidBody()->getWorldTransform();
	//trans.setRotation(BtOgre::Convert::toBullet(this->getRotation() * body_rot * agent_rot));
	//p->getRigidBody()->setWorldTransform(trans);
	//auto agent = this->findChildNode(Agent::AGENT);

    // 第二版的LookAround
	Ogre::Matrix3 orientMatrix;
    this->getRotation(dt::Node::SCENE).ToRotationMatrix(orientMatrix);

	Ogre::Radian yaw, pitch, roll;
	orientMatrix.ToEulerAnglesYXZ(yaw, pitch, roll);

	yaw += Ogre::Radian(body_rot.getYaw());
	pitch += Ogre::Radian(agent_rot.getPitch());	

	orientMatrix.FromEulerAnglesYXZ(yaw, pitch, roll);

	Ogre::Quaternion rotation;
	rotation.FromRotationMatrix(orientMatrix);

	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	btTransform trans = physics_body->getRigidBody()->getWorldTransform();
	trans.setRotation(BtOgre::Convert::toBullet(rotation));
	physics_body->getRigidBody()->setWorldTransform(trans);
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

void Spaceship::__onAttack(bool is_pressed) {
    if (is_pressed) {
        mIsAttacking = true;
        this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT)->playSound();
    } else {
        mIsAttacking = false;
        this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT)->stopSound();
    }
}