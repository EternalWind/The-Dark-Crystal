#include "Crystal.h"

Crystal::Crystal() {
}
	
Crystal::Crystal(const QString &name, uint16_t unlock_time) 
			: mUnlockTime(unlock_time) {
	Prop(name, CRYSTAL);
}
	
Crystal::~Crystal() {
}

uint16_t Crystal::getUnlockTime() const {
	return mUnlockTime;
}

void Crystal::setUnlockTime(uint16_t unlock_time) {	
	mUnlockTime = unlock_time;
}