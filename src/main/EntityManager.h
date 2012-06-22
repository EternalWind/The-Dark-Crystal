#ifndef THE_DARK_CRYSTAL_ENTITY_MANAGER
#define THE_DARK_CRYSTAL_ENTITY_MANAGER

#include <Core/Manager.hpp>
#include <OgreVector3.h>
#include <QString>

class EntityManager : public dt::Manager {

    Q_OBJECT
public:
     void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager单例化。
	   */
	 static EntityManager* get();
private:
     //单例化，把构造，复制构造都设成私有。
	EntityManager(){}
    EntityManager & operator = (const EntityManager &){}
    EntityManager(const EntityManager &){}

};

#endif