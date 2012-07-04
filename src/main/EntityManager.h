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
#include "Agent.h"
#include <set>
#include <string>
#include <vector>


using namespace std; 

class EntityManager : public dt::Manager {

    Q_OBJECT
public:
     void initialize() {}
     void afterLoadScene(dt::Scene * scene, QString stage);
     void deinitialize(){}
	 /**
	   *AIDivideAreaManager单例化。
	   */
	 static EntityManager* get();
     /**
       *判断前方扇形区域是否有威胁。
       */
     bool isForwardThreaten(Agent * agent);     
     vector<Character*> searchThreatEntity(Character * entity);
     vector<Character*> searchEntityByRange(Character * entity, double range);
     void afterLoad(dt::Scene * scene);
     void setHuman(Alien * human);
     void addPlayer(Alien * playerAI);
     void addMonster(Monster * monster);
     Alien* getHuman(); 
     void addEntityInScene(Character * monster, Agent * agent, double x, double y, double z, double scale);
     void fixTurn(double & d_degree);
     void fixDegree(double & degree);
     double clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre); 
     double avoidCollic(Character* entity, double range);
     const static double PI; 
     const static double THREAT_RANGE;
     const static double THREAT_HALF_DEGREE;
private:
    uint16_t mg[2][6];
    int16_t monsterNum[2];
    //0:血量；1：速度；2：攻击力；3：攻击间隔。4:怪物大小。
    double monsterValue[2][5];
    uint16_t mCurStage;
    Alien * mHuman; 
    vector<Character*> mAlien;
    vector<Character*> mMonster;
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