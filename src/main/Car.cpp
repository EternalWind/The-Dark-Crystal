#include "Car.h"
#include "Agent.h"

#include <Logic/RaycastComponent.hpp>
#include "ConfigurationManager.h"
#include <math.h>

const QString Car::MOVE_SOUND_COMPONENT = "move_sound";
const QString Car::RUSH_SOUND_COMPONENT = "rush_sound";

Car::Car(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString attack_sound_handle,
	const QString move_sound_handle,
	const QString rush_sound_handle,
	const float width,
	const float length,
	const float max_speed,
	const float straight_acce,
	const float angular_acce) 
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, 
	attack_value, attack_range, attack_interval, attack_sound_handle),
	mMoveSoundHandle(move_sound_handle),
	mRushSoundHandle(rush_sound_handle),
	mWidth(width),
	mLength(length),
	mMaxSpeed(max_speed),
	mMaxTheta(30.f) {
		mAcceleration = Ogre::Vector3(angular_acce, 0, angular_acce);
}

void Car::onInitialize() {
	Vehicle::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

	auto move_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mMoveSoundHandle, MOVE_SOUND_COMPONENT));
	auto rush_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRushSoundHandle, RUSH_SOUND_COMPONENT));

	move_sound->setVolume((float)sound_setting.getSoundEffect());
	rush_sound->setVolume((float)sound_setting.getSoundEffect());

	//this->setCurSpeed(5.f);
	this->setCurTheta(0.f);
	// 战车摩擦力！擦！擦

	auto p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
	p->getRigidBody()->setFriction(0.0f);
	//p->setCentralForce(0.0f, 10.0f, 0.0f);
	//p->applyCentralImpulse(0.0f, 50.0f, 0.0f);
	mMoveVector.y = 1.0f;
	mCurSpeed = 10.0f;
	p->getRigidBody()->setLinearVelocity(btVector3(10.f, 0.0, 0.0f));
	//p->applyCentralImpulse(10.f, 0.0f, 0.0f);
	btVector3 btv = p->getRigidBody()->getLinearVelocity();
	std::cout << btv.x() << " " << btv.y() << " " << btv.z() << std::endl;

	int a = 2;
	int b = 5;
}

void Car::onDeinitialize() {
}

void Car::onUpdate(double time_diff) {
	static const float EPS = 1e-9;

	// v = v0 + at
	float z_speed = getCurSpeed() + mMoveVector.z * mAcceleration.z * time_diff;
	if (z_speed > getMaxSpeed() + EPS) {
		z_speed = getMaxSpeed();
	}
	if (z_speed + EPS < getMinSpeed()) {
		z_speed = getMinSpeed();
	}
	setCurSpeed(z_speed);

	float x_speed = mMoveVector.x * mAcceleration.x * time_diff;
	float theta = getCurTheta() + x_speed * time_diff;
	if (theta > getMaxTheta()) {
		theta = getMaxTheta();
	}
	if (theta < -getMaxTheta()) {
		theta = -getMaxTheta();
	}

	setCurTheta(theta);

	dt::Node::onUpdate(time_diff);
}

float Car::getLength() const {
	return mLength;
}

float Car::getWidth() const {
	return mWidth;
}

float Car::getMaxTheta() const {
	return mMaxTheta;
}

float Car::getCurTheta() const {
	return mCurTheta;
}

void Car::setCurTheta(const float& cur_theta) {
	mCurTheta = cur_theta;
}

float Car::getMaxSpeed() const {
	return mMaxSpeed;
}

float Car::getMinSpeed() const {
	return mMinSpeed;
}

// slots

void Car::__onMove(MoveType type, bool is_pressed) {
	switch (type) {

	case FORWARD:
		mMoveVector.z += (is_pressed ? 1.0f : -1.0f);
		break;

	case BACKWARD:
		mMoveVector.z += (is_pressed ? -1.0f : 1.0f);
		break;

	case LEFTWARD:
		mMoveVector.x += (is_pressed ? -1.0f : 1.0f);
		break;

	case RIGHTWARD:
		mMoveVector.x += (is_pressed ? 1.0f : -1.0f);
		break;

	case STOP:
		break;

	default:
		dt::Logger::get().debug("Not processed MoveType!");
	}
}

void Car::__onSpeedUp(bool is_pressed) {
	/* 加速键对车来说无法起作用 */
}

void Car::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	physics_body->disable();
	this->setRotation(body_rot);

	auto agent = this->findChildNode(Agent::AGENT);
	agent->setRotation(agent_rot);

	physics_body->getRigidBody()	->setLinearVelocity(BtOgre::Convert::toBullet(this->getRotation(dt::Node::SCENE) * mMoveVector * mCurSpeed));

	physics_body->enable();
}

//void Car::__turnAround(bool is_left, bool is_forward) {
//	float dx, dy, alpha;
//	int flag_x = is_left ? 1 : -1;
//	int flag_y = is_forward ? 1 : -1;
//
//	__getDelta(dx, dy, alpha);
//
//	this->setRotation(Ogre::Quaternion(Ogre::Radian(flag_x * alpha), Ogre::Vector3(0, 1, 0)));
//	this->setPosition(this->getPosition() + Ogre::Vector3(flag_x * dx, 0, flag_y * dy));
//}

//void Car::__moveAround() {
//	float dx, dy, alpha;
//	int flag_x = this->getCurTheta() < 0.0f ? 1 : -1;
//	int flag_y = this->getCurSpeed() > 0.0f ? 1 : -1;
//
//	__getDelta(dx, dy, alpha);
//
//	dt::PhysicsBodyComponent* p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT).get();
//	
//	if (p != nullptr) {
//		p->disable();
//		this->setPosition(this->getPosition());
//		p->enable();
//	}
//	//this->setRotation(this->getParent()->getRotation(dt::Node::SCENE) * Ogre::Quaternion(Ogre::Radian(0.0f), Ogre::Vector3(0.0f, 1.0f, 0.0f)), dt::Node::SCENE);
//	///this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->enable();
//	//this->setPosition(this->getPosition() + Ogre::Vector3(flag_x * dx, 0, flag_y * dy));
//}


//float Car::__getRadian(const float &degree) {
//	return degree / 180.f;
//}
//
//void Car::__getDelta(float &dx, float &dy, float &alpha) {
//	float dt = 1.0f / 60; //每一帧时间
//
//	float theta = std::fabs(this->getCurTheta());
//	float L = std::fabs(this->getCurSpeed()) * dt;
//	float x1 = -L * std::sin(__getRadian(theta));
//	float y1 = L * std::cos(__getRadian(theta));
//	float x2 = 0.0f;
//	float y2 = -getLength();
//
//	// 斜率不存在时
//	if (fabs(x1 - x2) < 1e-9) {
//		dx = 0.0f;
//		dy = L;
//		alpha = 0.0f;
//		return;
//	}
//
//	float k = (y1 - y2) / (x1 - x2);
//	float a = mAcceleration.z;
//
//	float x0 = 0.0f;
//	float y0 = y2 / 2;
//
//	float x3 = x1 + a / std::sqrt(k * k + 1.0f) / 2;
//	float y3 = k * a / sqrt(k * k + 1.0f) / 2 + y1;
//
//	dx = x3 - x0;
//	dy = y3 - y0;
//	alpha = std::atan(fabs(x2 - x3) / fabs(y2 - y3));
//
//}