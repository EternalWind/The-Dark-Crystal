#ifndef TESTSTATE_H_
#define TESTSTATE_H_
#include <Scene/State.hpp>
class TestState: public dt::State{	
public:
	virtual void onInitialize(); 
	virtual void updateStateFrame(double simulation_frame_time);
};
#endif