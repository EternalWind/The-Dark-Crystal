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
	   *AIDivideAreaManager��������
	   */
	 static EntityManager* get();
private:
     //���������ѹ��죬���ƹ��춼���˽�С�
	EntityManager(){}
    EntityManager & operator = (const EntityManager &){}
    EntityManager(const EntityManager &){}

};

#endif