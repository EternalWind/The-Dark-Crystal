#include "Spaceship.h"
#include "Agent.h"
#include "Alien.h"

#include "ConfigurationManager.h"

#include <Logic/RaycastComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/ParticleSystemComponent.hpp>

const QString Spaceship::FLYING_SOUND_COMPONENT = "flying_sound";
const QString Spaceship::RISE_SOUND_COMPONENT = "rise_sound";
const QString Spaceship::FALL_SOUND_COMPONENT = "fall_sound";
const float Spaceship::MAX_LEAN_ANGLE = 16.0f;
const float Spaceship::ANGLE_PER_MOVE = Spaceship::MAX_LEAN_ANGLE / 1024;

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
	const float parallel_move_speed)
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, 
	attack_value, attack_range, attack_interval, attack_sound_handle),
	mFlyingSoundHandle(flying_sound_handle),
	mRiseSoundHandle(rise_sound_handle),
	mFallSoundHandle(fall_sound_handle),
	mCurAngle(0),
	mMaxSpeed(max_speed),
	mSpeedPerFrame(speed_per_frame),
	mParallelMoveSpeed(parallel_move_speed) {
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

	//设置摄像头位置
	this->setEyePosition(Ogre::Vector3(0, 2, 0));	

	//添加尾焰
	//this->addFlame(
	//	"back_left_flame", 
	//	"Test/Particle",
	//	Ogre::Vector3(-2.5, -0.9, 8.6),
	//	Ogre::Vector3(0, 0, 1)
	//	);

	//this->addFlame(
	//	"back_right_flame",
	//	"Test/Particle",
	//	Ogre::Vector3(2.5, -0.9, 8.6),
	//	Ogre::Vector3(0, 0, 1)
	//	);


	//auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);

	//mesh->setAnimation("takeOff");
	//mesh->setLoopAnimation(true);
	//mesh->playAnimation();
	//
}

void Spaceship::__onGetOffVehicle() {
	// 速度太快或者高度太高就不能下飞船，否则就会被摔死！！！恶狠狠地摔！！！
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

		// 下船之后船为静态物体
		physics_body->setMass(0);
		this->resetPhysicsBody();
	}
}

void Spaceship::onDeinitialize() {
}

void Spaceship::onUpdate(double time_diff) {	
	mIsUpdatingAfterChange = (time_diff == 0);

	auto p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
		
	float moving = 0.0f;
	//处理旋转
	if (mMoveVector.x > 0) {
		if (mCurAngle < MAX_LEAN_ANGLE) {
			mCurAngle += ANGLE_PER_MOVE;
			moving += ANGLE_PER_MOVE;
		}
	} else if (mMoveVector.x < 0) {
		if (mCurAngle > -MAX_LEAN_ANGLE) {
			mCurAngle -= ANGLE_PER_MOVE;
			moving -= ANGLE_PER_MOVE;
		}
	} else {
		if (mCurAngle > 0) {
			mCurAngle -= ANGLE_PER_MOVE;
			moving -= ANGLE_PER_MOVE;
		} else if (mCurAngle < 0) {
			mCurAngle += ANGLE_PER_MOVE;
			moving += ANGLE_PER_MOVE;
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

	dt::Node::onUpdate(time_diff);
}

void Spaceship::addFlame(const QString& name, const QString& flame_name, Ogre::Vector3 position, Ogre::Vector3 direction) {
	auto node = this->addChildNode(new dt::Node(name));
	node->setPosition(position);	

	auto flame = node->addComponent(new dt::ParticleSystemComponent(name));
	flame->setMaterialName(flame_name);
	flame->setParticleCountLimit(500);
	flame->getOgreParticleSystem()->setDefaultDimensions(0.03, 0.03);

	auto emitter = flame->addEmitter(name + "emitter", "Point");
	emitter->setDirection(direction);
	emitter->setAngle(Ogre::Degree(10.0f)); 
	emitter->setColour(Ogre::ColourValue(1.00, 0.83, 0.00, 0.2), Ogre::ColourValue(1.00, 0.99, 0.97, 0.4));
	emitter->setEmissionRate(100);
	emitter->setParticleVelocity(2.0f, 3.0f);
	emitter->setTimeToLive(0.2f, 0.5f);	
	
	flame->addScalerAffector("scaler", 7.0);
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