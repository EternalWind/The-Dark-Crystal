#include "AnimationState.h"


#include "BattleState.h"

AnimationState::AnimationState(const QString &filename, double time) 
                  : mTime(time), mCurTime(0), mFileName(filename), mAnimationPtr(nullptr) {
}

void AnimationState::onInitialize() {
	auto scene = this->addScene(new dt::Scene("movie_scene"));
	scene->addChildNode(mAnimationPtr = new Animation(mFileName));
	connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
			 this, SLOT(onKeyDown(dt::InputManager::InputCode, const OIS::EventArg &)));
}

void AnimationState::onDeinitialize() {
	disconnect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
			 this, SLOT(onKeyDown(dt::InputManager::InputCode, const OIS::EventArg &)));
}

AnimationState::~AnimationState() {
}

void AnimationState::updateStateFrame(double simulation_frame_time) {
	mCurTime += simulation_frame_time;
	if (mCurTime > mTime) {
		dt::StateManager::get()->setNewState(new BattleState("01"));
	}
}

void AnimationState::onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg &event) {
	if (code == dt::InputManager::KC_ESCAPE)
		dt::StateManager::get()->setNewState(new BattleState("01"));
}