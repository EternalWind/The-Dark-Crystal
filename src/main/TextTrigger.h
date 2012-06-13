#ifndef THE_DARK_CRYSTAL_TEXTTRIGGER
#define THE_DARK_CRYSTAL_TEXTTRIGGER


#include "Serializable.h"

#include <Scene/Component.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>


/**
  * 触发对话的类
  */
class TextTrigger: public dt::Node, public Serializable {
    Q_OBJECT
public:
  /**
    * 对变量序列化,继承自Serializable
    * @param doc  记录信息的XML文件
    * @return XML文件节点
    */
    QDomElement serializeToXML(QDomDocument& doc) ;

  /**
    * 对Element反序列化, 继承自Serializable
    * @param doc  XML文件节点
    */
    void deserializeFromXML(const QDomElement& doc);

    uint16_t getTextId () const;

    void setTextId (const uint16_t text_id);

public slots:
    void onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment);

signals:
    void sTriggerText(uint16_t text_id);

private:
    uint16_t mTextId;           //!< 文本编号
};
#endif