#ifndef THE_DARK_CRYSTAL_ENTITY_MANAGER
#define THE_DARK_CRYSTAL_ENTITY_MANAGER

#include <Core/Manager.hpp>
#include <OgreVector3.h>
#include <QString>
#include <Scene/Scene.hpp>
#include "Alien.h"
#include "Monster.h"
#include "MonsterAIAgent.h"
#include <map>
#include <string>
#include <vector>


using namespace std; 

class EntityManager : public dt::Manager {

    Q_OBJECT
public:
    void initialize() {}
     void afterLoadScene(dt::Scene * scene);
     void deinitialize(){}
	 /**
	   *AIDivideAreaManager��������
	   */
	 static EntityManager* get();
     void afterLoad(dt::Scene * scene);
     void setHuman(Alien * human);
     void addPlayer(Alien * playerAI) {}
     void addMonster(Monster * monster) {}
     Alien* getHuman(); 
     void addMonsterInScene(Monster * monster, MonsterAIAgent * agent, double x, double y, double z);
private:

    Alien * mHuman; 
    vector<Alien*> mAI;
    dt::Scene * mCurScene;
    uint16_t mMonsterNum;
     //���������ѹ��죬���ƹ��춼���˽�С�
	EntityManager(){}
    EntityManager & operator = (const EntityManager &){}
    EntityManager(const EntityManager &){}

};

#endif