#ifndef THE_DARK_CRYSTAL_CRYSTAL
#define THE_DARK_CRYSTAL_CRYSTAL
#include "Prop.h"

/**
  * ˮ����
  */
class Crystal: public Prop {
	Q_OBJECT
public:
	Crystal();
	
	Crystal(const QString &prop_name, const QString &node_name, double unlock_time);
	
	~Crystal();

	double getUnlockTime()const;

	void setUnlockTime(double unlock_time);

    void onInitialize();

    void onDeinitialize();

    void onUpdate(double time_diff);

    double getUnlockProgress() const;

    void setUnlockProgress(const double& unlock_progress);

    /**
      * �԰ٷֱȵ���ʽ��ȡ�������ȡ�
      * @returns �ٷֱ���ʽ��ʾ�Ľ�������
      */
    uint16_t getUnlockProgressPercentage() const;

    /**
      * ����ˮ���Ƿ��ѽ���
      * @returns ��ˮ���Ƿ��ѽ���
      */
    bool hasUnlocked() const;

    /**
      * ��ʼ����ˮ����
      */
    void beginUnlock();

    bool isUnlocking();

signals:
    /**
      * һ����ˮ���Ľ������ȸı�ʱ������signal��
      * @param cur_progress �ı��Ľ���
      */
    void sUnlockCrystalProgressChanged(uint16_t cur_percent); 
    

private:
	double mUnlockTime;         //!< ˮ������ʱ��
    double mUnlockProgress;     //!< ˮ����������
    bool mIsUnlocking;          //!< ��ʾ��ˮ���Ƿ����ڱ�������������ֹ���������н������ȵ��½�
};


#endif