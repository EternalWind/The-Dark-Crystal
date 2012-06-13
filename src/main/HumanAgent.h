#ifndef THE_DARK_CRYSTAL_HUMAN_AGENT
#define THE_DARK_CRYSTAL_HUMAN_AGENT

#include "Agent.h"

#include <Input/InputManager.hpp>
#include <Graphics/CameraComponent.hpp>

/**
  * ��Ҵ�����������ҵ����뷭�������ָ����͸�Entity�ࡣ
  */
class HumanAgent : Agent {

    Q_OBJECT

public:
    /**
      * ���캯����
      * @param name �ô��������
      */
    HumanAgent(const QString name);

    void onInitialize();

    void onDeinitialize();

protected slots:
    void __onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg& event);

    void __onKeyUp(dt::InputManager::InputCode code, const OIS::EventArg& event);

    void __onMouseMove(const OIS::MouseEvent& event);

private:
    dt::CameraComponent* mCameraComponent;          //!< ��ҵ��������
};

#endif