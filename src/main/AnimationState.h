#ifndef THE_DARK_CRYSTAL_ANIMATION_STATE
#define THE_DARK_CRYSTAL_ANIMATION_STATE

#include <Scene/State.hpp>
#include <Input/InputManager.hpp>

/**
  * ��������
  */
class AnimationState : public dt::State {
    Q_OBJECT
public:
	/**
	  * ������ʼ������
	  */
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

public slots:
    /**
	  * �����¼���Ӧ����
	  * @param code ���̵İ���
	  * @param event �����¼�
	  */
    void onKeyDown(dt::InputManager::InputCode code, OIS::EventArg& event);
	
};

#endif