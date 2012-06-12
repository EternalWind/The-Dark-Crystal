#ifndef THE_DARK_CRYSTAL_QAMANAGER
#define THE_DARK_CRYSTAL_QAMANAGER

#include "Question.h"

#include <Core/Manager.hpp>
#include <QString>
#include <QtXml/QtXml>
#include <cstdint>
#include <map>

/** 
  * 问答系统问题管理类
  */

class QAManager: public dt ::Manager {
public:
    /** 
      * 获取QAManager
      * @returns QAManager 指针
      */
    static QAManager* getInstance();
    
    /** 
      * 从文件获取问题
      * @param 文件路径
      */
    bool loadQuestions(QString path);

    /** 
      * 随机抽取一个问题
      * @param 抽取到的问题的指针
      */
    std::shared_ptr<Question> getRandomQuestion();

    /** 
      * 继承自Manager类的初始化函数
      */
    void initialize();

    /** 
      * 继承自Manager类的deinitialize函数
      */
    void deinitialize();

    /** 
      * 继承自Manager类的析构函数
      */
    ~QAManager();
private:
    std::map<uint16_t, std::shared_ptr<Question> > mQuestions;        //!< 存放问题的数组
    static std::shared_ptr<QAManager> mInstance;                   //!< QAManager 的静态实例 
};

#endif
