#include "FirstAidKit.h"

FirstAidKit::FirstAidKit() {
}

FirstAidKit::FirstAidKit(uint16_t recovery_val) 
					: mRecoveryVal(recovery_val) {
}

FirstAidKit::~FirstAidKit() {
}
	
uint16_t FirstAidKit::getRecoveryVal()const {
	return mRecoveryVal;
}
	
void FirstAidKit::setRecoveryVal(uint16_t recovery_val) {
	mRecoveryVal = recovery_val;
}