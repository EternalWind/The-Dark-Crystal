#ifndef THE_DARK_CRYSTAL_BATTLE_STATE
#define THE_DARK_CRYSTAL_BATTLE_STATE

#include <Scene/State.hpp>

/**
  * 
  *
  */
class BattleState : public dt::State {
    Q_OBJECT
public:
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);
};

#endif
