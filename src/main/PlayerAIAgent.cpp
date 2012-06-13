#include "PlayerAIAgent.h"
#include "AIDivideAreaManager.h"

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
    Ogre::Vector3 nxt_area_position = AIDivideAreaManager::get()->getPositionById(mNxtArea);    
    Ogre::Vector3 cur_position = mBody->getPosition();     
    Ogre::Vector3 dv = nxt_area_position - cur_position; 
    dv.y = 0; 
    
//    cur_rotation.getPitch
	
}
void PlayerAIAgent::guard() {
}
void PlayerAIAgent::decision() {
}
void PlayerAIAgent::onUpdate(double time_diff) {
}
void PlayerAIAgent::initialize() {
}

