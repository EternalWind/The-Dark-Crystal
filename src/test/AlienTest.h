#ifndef THE_DARK_CRYSTAL_ALIEN_TEST
#define THE_DARK_CRYSTAL_ALIEN_TEST

#include <Scene/State.hpp>

class AlienTest : public dt::State {

    Q_OBJECT

public:
    void onInitialize();

    void updateStateFrame(double simulation_frame_time);
};

#endif