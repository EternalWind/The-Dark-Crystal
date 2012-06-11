#include "Prop.h"

Prop::Prop() {
}

Prop::Prop(const QString &name): mName(name) {
}

Prop::~Prop() {
}

QString Prop::getName() {
	return mName;
}
void Prop::setName(QString name) {
	mName = name;
}
Prop::ProType Prop::getPropType() {
	return mPropType;
}