#ifndef THE_DARK_CRYSTAL_QAMANAGER
#define THE_DARK_CRYSTAL_QAMANAGER

#include "Question.h"

#include <Core/Manager.hpp>
#include <QString>
#include <QtXml/QtXml>
#include <cstdint>
#include <map>

/** 
 *问答系统问题管理类
 */

class QAManager: public dt ::Manager {
public:
    /** 
      *获取QAManager
      *@return QAManager 指针
      */
    QAManager* get();
    
    /** 
      *从文件获取问题内容

};

#endif
