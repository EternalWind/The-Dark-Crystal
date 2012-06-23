#ifndef THE_DARK_CRYSTAL_AI_TEST
#define THE_DARK_CRYSTAL_AI_TEST

#include <Scene/State.hpp>

class AITest: public dt::State {

    Q_OBJECT

public:

     virtual void onInitialize();

     virtual void onDeinitialize();

     virtual void updateStateFrame(double simulation_frame_time);

};

#endif