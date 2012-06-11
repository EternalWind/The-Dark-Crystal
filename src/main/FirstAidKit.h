#ifndef THE_DARK_CRYSTAL_FIRSTAIDKIT
#define THE_DARK_CRYSTAL_FIRSTAIDKIT

#include "Prop.h"

/**
  * �����࣬�ظ�Ѫ��
  */
class FirstAidKit: public Prop {
	Q_OBJECT
public:
	FirstAidKit();

	FirstAidKit(const QString &name, uint16_t recovery_val);

	~FirstAidKit();
	
	uint16_t getRecoveryVal()const;
	
	void setRecoveryVal(uint16_t recovery_val);	

private:
	uint16_t mRecoveryVal;  //!< �ɻָ�Ѫ��

};

#endif
