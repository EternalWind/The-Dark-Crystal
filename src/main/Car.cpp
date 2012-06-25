#include "Car.h"
#include "Agent.h"

#include <Logic/RaycastComponent.hpp>
#include "ConfigurationManager.h"
#include <math.h>
#include <cstdio>

const QString Car::MOVE_SOUND_COMPONENT = "move_sound";
const QString Car::RUSH_SOUND_COMPONENT = "rush_sound";
const float Car::MAX_THETA = 39.6 / 180.0f;
const float Car::THETA_PER_FRAME = Car::MAX_THETA / 2048;

Car::Car(const QString node_name, 
	const QString mesh_handle, 
	const QString launcher_handle,
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString attack_sound_handle,
	const QString move_sound_handle,
	const QString rush_sound_handle,
	const float max_speed,
	const float speed_per_frame) 
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, 
	attack_value, attack_range, attack_interval, attack_sound_handle),
	mMoveSoundHandle(move_sound_handle),
	mRushSoundHandle(rush_sound_handle),
	mLauncherHandle(launcher_handle),
	mMaxSpeed(max_speed),
	mSpeedPerFrame(speed_per_frame) {		
}

void Car::onInitialize() {
	Vehicle::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

	auto move_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mMoveSoundHandle, MOVE_SOUND_COMPONENT));
	auto rush_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRushSoundHandle, RUSH_SOUND_COMPONENT));

	move_sound->setVolume((float)sound_setting.getSoundEffect());
	rush_sound->setVolume((float)sound_setting.getSoundEffect());

	//设置镜头位置
	//this->setEyePosition(this->getPosition() + Ogre::Vector3(0, 6, 19));	

	//设置汽车长、宽
	btBoxShape* box = dynamic_cast<btBoxShape*>(this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)
													->getRigidBody()->getCollisionShape());
	if (box != nullptr) {
		btVector3 size = box->getHalfExtentsWithoutMargin();
		mWidth = size.x();
		mLength = size.z();
	}

	//设置炮台
	mLauncher = this->addChildNode(new dt::Node("launcher"));
	mLauncher->addComponent<dt::MeshComponent>(new dt::MeshComponent(mLauncherHandle, "", "Launcher"));
	mLauncher->setPosition(0, 2, 0);	
	
	mCurTheta = 0.0f;
	mCurSpeed = 0.0f;
	mMinSpeed = -mMaxSpeed / 2;
	
	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
	physics_body->getRigidBody()->setFriction(0.0f);
	physics_body->setGravity(0, 0, 0);	
}

void Car::onDeinitialize() {
}

void Car::onUpdate(double time_diff) {
	mIsUpdatingAfterChange = (time_diff == 0);

	if (mMoveVector.x < 0) {
		if (mCurTheta > -MAX_THETA) {
			mCurTheta -= THETA_PER_FRAME;
		}
	} else if (mMoveVector.x > 0) {
		if (mCurTheta < MAX_THETA) {
			mCurTheta += THETA_PER_FRAME;
		}
	} else {

		mCurTheta = 0.0;
	}


	//这也算一个仿摩擦力参数了-_- 
	static int mod = 1;

	if (mMoveVector.z < 0) {
		if (mCurSpeed > mMinSpeed) {
			mCurSpeed -= mSpeedPerFrame;
		}
	} else if (mMoveVector.z > 0) {
		if (mCurSpeed < mMaxSpeed) {
			mCurSpeed += mSpeedPerFrame;
		}
	} else {		
		
		mod = (mod + 1) % 8;
		//模仿摩擦力
		if (0 == mod) {
			if (mCurSpeed > 0) {
				mCurSpeed -= mSpeedPerFrame;
			} else if (mCurSpeed > 0) {
				mCurSpeed += mSpeedPerFrame;
			}			
		}
	}

	bool is_changing_dir = (fabs(mCurTheta) > 1e-4);
	float orig_speed = mCurSpeed;

	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
	
	//优化之一： 当转角足够小的时候就不用转向了
	if (is_changing_dir) {
		
		// 获取转过的角度, 位移偏移量
		float dx = 0, dy = 0, alpha = 0;
		__getDelta(dx, dy, alpha);

		int is_leftward = mCurTheta < 0 ? 1 : -1;
		int is_forward = mCurSpeed > 0 ? 1 : -1;

		btTransform trans = physics_body->getRigidBody()->getWorldTransform();
		Ogre::Vector3 position = this->getPosition() + (this->getRotation() * Ogre::Quaternion(Ogre::Radian(alpha), Ogre::Vector3(0, 1, 0)) * Ogre::Vector3(is_leftward * dx, 0, dy * is_forward * 1.0));
		trans.setOrigin(BtOgre::Convert::toBullet(position));
		trans.setRotation(BtOgre::Convert::toBullet(this->getRotation(dt::Node::SCENE) * Ogre::Quaternion(Ogre::Radian(alpha * is_leftward * is_forward), Ogre::Vector3(0, 1, 0))));

		physics_body->getRigidBody()->setWorldTransform(trans);	
	}

	Ogre::Vector3 direction(0, 0, -mCurSpeed);
	auto velocity = BtOgre::Convert::toBullet(this->getRotation(dt::Node::SCENE) * direction);

	if (velocity != physics_body->getRigidBody()->getLinearVelocity()) {
		physics_body->activate();
		physics_body->getRigidBody()->setLinearVelocity(velocity);
	}

	dt::Node::onUpdate(time_diff);
}

void Car::setCurTheta(float theta) {
	mCurTheta = theta;
}

float Car::getCurTheta() {
	return mCurTheta;
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
    //Ogre::Quaternion rotation(body_rot.getYaw(), Ogre::Vector3(0.0f, 1.0f, 0.0f));

    //auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    //btTransform trans;

    //this->findChildNode(Agent::AGENT)->setRotation(agent_rot);

    //physics_body->activate();
    //trans = physics_body->getRigidBody()->getWorldTransform();
    //trans.setRotation(BtOgre::Convert::toBullet(rotation));
    //physics_body->getRigidBody()->setWorldTransform(trans);
}


void Car::__getDelta(float &dx, float &dy, float &alpha, double time_diff) {
	//float dt = 1.0f / 60; //每一帧时间
	float dt = time_diff;

	float theta = std::fabs(mCurTheta);
	float L = std::fabs(this->mCurSpeed) * dt;
	float x1 = -L * std::sin(theta);
	float y1 = L * std::cos(theta);
	float x2 = 0.0f;
	float y2 = -mLength;

	// 斜率不存在时
	if (fabs(x1 - x2) < 1e-9) {
		dx = 0.0f;
		dy = L;
		alpha = 0.0f;
		return;
	}

	float k = (y1 - y2) / (x1 - x2);
	//float a = mAcceleration.z;
	float a = mLength;

	float x0 = 0.0f;
	float y0 = y2 / 2;

	float x3 = x1 + a / std::sqrt(k * k + 1.0f) / 2;
	float y3 = k * a / sqrt(k * k + 1.0f) / 2 + y1;

	dx = x3 - x0;
	dy = y3 - y0;
	alpha = std::atan(fabs(x2 - x3) / fabs(y2 - y3));
}