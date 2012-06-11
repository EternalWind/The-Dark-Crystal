#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"
#include "Alien.h"

class PlayerAIAgent : public Agent {
Q_OBJECT
public:
	bool isOnWay(); 
	void setOnWay(bool type); 	
	Alien* getBody(); 
	void setBody(Alien* body); 
	/**
      * ����mOnWay���״̬Ϊtrue�������ߡ�
      */	
	void walk(); 
	/**
      * ��mThreat״̬Ϊtrue������ԭ�ؾ��䣬��ɱ���        
      */	
	void guard(); 
	/**
      * ��û���κ�״̬Ϊtrue��ʱ��Ҫ��������ѡ���ƶ�����ԭ�ؾ��䡣     
      */	
	void decision(); 
	void onUpdate(double time_diff);
	void initialize();
private:
	Alien* mBody;                          //<! ���������壬Ϊagent�ĸ��ڵ㣬agent�䵱�ſ��������˵Ĵ��ԡ�
	bool mOnWay;                            //<! �Ƿ���·�ϡ����������ƶ����ߣ���û�е���Ŀ�ĵ�֮ǰ�����״̬��Ϊtrue��
	bool mThreat;                           //<! ��thickComponent���������ܵ���в��ʱ�򣬽���guard״̬
	uint16_t mThreatCoolNum;                //<! ���ܵ���в��ʼÿ��update����вϵ��������ȴ��
	uint16_t mCurArea;                      //<! ��ǰ��������ID
	uint16_t mNxtArea;                      //<! ������·�����״̬�������Ŀ�������ID��
};
#endif