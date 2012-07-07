#include "spaceShipAIAgent.h"
#include "EntityManager.h"

void spaceShipAIAgent::onUpdate(double time_diff) {
    if (time_diff == 0) return; 

    Ogre::Vector3 human_pos = EntityManager::get()->getHumanShip()->getPosition();
    Ogre::Quaternion cur_q = mBody->getRotation(); 
    mBody->lookAt(human_pos);

    Ogre::Quaternion expect_q = mBody->getRotation(); 
    mBody->setRotation(cur_q);
    Ogre::Quaternion d_q = cur_q - expect_q; 
    
    emit(sLookAround(d_q));

  
    static bool flag = 1; 
    if (flag) {
        emit(sMove(Entity::BACKWARD, true));
        flag = 0; 
    }
   
   /* mBody->findComponent<dt::PhysicsBodyComponent>(Vehicle::PHYSICS_BODY_COMPONENT)->setMass(mBody->getMass());
    mBody->resetPhysicsBody();
*/

}

spaceShipAIAgent::spaceShipAIAgent(QString name):Agent(name) {
}

void spaceShipAIAgent::setBody(Spaceship * ship) {
    mBody = ship; 

}
Spaceship * spaceShipAIAgent::getBody() {
    return mBody; 
}

void spaceShipAIAgent::onInitialize() {
    setBody(dynamic_cast<Spaceship*>(this->getParent()));
}