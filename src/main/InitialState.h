#ifndef THE_DARK_CRYSTAL_INITIAL_STATE
#define THE_DARK_CRYSTAL_INITIAL_STATE

#include <Scene/State.hpp>

/**
  * 一个用来进行全局初始化的类。
  */
class InitialState : public dt::State {

    Q_OBJECT

public:
    /**
      * 默认构造函数。
      */
    InitialState();

    void onInitialize();

    void onDeinitialize();

    void updateStateFrame(double simulation_frame_time);
};

#endif