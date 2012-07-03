#ifndef THE_DARK_CRYSTAL_RAY_CAST_NOT_ME_COMPONENT
#define THE_DARK_CRYSTAL_RAY_CAST_NOT_ME_COMPONENT

#include <Logic/InteractionComponent.hpp>

class RaycastNotMeComponent : public dt::InteractionComponent {

    Q_OBJECT

public:
    /**
      * 构造函数。
      * @param me 要忽略掉的btCollisionObject
      * @param name 该Component的名字
      */
    RaycastNotMeComponent(btCollisionObject* me, const QString name = "");

    void onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end);
private:
    btCollisionObject* mMe;
};

#endif