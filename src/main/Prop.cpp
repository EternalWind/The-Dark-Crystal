#include "Prop.h"

Prop::Prop() {
}

Prop::Prop(const QString &name, Prop::PropType prop_type)
			: mName(name), mPropType(prop_type) {
}

Prop::~Prop() {
}

QString Prop::getName() {
	return mName;
}
void Prop::setName(QString name) {
	mName = name;
}
Prop::PropType Prop::getPropType() {
	return mPropType;
}