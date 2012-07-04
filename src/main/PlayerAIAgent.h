#ifndef THE_DARK_CRYSTAL_PLAYERAIAGENT
#define THE_DARK_CRYSTAL_PLAYERAIAGENT
#include "Agent.h"
#include "Alien.h"
#include <Logic/TriggerAreaComponent.hpp>
#include <QString>
#include <Logic/InteractionComponent.hpp>

class PlayerAIAgent : public Agent {
Q_OBJECT
public: 
    PlayerAIAgent(QString name); 
    bool isOnWay(); 
    void setOnWay(bool type); 	
    Alien* getBody(); 
    void fixTurn(double & d_degree);
    void fixDegree(double & degree);
    void lookAround(double d_degree, double time_diff, double speed);
    double clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre); 
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
    void onInitialize();
    void onDeinitialize();
    dt::PhysicsBodyComponent*  test();
    void __onTrigger(Character * c);
private slots:
    void __onFire(dt::PhysicsBodyComponent* pbc); 
    
private:
    bool mSpeedUp; 
    double mPreDegree;                                       //<! �洢��һ֡����ĳ���
    double mExpectDegree;                                    //<! �����ĳ���
    bool mAttack;                                            //<! �Ƿ��ڹ���״̬��
    bool mFollow;                                            //<! ��Ϊtrue����������ߵ�������
    Alien* mBody;                                            //<! ���������壬Ϊagent�ĸ��ڵ㣬agent�䵱�ſ��������˵Ĵ��ԡ�
    Alien* mHumanBody;                                       //<! ���ָ��
    bool mOnWay;                                             //<! �Ƿ���·�ϡ����������ƶ����ߣ���û�е���Ŀ�ĵ�֮ǰ�����״̬��Ϊtrue��
    bool mThreat;                                            //<! ��thickComponent���������ܵ���в��ʱ�򣬽���guard״̬
    double mThreatTime;                                      //<! ���ܵ���в��ʼÿ��update����вϵ��������ȴ��
    bool mHasEnemy;                                          //<! ����״̬����ǰ���Ƿ��е��ˡ�
    std::pair<uint16_t, uint16_t> mNxtArea;                                       //<! ������·�����״̬�������Ŀ�������ID,����Ϊ��ǰ���ڵص�ID��  
    dt::InteractionComponent * mIteractor;                   //<! ����ǰ���Ƿ��й����component��
    dt::TriggerAreaComponent * mTrigger;                     //<! �����Ƿ񴥷�trigger��component��
    bool mOnMovePress;                                       
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