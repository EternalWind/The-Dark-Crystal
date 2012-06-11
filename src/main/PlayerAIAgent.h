#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"

class PlayerAIAgent : public Agent {
Q_OBJECT
public:
	bool isOnWay(); 
	void setOnWay(bool type); 
	/**
  * ������·�����״̬��������
  * @param ����
  * @returns ����ֵ
  */	
	void walk(); 
	/**
  * ����������״̬����ԭ�ؾ��䣬��ɱ����
  * @param ����
  * @returns ����ֵ
  */	
	void guard(); 
	/**
  * ��û���κ�״̬��ʱ��Ҫ��������ѡ���ƶ�����ԭ�ؾ���
  * @param ����
  * @returns ����ֵ
  */	
	void decision(); 
private:
	bool mOnWay; //<! �Ƿ���·�ϡ����������ƶ����ߣ���û�е���Ŀ�ĵ�֮ǰ��������·�����״̬
	bool mThreat; //<! ��thickComponent���������ܵ���в��ʱ�򣬽���guard״̬
	uint16_t mThreatCoolNum;  //<! ���ܵ���в��ʼÿ��update����вϵ��������ȴ��
	uint16_t mCurArea;  //<! ��ǰ��������ID
	uint16_t mNxtArea;  //<! ������·�����״̬�������Ŀ�������ID��
};
#endif