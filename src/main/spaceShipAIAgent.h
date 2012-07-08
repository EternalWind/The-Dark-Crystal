#ifndef THE_DARK_CRYSTAL_SPACE_SHIP_AI_AGENT
#define THE_DARK_CRYSTAL_SPACE_SHIP_AI_AGENT

#include "Agent.h"
#include "Spaceship.h"
class spaceShipAIAgent : public Agent {
Q_OBJECT
public:
    spaceShipAIAgent(QString name); 
    void onInitialize();
    void onDeinitialize(){}
    void onUpdate(double time_diff);
    void setBody(Spaceship * ship); 
    Spaceship * getBody(); 
private:
    Spaceship * mBody; 
};

#endif