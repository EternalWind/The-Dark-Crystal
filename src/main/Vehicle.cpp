#include "Vehicle.h"

uint16_t Vehicle::getAttackValue() {
	return mAttackValue;
}

void Vehicle::setAttackValue(uint16_t attack_value) {
	mAttackValue = attack_value;
}

float Vehicle::getAttackRange() {
	return mAttackRange;
}

void Vehicle::setAttackRange(float attack_range) {
	mAttackRange = attack_range;
}
