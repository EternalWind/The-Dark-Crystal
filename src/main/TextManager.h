#ifndef THE_DARK_CRYSTAL_TEXTMANAGER
#define THE_DARK_CRYSTAL_TEXTMANAGER

#include <Core/Manager.hpp>
#include <QString>
#include <QtXml/QtXml>
#include <cstdint>
#include <map>

/** 
  * 对话系统管理类
  */

class TextManager: public dt ::Manager {
public:
    /** 
      * 获取TextManager
      * @return TextManager 指针
      */
    static TextManager* getInstance();
    
    /** 
      * 从文件获取文本
      * @param 文件路径
      */
    bool loadTexts(QString path);

    /** 
      * 根据TextId获取文本
      * @param 文本内容
      */
    QString getText(uint16_t text_id);

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
    ~TextManager();
private:
    std::map<uint16_t, QString> mTexts;        //!< 存放文本的数组
    static std::shared_ptr<TextManager> mInstance;     //!< TextManager 的静态实例 
};

#endif
