#ifndef THE_DARK_CRYSTAL_QASETTING
#define THE_DARK_CRYSTAL_QASETTING

/** 
  * ����QA�Ƿ�������
  */
class QASetting {
public:
    /** 
      * ���캯��
      */
    QASetting ();

    /** 
      * ��ȡ��Ա����mIsEnable��ֵ
      * @returns ��Ա����mIsEnable
      */
    bool getIsQAEnable () const;

    /** 
      * ���ó�Ա����mIsEnable��ֵ
      * @param ��Ա����mIsEnable
      */
    void setIsQAEnable (const bool is_enable);

private:
    bool mIsEnable;    //!< QA�Ƿ���
};


#endif