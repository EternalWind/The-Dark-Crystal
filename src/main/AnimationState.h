#ifndef THE_DARK_CRYSTAL_ANIMATION_STATE
#define THE_DARK_CRYSTAL_ANIMATION_STATE

#include <Scene/State.hpp>
#include <Input/InputManager.hpp>

/**
  * 动画界面
  */
class AnimationState : public dt::State {
    Q_OBJECT
public:
	/**
	  * 场景初始化函数
	  */
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

public slots:
    /**
	  * 键盘事件响应函数
	  * @param code 键盘的按键
	  * @param event 触发事件
	  */
    void onKeyDown(dt::InputManager::InputCode code, OIS::EventArg& event);
	
};

#endif