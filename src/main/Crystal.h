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

    void onUpdate(double time_diff);

    double getUnlockProgress() const;

    void setUnlockProgress(const double unlock_progress);

    /**
      * �԰ٷֱȵ���ʽ��ȡ�������ȡ�
      * @returns �ٷֱ���ʽ��ʾ�Ľ�������
      */
    double getUnlockProgressPercentage() const;

    /**
      * �԰ٷֱȵ���ʽ���ý������ȡ�
      * @param unlock_progress �԰ٷֱ���ʽ��ʾ�Ľ�������
      */
    void setUnlockProgressPercentage(const double unlock_progress);

    /**
      * ����ˮ���Ƿ��ѽ������������������ʱ����ȣ���
      * @returns ��ˮ���Ƿ��ѽ���
      */
    bool isUnlocked() const;

    /**
      * ��ʼ����ˮ����
      */
    void beginUnlock();

signals:
    /**
      * һ����ˮ���Ľ������ȸı�ʱ������signal��
      * @param crystal �������źŵ�ˮ��
      * @param pre_progress �ı�ǰ�Ľ���
      * @param cur_progress �ı��Ľ���
      */
    void sUnlockProgressChanged(Crystal* crystal, double pre_progress, double cur_progress);

private:
	double mUnlockTime;         //!< ˮ������ʱ��
    double mUnlockProgress;     //!< ˮ����������
    bool mIsUnlocking;          //!< ��ʾ��ˮ���Ƿ����ڱ�������������ֹ���������н������ȵ��½�
};


#endif