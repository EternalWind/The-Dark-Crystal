#ifndef THE_DARK_CRYSTAL_SERIALIZABLE
#define THE_DARK_CRYSTAL_SERIALIZABLE

#include <QtXml/QtXml>

/**
  * �Ա����������л��Ľӿ�
  *
  */
class Serializable {
public:
  /**
    * �Ա������л�
    * @param doc  ��¼��Ϣ��XML�ļ�
    * @returns XML�ļ��ڵ�
    */
    virtual QDomElement serializeToXML(QDomDocument& doc) = 0;

  /**
    * ��Element�����л�
    * @param doc  XML�ļ��ڵ�
    */
    virtual void deserializeFromXML(QDomElement& doc) = 0;
};

#endif