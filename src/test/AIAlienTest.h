#ifndef THE_DARK_CRYSTAL_AI_ALIEN_TEST
#define THE_DARK_CRYSTAL_AI_ALIEN_TEST

#include <Scene/State.hpp>
#include <Logic/TriggerAreaComponent.hpp>

class AIAlienTest : public dt::State {

    Q_OBJECT

public:
    AIAlienTest();

    void onInitialize();

    void updateStateFrame(double simulation_frame_time);

    void onDeinitialize();

    void areaTriggered(dt::TriggerAreaComponent* trigger_area, dt::Component* component);
};

#endif