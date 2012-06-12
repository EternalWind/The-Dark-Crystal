#include "QATrigger.h"

QDomElement QATrigger::serializeToXML(QDomDocument& doc) {
    auto QA_node = doc.createElement("QA_Info");

    auto isEnabled_node = doc.createElement("IsEnable");
    isEnabled_node.setAttribute("value", QATrigger::isEnabled());
    QA_node.appendChild(isEnabled_node);

    return QA_node;
}

void QATrigger::deserializeFromXML(const QDomElement& element) {
    auto is_enable_node = element.firstChildElement("IsEnable");
    bool is_enable_value = is_enable_node.attribute("value").toUInt();
    if (is_enable_value) {
        enable();
    } else {
        disable();
    }
}

void QATrigger::onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment) {
    emit sTriggerQA();
}
