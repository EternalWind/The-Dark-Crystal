#include "QATrigger.h"

QDomElement QATrigger::serializeToXML(QDomDocument& doc) const {

}

void QATrigger::deserializeFromXML(const QDomElement& doc) {


}

void QATrigger::onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment) {
    emit sTriggerQA();
}