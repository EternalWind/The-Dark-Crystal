#include "AnimationState.h"

AnimationState::AnimationState(const QString &filename, double time, dt::State* next_state) 
                  : mTime(time), 
                    mCurTime(0), 
                    mFileName(filename), 
                    mAnimationPtr(nullptr),
                    mNextState(next_state) {
}

void AnimationState::onInitialize() {
	auto scene = this->addScene(new dt::Scene("movie_scene"));
	scene->addChildNode(mAnimationPtr = new Animation(mFileName));

	connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
			 this, SLOT(onKeyDown(dt::InputManager::InputCode, const OIS::EventArg &)));
    connect(mAnimationPtr, SIGNAL(sAnimationStopped()), this, SLOT(onAnimationStopped()));

    mAnimationPtr->play();
}

void AnimationState::onDeinitialize() {
	disconnect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
			 this, SLOT(onKeyDown(dt::InputManager::InputCode, const OIS::EventArg &)));
}

AnimationState::~AnimationState() {
}

void AnimationState::updateStateFrame(double simulation_frame_time) {}

void AnimationState::onAnimationStopped() {
    dt::StateManager::get()->setNewState(mNextState);
}

void AnimationState::onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg &event) {
	if (code == dt::InputManager::KC_ESCAPE) {
        mAnimationPtr->stop();
		dt::StateManager::get()->setNewState(mNextState);
    }
}