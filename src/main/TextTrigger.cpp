#include <TextTrigger.h>

QDomElement TextTrigger::serializeToXML(QDomDocument& doc) {
    auto Text_node = doc.createElement("Text_Info");

    auto isEnabled_node = doc.createElement("IsEnable");
    isEnabled_node.setAttribute("value", TextTrigger::isEnabled());
    Text_node.appendChild(isEnabled_node);

    return Text_node;
}

void TextTrigger::deserializeFromXML(const QDomElement& element) {
    auto is_enable_node = element.firstChildElement("IsEnable");
    bool is_enable_value = is_enable_node.attribute("value").toUInt();
    if (is_enable_value) {
        enable();
    } else {
        disable();
    }
}

//void TextTrigger::onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment) {
//    uint16_t text_id = getTextId();
//    emit sTriggerText(text_id);
//}

uint16_t TextTrigger::getTextId() const {
    return mTextId;
}

void TextTrigger::setTextId (const uint16_t text_id) {
    mTextId = text_id;
}