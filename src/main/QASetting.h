#ifndef THE_DARK_CRYSTAL_QASETTING
#define THE_DARK_CRYSTAL_QASETTING

/** 
  * 设置QA是否开启的类
  */
class QASetting {
public:
    /** 
      * 构造函数
      */
    QASetting ();

    /** 
      * 获取成员变量mIsEnable的值
      * @returns 成员变量mIsEnable
      */
    bool getIsQAEnable () const;

    /** 
      * 设置成员变量mIsEnable的值
      * @param 成员变量mIsEnable
      */
    void setIsQAEnable (const bool is_enable);

private:
    bool mIsEnable;    //!< QA是否开启
};


#endif