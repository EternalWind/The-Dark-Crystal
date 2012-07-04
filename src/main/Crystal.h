#ifndef THE_DARK_CRYSTAL_CRYSTAL
#define THE_DARK_CRYSTAL_CRYSTAL
#include "Prop.h"

/**
  * 水晶类
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
      * 以百分比的形式获取解锁进度。
      * @returns 百分比形式表示的解锁进度
      */
    uint16_t getUnlockProgressPercentage() const;

    /**
      * 检查该水晶是否已解锁
      * @returns 该水晶是否已解锁
      */
    bool hasUnlocked() const;

    /**
      * 开始解锁水晶。
      */
    void beginUnlock();

    bool isUnlocking();

signals:
    /**
      * 一个在水晶的解锁进度改变时发出的signal。
      * @param cur_progress 改变后的进度
      */
    void sUnlockCrystalProgressChanged(uint16_t cur_percent); 
    

private:
	double mUnlockTime;         //!< 水晶解锁时间
    double mUnlockProgress;     //!< 水晶解锁进度
    bool mIsUnlocking;          //!< 表示该水晶是否正在被解锁，用以阻止解锁过程中解锁进度的下降
};


#endif