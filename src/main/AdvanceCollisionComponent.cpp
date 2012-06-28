

#include <Logic/CollisionComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include "AdvanceCollisionComponent.h"
#include <Graphics/ParticleSystemComponent.hpp>
#include <OgreParticleAffector.h>


AdvanceCollisionComponent::AdvanceCollisionComponent(const QString bullet_handle, const QString name)
    : dt::InteractionComponent(name),
      mBulletMeshHandle(bullet_handle) {}

uint32_t AdvanceCollisionComponent::autoid = 0;

void AdvanceCollisionComponent::onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end) {
    Ogre::Vector3 impulse;
    impulse = end - start;
    impulse.normalise();

    auto id = autoid ++;
    QString name = QString("bullet") + dt::Utils::toString(id);

    std::shared_ptr<dt::Node> bullet = getNode()->getScene()->addChildNode(new dt::Node(QString(id)));

    bullet->addComponent<dt::MeshComponent>(new dt::MeshComponent(mBulletMeshHandle, "", name));
    bullet->setPosition(start, dt::Node::SCENE);
    std::shared_ptr<dt::PhysicsBodyComponent> bullet_body = bullet->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent(name, "bullet_body"));
    bullet_body->setMass(1.0);

	// create the particle system
    auto p_sys = bullet->addComponent(new dt::ParticleSystemComponent("p_sys" + dt::Utils::toString(id)));
    p_sys->setMaterialName("Test/Particle");
    p_sys->setParticleCountLimit(1000);
    p_sys->getOgreParticleSystem()->setDefaultDimensions(0.9, 0.9);

    Ogre::ParticleEmitter* e = p_sys->addEmitter("emit1", "Point");
    e->setAngle(Ogre::Degree(45));
    e->setColour(Ogre::ColourValue(1.f, 0.6f, 0.f), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
    e->setEmissionRate(100);
    e->setParticleVelocity(4.0f, 5.f);
    e->setTimeToLive(0.1f, 0.3f);

    p_sys->addScalerAffector("scaler", 3.05);
    p_sys->addLinearForceAffector("force", Ogre::Vector3(0, 0, 0));

    Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", "0");
    a->setParameter("colour0", "0 1 1 1");
    a->setParameter("time1", "0.5");
    a->setParameter("colour1", "0 0.3 1 1");
    a->setParameter("time2", "1");
    a->setParameter("colour2", "0 0 1 0");

    if(!QObject::connect(bullet_body.get(), SIGNAL(collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)),
                         this,        SLOT(onHit(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection)) {
            dt::Logger::get().error("Cannot connect the bullet's collided signal with the OnHit slot.");
    }

    bullet_body->applyCentralImpulse((impulse * mRange).x, (impulse * mRange).y, (impulse * mRange).z);
}

void AdvanceCollisionComponent::onHit(dt::PhysicsBodyComponent* hit, dt::PhysicsBodyComponent* bullet) {
    dt::Node* node = bullet->getNode();
    node->kill();

    emit sHit(hit);
}

void AdvanceCollisionComponent::onInitialize() {
    //Preload the bullet mesh.
    std::shared_ptr<dt::Node> bullet = this->getNode()->getScene()->addChildNode(new dt::Node("preload_bullet"));
    bullet->addComponent(new dt::MeshComponent(mBulletMeshHandle, "", "bullet"));
    bullet->setPosition(0, -100, 0, dt::Node::SCENE);

    bullet->kill();
}

