#ifndef THE_DARK_CRYSTAL_TEXTMANAGER
#define THE_DARK_CRYSTAL_TEXTMANAGER

#include <Core/Manager.hpp>
#include <QString>
#include <QtXml/QtXml>
#include <cstdint>
#include <map>

/** 
  * �Ի�ϵͳ������
  */

class TextManager: public dt ::Manager {
public:
    /** 
      * ��ȡTextManager
      * @return TextManager ָ��
      */
    static TextManager* getInstance();
    
    /** 
      * ���ļ���ȡ�ı�
      * @param �ļ�·��
      */
    bool loadTexts(QString path);

    /** 
      * ����TextId��ȡ�ı�
      * @param �ı�����
      */
    QString getText(uint16_t text_id);

    /** 
      * �̳���Manager��ĳ�ʼ������
      */
    void initialize();

    /** 
      * �̳���Manager���deinitialize����
      */
    void deinitialize();

    /** 
      * �̳���Manager�����������
      */
    ~TextManager();
private:
    std::map<uint16_t, QString> mTexts;        //!< ����ı�������
    static std::shared_ptr<TextManager> mInstance;     //!< TextManager �ľ�̬ʵ�� 
};

#endif
