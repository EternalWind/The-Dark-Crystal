#include "EntityManager.h"


void EntityManager::initialize() {
}

void EntityManager::deinitialize() {
}

EntityManager* EntityManager::get() {
	static EntityManager * singleton = new EntityManager(); 
	return singleton;
}