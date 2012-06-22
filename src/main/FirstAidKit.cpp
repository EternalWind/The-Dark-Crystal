#include "FirstAidKit.h"

FirstAidKit::FirstAidKit() {
}

FirstAidKit::FirstAidKit(const QString &name, uint16_t recovery_val) 
					: mRecoveryVal(recovery_val), Prop(name, FIRST_AID_KIT){
	
}

FirstAidKit::~FirstAidKit() {
}
	
uint16_t FirstAidKit::getRecoveryVal() const {
	return mRecoveryVal;
}
	
void FirstAidKit::setRecoveryVal(uint16_t recovery_val) {
	mRecoveryVal = recovery_val;
}