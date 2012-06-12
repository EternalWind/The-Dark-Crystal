#include "Vehicle.h"

uint16_t Vehicle::getAttackValue() const {
	return mAttackValue;
}

void Vehicle::setAttackValue(uint16_t attack_value) {
	mAttackValue = attack_value;
}

float Vehicle::getAttackRange() const {
	return mAttackRange;
}

void Vehicle::setAttackRange(float attack_range) {
	mAttackRange = attack_range;
}


Vehicle::Vehicle(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval) 
	: Entity(node_name, mesh_handle, collision_shape_type, mass),
	mAttackValue(attack_value),
	mAttackRange(attack_range),
	mAttackInterval(attack_interval) {
}


void Vehicle::onInitialize() {
	Entity::onInitialize();
}

void Vehicle::onDeinitialize() {
}