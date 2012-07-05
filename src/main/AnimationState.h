#ifndef THE_DARK_CRYSTAL_ANIMATIONSTATE
#define THE_DARK_CRYSTAL_ANIMATIONSTATE

#include "Animation.h"

#include <QString>
#include <Scene/State.hpp>
#include <Scene/StateManager.hpp>
#include <Input/InputManager.hpp>

/**
  * �����࣬���ڲ��Ź�������
  */
class AnimationState : public dt::State {
	Q_OBJECT
public:
	AnimationState(const QString &filename, double time, dt::State* next_state);

	~AnimationState();

	void onInitialize();

	void onDeinitialize();

	void updateStateFrame(double simulation_frame_time);

public slots:
	/**
	  * ����ESC��ʱֱ����������
	  * @param code ����İ���
	  */
	void onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg &event);

private:
	Animation *mAnimationPtr;   //!< �����Ŷ���ָ��
	double mTime;               //!< ��������ʱ��
	double mCurTime;            //!< �����Ѳ���ʱ��
	QString mFileName;          //!< �����ļ���
    dt::State* mNextState;      //!< ���궯����Ҫ�����State
};

#endif