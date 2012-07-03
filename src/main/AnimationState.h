#ifndef THE_DARK_CRYSTAL_ANIMATIONSTATE
#define THE_DARK_CRYSTAL_ANIMATIONSTATE

#include <QString>
#include <Scene/State.hpp>
#include <Scene/StateManager.hpp>
#include "Animation.h"
#include <Input/InputManager.hpp>

/**
  * �����࣬���ڲ��Ź�������
  */
class AnimationState : public dt::State {
	Q_OBJECT
public:
	AnimationState(const QString &filename, double time);

	~AnimationState();

	void onInitialize();

	void updateStateFrame(double simulation_frame_time);

	/**
	  * ����ESC��ʱֱ����������
	  * @param code ����İ���
	  */
	void onKeyDown(dt::InputManager::InputCode code, OIS::EventArg &event);

private:
	Animation *mAnimationPtr;   //!< �����Ŷ���ָ��
	double mTime;               //!< ��������ʱ��
	double mCurTime;            //!< �����Ѳ���ʱ��
	QString mFileName;           //!< �����ļ���
};

#endif