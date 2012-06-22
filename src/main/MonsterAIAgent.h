#ifndef THE_DARK_CRYSTAL_MONSTERAIAGENT
#define THE_DARK_CRYSTAL_MONSTERAIAGENT

#include "Agent.h"
#include "Monster.h"
#include <Logic/TriggerAreaComponent.hpp>
#include <Logic/InteractionComponent.hpp>

/**
  * Monster��AIʵ����
  */
class MonsterAIAgent : public Agent {
Q_OBJECT
public:
     /**
       * �������ͣ��ɳ����༭������Ĺ�������һ����GUARD.        
       */	    
     enum MonsterType {
        GUARD,
        PATROL
        };
     MonsterAIAgent(QString name, MonsterAIAgent::MonsterType type = GUARD, uint16_t cur_area = 0);
     bool isThreat();
     void setThreat(bool type); 
     void setBody(Monster * body);
     /** 
       *������в״̬�µ�ʱ��Ѱ��һ����в��λ�������˹�ȥ���ڹ�����Χ��ʱ��͹���
       */
     void findAndAttack(double time_diff);  
     void onInitialize();
     void onDeinitialize();
     void onUpdate(double time_diff);
     void walk(double time_diff); 
     void decision(double time_diff);
private slots:
     void onTriggerr(dt::TriggerAreaComponent* trigger_area, dt::Component* component);
     void __onFind(dt::PhysicsBodyComponent* pbc);
private:
    uint16_t mCurArea;                                       //<! ��ǰ��������ID��
    uint16_t mNxtArea;                                       //<! ������·�����״̬�������Ŀ�������ID�� 
    MonsterType mType;                                       //!< ��������
    Monster * mBody;                                         //!< ��������
    bool mThreat;                                            //!< �Ƿ��ܵ���в  ����Χһ����Χ�ڳ��ֶԷ�ʱ����thickComponent����	
    bool mOnWay;                                             //!< �Ƿ���·��
    bool mHasEnemy;                                          //<! ��ǰ�����Ƿ��е��ˡ�    
    dt::InteractionComponent * mIteractor;                   //<! ����ǰ���Ƿ��й����component��
    dt::TriggerAreaComponent * mTrigger;                     //<! �����Ƿ񴥷�trigger��component��   
    const static QString INTERACTOR_COMPONENT;               //!< ���ǰ���Ƿ��й��������InteractorComponent�����֡�
    const static QString TRIGGER_AREA_COMPONENT;             //!< TRIGGER_AREA_COMPONENT�����֡�
    const static double eps;                                 //!< ���ȿ��ơ�
    const static double MOVE_ROTATE_SPEED;                   //!< ������ת�ٶ�
    const static double GUARD_ROTATE_SPEED;                  //!< ������ת�ٶ�
    const static double PI;                                  //!< PI
    const static double ROTATE_FLOAT;                        //!< ֻҪ����ת����֮���������ǰ�ߡ�
    const static double ENTER_SCOPE;                         //!< ����ǰλ����Ŀ��λ�õľ���С�ڸ�ֵ���������Ϊ�Ѿ��߽��˸�����

};
	

#endif