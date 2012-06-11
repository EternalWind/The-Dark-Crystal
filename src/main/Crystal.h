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
	
	Crystal(uint16_t unlock_time);
	
	~Crystal();

	uint16_t getUnlockTime()const;

	void setUnlockTime(uint16_t unlock_time);

private:
	uint16_t mUnlockTime;  //!< 水晶解锁时间 
};


#endif