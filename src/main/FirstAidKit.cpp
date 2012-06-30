#include "FirstAidKit.h"

FirstAidKit::FirstAidKit() {
}

FirstAidKit::FirstAidKit(const QString &prop_name, const QString &node_name, uint16_t recovery_val) 
					: mRecoveryVal(recovery_val), Prop(prop_name, node_name, FIRST_AID_KIT){
	
}

FirstAidKit::~FirstAidKit() {
}
	
uint16_t FirstAidKit::getRecoveryVal() const {
	return mRecoveryVal;
}
	
void FirstAidKit::setRecoveryVal(uint16_t recovery_val) {
	mRecoveryVal = recovery_val;
}