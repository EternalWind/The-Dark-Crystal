#include "AnimationState.h"


AnimationState::AnimationState(const QString &filename, double time) 
                  : mTime(time), mCurTime(0), mAnimationPtr(nullptr) {
	auto scene = this->addScene(new dt::Scene("tmpscene"));
	scene->addChildNode(mAnimationPtr = new Animation(filename));
}

AnimationState::~AnimationState() {
}

void AnimationState::updateStateFrame(double simulation_frame_time) {
	mCurTime += simulation_frame_time;
	if (mCurTime > mTime)
		dt::StateManager::get()->pop(1);
}

void AnimationState::onKeyDown(dt::InputManager::InputCode code, OIS::EventArg &event) {
	if (code == dt::InputManager::KC_ESCAPE)
		dt::StateManager::get()->pop(1);
}