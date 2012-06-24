#include "Prop.h"

Prop::Prop() {
}

Prop::Prop(const QString &name, PropType prop_type)
			: mName(name), mPropType(prop_type), mPhysicsBody(nullptr) {
}

Prop::~Prop() {
}

void Prop::onInitialize() {
	this->addComponent(new dt::MeshComponent(mName + ".mesh", mName + ".material", "prop_mesh"));
	mPhysicsBody = this->addComponent(new dt::PhysicsBodyComponent("prop_mesh", "physics_body",
        dt::PhysicsBodyComponent::BOX, 100)).get();
	mPhysicsBody->setGravity(0.0f, 0.0f, 0.0f);
}

QString Prop::getName() const {
	return mName;
}

void Prop::setName(QString name) {
	mName = name;
}

Prop::PropType Prop::getPropType() const {
	return mPropType;
}

void Prop::setPropType(PropType prop_type) {
	mPropType = prop_type;
}