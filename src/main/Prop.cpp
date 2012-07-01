#include "Prop.h"

Prop::Prop() {
}

Prop::Prop(const QString &prop_name, const QString &node_name, PropType prop_type)
			: mPropName(prop_name), mPropType(prop_type), Node(node_name) {
}

Prop::~Prop() {
}

void Prop::onInitialize() {
	this->addComponent(new dt::MeshComponent(mPropName + ".mesh", "", "prop_mesh"));
	auto PhysicsBody = this->addComponent(new dt::PhysicsBodyComponent("prop_mesh", "physics_body",
        dt::PhysicsBodyComponent::BOX, 0)).get();
	PhysicsBody->setGravity(0.0f, 0.0f, 0.0f);
}

QString Prop::getName() const {
	return mPropName;
}

void Prop::setName(QString name) {
	mPropName = name;
}

Prop::PropType Prop::getPropType() const {
	return mPropType;
}

void Prop::setPropType(PropType prop_type) {
	mPropType = prop_type;
}