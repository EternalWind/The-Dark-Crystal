#ifndef THE_DARK_CRYSTAL_QATRIGGER
#define THE_DARK_CRYSTAL_QATRIGGER

#include "Serializable.h"

#include <Scene/Component.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>


/**
  *����QA����
  */
class QATrigger: public dt::Node, public Serializable {
    Q_OBJECT
public:
  /**
    * �Ա������л�,�̳���Serializable
    * @param doc  ��¼��Ϣ��XML�ļ�
    * @returns XML�ļ��ڵ�
    */
    QDomElement serializeToXML(QDomDocument& doc) const;

  /**
    * ��Element�����л�, �̳���Serializable
    * @param doc  XML�ļ��ڵ�
    */
    void deserializeFromXML(const QDomElement& doc);

public slots:
    void onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment);

signals:
    void sTriggerQA();

};


#endif