#ifndef THE_DARK_CRYSTAL_SERIALIZABLE
#define THE_DARK_CRYSTAL_SERIALIZABLE

#include <QtXml/QtXml>

/**
  * 对变量进行序列化的接口
  *
  */
class Serializable {
public:
  /**
    * 对变量序列化
    * @param doc  记录信息的XML文件
    * @returns XML文件节点
    */
    virtual QDomElement serializeToXML(QDomDocument& doc) = 0;

  /**
    * 对Element反序列化
    * @param doc  XML文件节点
    */
    virtual void deserializeFromXML(QDomElement& doc) = 0;
};

#endif