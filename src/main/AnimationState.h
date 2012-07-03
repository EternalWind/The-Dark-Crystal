#ifndef THE_DARK_CRYSTAL_ANIMATIONSTATE
#define THE_DARK_CRYSTAL_ANIMATIONSTATE

#include "Animation.h"

#include <QString>
#include <Scene/State.hpp>
#include <Scene/StateManager.hpp>
#include <Input/InputManager.hpp>

/**
  * 动画类，用于播放过场动画
  */
class AnimationState : public dt::State {
	Q_OBJECT
public:
	AnimationState(const QString &filename, double time);

	~AnimationState();

	void updateStateFrame(double simulation_frame_time);

	/**
	  * 按下ESC键时直接跳过动画
	  * @param code 输入的按键
	  */
	void onKeyDown(dt::InputManager::InputCode code, OIS::EventArg &event);

private:
	Animation *mAnimationPtr;   //!< 待播放动画指针
	double mTime;               //!< 动画播放时间
	double mCurTime;            //!< 动画已播放时间
};

#endif