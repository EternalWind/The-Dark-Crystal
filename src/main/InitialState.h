#ifndef THE_DARK_CRYSTAL_INITIAL_STATE
#define THE_DARK_CRYSTAL_INITIAL_STATE

#include <Scene/State.hpp>

/**
  * һ����������ȫ�ֳ�ʼ�����ࡣ
  */
class InitialState : public dt::State {

    Q_OBJECT

public:
    /**
      * Ĭ�Ϲ��캯����
      */
    InitialState();

    void onInitialize();

    void onDeinitialize();

    void updateStateFrame(double simulation_frame_time);
};

#endif