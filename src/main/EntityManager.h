#ifndef THE_DARK_CRYSTAL_ENTITY_MANAGER
#define THE_DARK_CRYSTAL_ENTITY_MANAGER

#include <Core/Manager.hpp>
#include <OgreVector3.h>
#include <QString>
#include <Scene/Scene.hpp>
#include "Alien.h"
#include "Monster.h"
#include "MonsterAIAgent.h"
#include "Character.h"

#include <set>
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
	   *AIDivideAreaManager单例化。
	   */
	 static EntityManager* get();
     vector<Character*> searchEntityByRange(Character * entity, double range);
     void afterLoad(dt::Scene * scene);
     void setHuman(Alien * human);
     void addPlayer(Alien * playerAI);
     void addMonster(Monster * monster);
     Alien* getHuman(); 
     void addEntityInScene(Character * monster, Agent * agent, double x, double y, double z, double scale);
private:

    Alien * mHuman; 
    set<Character*> mAlien;
    set<Character*> mMonster;
    dt::Scene * mCurScene;
    uint16_t mMonsterNum;
     //单例化，把构造，复制构造都设成私有。
	EntityManager(){}
    EntityManager & operator = (const EntityManager &){}
    EntityManager(const EntityManager &){}
    double _dis(Ogre::Vector3 a, Ogre::Vector3 b);
public slots:
    void __isMonsterDead(Character * monster);
    void __isAlienDead(Character * alien);

};

#endif