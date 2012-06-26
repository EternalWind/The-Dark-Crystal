#ifndef THE_DARK_CRYSTAL_ENTITY_MANAGER
#define THE_DARK_CRYSTAL_ENTITY_MANAGER

#include <Core/Manager.hpp>
#include <OgreVector3.h>
#include <QString>
#include <Scene/Scene.hpp>
#include "Alien.h"
#include "Monster.h"
#include <map>
#include <string>
#include <vector>

using namespace std; 

class EntityManager : public dt::Manager {

    Q_OBJECT
public:
     void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager单例化。
	   */
	 static EntityManager* get();
     void afterLoad(dt::Scene * scene);
     void setHuman(Alien * human);
     void addPlayer(Alien * playerAI) {}
     void addMonster(Monster * monster) {}
     Alien* getHuman(); 
     
private:

    Alien * mHuman; 
    vector<Alien*> mAI;
     //单例化，把构造，复制构造都设成私有。
	EntityManager(){}
    EntityManager & operator = (const EntityManager &){}
    EntityManager(const EntityManager &){}

};

#endif