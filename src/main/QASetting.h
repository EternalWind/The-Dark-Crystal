#ifndef THE_DARK_CRYSTAL_QASETTING
#define THE_DARK_CRYSTAL_QASETTING

class QASetting {
public:
    QASetting ();
    bool getIsQAEnable () const;
    void setIsQAEnable (const bool is_enable);
private:
    bool mIsEnable;
};


#endif