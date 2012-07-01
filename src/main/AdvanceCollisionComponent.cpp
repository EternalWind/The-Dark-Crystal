

#include <Logic/CollisionComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include "AdvanceCollisionComponent.h"
#include <Graphics/ParticleSystemComponent.hpp>

#include "Entity.h"

AdvanceCollisionComponent::AdvanceCollisionComponent(const QString bullet_handle, const ParticleInfo &fire_back, const ParticleInfo &bomb, bool is_throwable, const QString name)
    : dt::InteractionComponent(name),
      mBulletMeshHandle(bullet_handle), 
      mFireBack(fire_back),
      mBomb(bomb),
      mIsThrowable(is_throwable) {}

uint32_t AdvanceCollisionComponent::autoid = 0;

void AdvanceCollisionComponent::onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end) {
    Ogre::Vector3 impulse;
    impulse = end - start;
    impulse.normalise();

    auto id = autoid ++;
    QString name = QString("bullet") + dt::Utils::toString(id);
	

    std::shared_ptr<dt::Node> bullet = getNode()->getScene()->addChildNode(new dt::Node(name + "_ball_node"));

    bullet->addComponent<dt::MeshComponent>(new dt::MeshComponent(mBulletMeshHandle, "", name));
    bullet->setPosition(start, dt::Node::SCENE);
    std::shared_ptr<dt::PhysicsBodyComponent> bullet_body = bullet->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent(name, "bullet_body"));
    bullet_body->setMass(1.0);

	// create the particle system
    auto p_sys = bullet->addComponent(new dt::ParticleSystemComponent("p_sys" + dt::Utils::toString(id)));
    p_sys->setMaterialName(mFireBack.MaterialHandle);
	p_sys->setParticleCountLimit(mFireBack.ParticleCountLimit);
	p_sys->getOgreParticleSystem()->setDefaultDimensions(mFireBack.DefaultDimensionsWidth, mFireBack.DefaultDimensionsHeight);

    Ogre::ParticleEmitter* e = p_sys->addEmitter("emit1", "Point");
    e->setAngle(Ogre::Degree(mFireBack.degree));
	e->setColour(mFireBack.EmitterColorStart, mFireBack.EmitterColorEnd);
	e->setEmissionRate(mFireBack.EmissionRate);
    e->setParticleVelocity(4.0f, 5.f);
    e->setTimeToLive(mFireBack.TimeToLiveL, mFireBack.TimeToLiveR);

	p_sys->addScalerAffector("scaler", mFireBack.ScalerAffector);
    p_sys->addLinearForceAffector("force", Ogre::Vector3(0, 0, 0));

    Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", dt::Utils::toString(mFireBack.time0).toStdString());
    a->setParameter("colour0", dt::Utils::toString(mFireBack.colour0.x).toStdString() + " " + dt::Utils::toString(mFireBack.colour0.y).toStdString() + " " + dt::Utils::toString(mFireBack.colour0.z).toStdString() + " 1");
    a->setParameter("time1", dt::Utils::toString(mFireBack.time1).toStdString());
    a->setParameter("colour1", dt::Utils::toString(mFireBack.colour1.x).toStdString() + " " + dt::Utils::toString(mFireBack.colour1.y).toStdString() + " " + dt::Utils::toString(mFireBack.colour1.z).toStdString() + " 1");
    a->setParameter("time2", dt::Utils::toString(mFireBack.time2).toStdString());
    a->setParameter("colour2", dt::Utils::toString(mFireBack.colour2.x).toStdString() + " " + dt::Utils::toString(mFireBack.colour2.y).toStdString() + " " + dt::Utils::toString(mFireBack.colour2.z).toStdString() + " 0");

    if(!QObject::connect(bullet_body.get(), SIGNAL(collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)),
                         this,        SLOT(onHit(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection)) {
            dt::Logger::get().error("Cannot connect the bullet's collided signal with the OnHit slot.");
    }

    bullet_body->applyCentralImpulse((impulse * mRange).x, (impulse * mRange).y, (impulse * mRange).z);
}

void AdvanceCollisionComponent::onTrigger(AdvanceTriggerAreaComponent* trigger_area, dt::Component* component) {
    Entity* obj = dynamic_cast<Entity*>(component->getNode());
    if (obj != nullptr) {
        emit sHit(obj->findComponent<dt::PhysicsBodyComponent>("physics_body").get());
    }
}

void AdvanceCollisionComponent::onHit(dt::PhysicsBodyComponent* hit, dt::PhysicsBodyComponent* bullet) {
    dt::Node* node = bullet->getNode();

	ParticlesEffect *pp = new ParticlesEffect(node->getName() + "_particle", "Test/Particle", "", mBomb);
	pp->setPosition(node->getPosition().x, node->getPosition().y, node->getPosition().z);
	node->getScene()->addChildNode(pp);
	if (mIsThrowable) {
	    auto trigger = (new AdvanceTriggerAreaComponent(new btSphereShape(12.0f), node->getName() + "_Trigger"));
	    connect(trigger, SIGNAL(triggered(AdvanceTriggerAreaComponent*, dt::Component*)), this, SLOT(onTrigger(AdvanceTriggerAreaComponent* , dt::Component* )));
	    node->addComponent<AdvanceTriggerAreaComponent>(trigger);
	} else {
           node->kill();
           emit sHit(hit);
	}
}

void AdvanceCollisionComponent::onInitialize() {
    //Preload the bullet mesh.
    std::shared_ptr<dt::Node> bullet = this->getNode()->getScene()->addChildNode(new dt::Node("preload_bullet"));
    bullet->addComponent(new dt::MeshComponent(mBulletMeshHandle, "", "bullet"));
    bullet->setPosition(0, -100, 0, dt::Node::SCENE);

    bullet->kill();
}

