#ifndef THE_DARK_CRYSTAL_HUMAN_AGENT
#define THE_DARK_CRYSTAL_HUMAN_AGENT

#include "Agent.h"

#include <Input/InputManager.hpp>
#include <Graphics/CameraComponent.hpp>

/**
  * 玩家代理。用来将玩家的输入翻译成虚拟指令，发送给Entity类。
  */
class HumanAgent : Agent {

    Q_OBJECT

public:
    /**
      * 构造函数。
      * @param name 该代理的名字
      */
    HumanAgent(const QString name);

    void onInitialize();

    void onDeinitialize();

protected slots:
    void __onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg& event);

    void __onKeyUp(dt::InputManager::InputCode code, const OIS::EventArg& event);

    void __onMouseMove(const OIS::MouseEvent& event);

private:
    dt::CameraComponent* mCameraComponent;          //!< 玩家的摄像机。
};

#endif