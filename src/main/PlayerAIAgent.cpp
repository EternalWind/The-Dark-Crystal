#include "PlayerAIAgent.h"

bool PlayerAIAgent::isOnWay() {
	return mOnWay;
}

void PlayerAIAgent::setOnWay(bool type) {
	mOnWay = type;
}

Alien* PlayerAIAgent::getBody() {
	return mBody;
}

void PlayerAIAgent::setBody(Alien * body) {
	mBody = body;
}
void PlayerAIAgent::walk() {
	Ogre::Quaternion cur_rotation = mBody->getRotation();
	
}
void PlayerAIAgent::guard() {
}
void PlayerAIAgent::decision() {
}
void PlayerAIAgent::onUpdate(double time_diff) {
}
void PlayerAIAgent::initialize() {
}

