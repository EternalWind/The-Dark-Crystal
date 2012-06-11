#ifndef THE_DARK_CRYSTAL_QATRIGGER
#define THE_DARK_CRYSTAL_QATRIGGER

#include "Serializable.h"

#include <Scene/Component.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>


/**
  *触发QA的类
  */
class QATrigger: public dt::Node, public Serializable {
    Q_OBJECT
public:
  /**
    * 对变量序列化,继承自Serializable
    * @param doc  记录信息的XML文件
    * @returns XML文件节点
    */
    QDomElement serializeToXML(QDomDocument& doc) const;

  /**
    * 对Element反序列化, 继承自Serializable
    * @param doc  XML文件节点
    */
    void deserializeFromXML(const QDomElement& doc);

public slots:
    void onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment);

signals:
    void sTriggerQA();

};


#endif