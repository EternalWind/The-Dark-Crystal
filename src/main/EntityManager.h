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
#include "SpaceShip.h"

using namespace std; 

struct MonsterInfo {
    QString mMeshHandle;
    uint16_t mInitNum;
    uint16_t mMaxHealth;
    uint16_t mOrigSpeed;
    float mMass;
    float mAttackValue;
    float mAttackRange;
    float mAttackInterval;
    float mScale;
};


class EntityManager : public dt::Manager {

    Q_OBJECT
public:
     void initialize() {}
     void beforeLoadScene();
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
     /**
       *返回前方威胁物。
       */
     vector<Character*> searchThreatEntity(Character * entity);
     /**
       *返回range范围内任意一个威胁物。
       */
     Character* searchEntityByRange(Character * entity, double range);     
       
     void setHuman(Alien * human);
     void addPlayer(Alien * playerAI);
     void addMonster(Monster * monster);
     Alien* getHuman(); 
     void addEntityInScene(Character * monster, Agent * agent, double x, double y, double z, double scale);
     void fixTurn(double & d_degree);
     void fixDegree(double & degree);
     double clacDegree(Ogre::Vector3 nxt, Ogre::Vector3 pre); 
     double avoidCollic(Character* entity, double range);
     void addShip(Spaceship * ship); 
     void setHumanShip(Spaceship * ship);
     Spaceship * getHumanShip(); 

     void loadMonster(QString monster_name);

     const static double PI; 
     const static double THREAT_RANGE;
     const static double THREAT_HALF_DEGREE;
private:

    Spaceship * mHumanShip; 
    vector<Spaceship *> mAIShip;


    uint16_t mg[3][6];
    int32_t monsterNum[3];
  
    uint16_t mCurStage;
    Alien * mHuman; 
    vector<Character*> mAlien;
    vector<Character*> mMonster;
    dt::Scene * mCurScene;
    int16_t mMonsterNum;

    MonsterInfo mMonsterInfo;
     //单例化，把构造，复制构造都设成私有。
	EntityManager(){}
    EntityManager & operator = (const EntityManager &){}
    EntityManager(const EntityManager &){}

    double _dis(Ogre::Vector3 a, Ogre::Vector3 b);    
    
    void __loadStage(QString stage); 

public slots:
    void __isMonsterDead(Character * monster);
    void __isAlienDead(Character * alien);

};

#endif