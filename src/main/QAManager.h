#ifndef THE_DARK_CRYSTAL_QAMANAGER
#define THE_DARK_CRYSTAL_QAMANAGER

#include "Question.h"

#include <Core/Manager.hpp>
#include <QString>
#include <QtXml/QtXml>
#include <cstdint>
#include <map>

/** 
  * �ʴ�ϵͳ���������
  */

class QAManager: public dt ::Manager {
public:
    /** 
      * ��ȡQAManager
      * @return QAManager ָ��
      */
    QAManager* get();
    
    /** 
      * ���ļ���ȡ����
      * @param �ļ�·��
      */
    bool loadQuestions(QString path);

    /** 
      * �����ȡһ������
      * @param ��ȡ���������ָ��
      */
    Question* getRandomQuestion();

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
    ~QAManager();
private:
    std::map<uint16_t, Question*> mQuestions;        //!< ������������
    static std::shared_ptr<QAManager> mInstance;     //!< QAManager �ľ�̬ʵ�� 
};

#endif
