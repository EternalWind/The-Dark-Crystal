#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"
#include "Alien.h"
#include <Logic/TriggerAreaComponent.hpp>

class PlayerAIAgent : public Agent {
Q_OBJECT
public:
    PlayerAIAgent(); 
	bool isOnWay(); 
	void setOnWay(bool type); 	
	Alien* getBody(); 
	void setBody(Alien* body); 
	/**
      * ����mOnWay���״̬Ϊtrue�������ߡ�
      */	
	void walk(double time_diff); 
	/**
      * ��mThreat״̬Ϊtrue������ԭ�ؾ��䣬��ɱ���        
      */	
	void guard(double time_diff); 
	/**
      * ��û���κ�״̬Ϊtrue��ʱ��Ҫ��������ѡ���ƶ�����ԭ�ؾ��䡣     
      */	
	void decision(double time_diff); 
	void onUpdate(double time_diff);
	void initialize();
private slots:
    void __onFire(dt::PhysicsBodyComponent* pbc); 
    void __onTrigger(dt::TriggerAreaComponent* tac, dt::Component* c);
private:
    bool mFollow;                                            //<! ��Ϊtrue����������ߵ�������
	Alien* mBody;                                            //<! ���������壬Ϊagent�ĸ��ڵ㣬agent�䵱�ſ��������˵Ĵ��ԡ�
	bool mOnWay;                                             //<! �Ƿ���·�ϡ����������ƶ����ߣ���û�е���Ŀ�ĵ�֮ǰ�����״̬��Ϊtrue��
	bool mThreat;                                            //<! ��thickComponent���������ܵ���в��ʱ�򣬽���guard״̬
	double mThreatTime;                                      //<! ���ܵ���в��ʼÿ��update����вϵ��������ȴ��
    bool mHasEnemy;                                          //<! ����״̬�µ�ǰ�����Ƿ��е��ˡ�
	uint16_t mCurArea;                                       //<! ��ǰ��������ID��
	uint16_t mNxtArea;                                       //<! ������·�����״̬�������Ŀ�������ID��   

    const static QString TRIGGER_AREA_COMPONENT;             //!< TRIGGER_AREA_COMPONENT�����֡�
    const static QString INTERACTOR_COMPONENT;               //!< ���ǰ���Ƿ��й��������InteractorComponent�����֡�
    const static double THREAT_COOL_TIME;                    //!< ��в���ʱ�䡣
    const static double eps;                                 //!< ���ȿ��ơ�
    const static double MOVE_ROTATE_SPEED;                   //!< ������ת�ٶ�
    const static double GUARD_ROTATE_SPEED;                  //!< ������ת�ٶ�
    const static double PI;                                  //!< PI
    const static double ROTATE_FLOAT;                        //!< ֻҪ����ת����֮���������ǰ�ߡ�
    const static double ENTER_SCOPE;                         //!< ����ǰλ����Ŀ��λ�õľ���С�ڸ�ֵ���������Ϊ�Ѿ��߽��˸�����

};
#endif