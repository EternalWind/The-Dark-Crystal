#ifndef THE_DARK_CRYSTAL_QAMANAGER
#define THE_DARK_CRYSTAL_QAMANAGER

#include "Question.h"

#include <Core/Manager.hpp>
#include <QString>
#include <QtXml/QtXml>
#include <cstdint>
#include <map>

/** 
 *�ʴ�ϵͳ���������
 */

class QAManager: public dt ::Manager {
public:
    /** 
      *��ȡQAManager
      *@return QAManager ָ��
      */
    QAManager* get();
    
    /** 
      *���ļ���ȡ��������

};

#endif
