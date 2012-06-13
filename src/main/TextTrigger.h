#ifndef THE_DARK_CRYSTAL_TEXTTRIGGER
#define THE_DARK_CRYSTAL_TEXTTRIGGER


#include "Serializable.h"

#include <Scene/Component.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>


/**
  * �����Ի�����
  */
class TextTrigger: public dt::Node, public Serializable {
    Q_OBJECT
public:
  /**
    * �Ա������л�,�̳���Serializable
    * @param doc  ��¼��Ϣ��XML�ļ�
    * @return XML�ļ��ڵ�
    */
    QDomElement serializeToXML(QDomDocument& doc) ;

  /**
    * ��Element�����л�, �̳���Serializable
    * @param doc  XML�ļ��ڵ�
    */
    void deserializeFromXML(const QDomElement& doc);

    uint16_t getTextId () const;

    void setTextId (const uint16_t text_id);

public slots:
    void onTrigger(dt::TriggerAreaComponent* trigger_area, dt::Component* compoment);

signals:
    void sTriggerText(uint16_t text_id);

private:
    uint16_t mTextId;           //!< �ı����
};
#endif