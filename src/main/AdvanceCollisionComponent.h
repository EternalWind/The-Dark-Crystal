
#ifndef THE_DARK_CRYSTAL_AGENT_ADVANCECOLLISIONCOMPONENT
#define THE_DARK_CRYSTAL_AGENT_ADVANCECOLLISIONCOMPONENT

#include <Config.hpp>
#include <btBulletCollisionCommon.h>
#include <Scene/Component.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/InteractionComponent.hpp>
#include <OgreParticleAffector.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include "ParticlesEffect.h"
#include <QString>
#include "stateManager.h"
#include "AdvanceTriggerAreaComponent.h"
/**
  * A component using collision detection by shooting bullets for interacting with other objects in the scene.
  */
class  AdvanceCollisionComponent : public dt::InteractionComponent {
    Q_OBJECT
public:
    
    /**
      * Advanced constructor.
      * @param bullet_handle The handle o the bullet's mesh.
      * @param name The name of the Component.
      * @see Component
      */
    AdvanceCollisionComponent(const QString bullet_handle, const ParticleInfo &fire_back, const ParticleInfo &bomb, bool is_throwable, QString name = "");

    /**
      * Sets the handle of the bullet's mesh.
      * @param bullet_handle The handle to set.
      */
    void setBulletMeshHandle(const QString bullet_handle);

    /**
      * Gets the handle of the bullet's mesh.
      * @returns The handle of the bullet's mesh.
      */
    const QString getBulletMeshHandle();

    void onInitialize();

protected:
    /*
     * Called when Check() is called. It will emit a sCheck signal.
     * @param start The absolute starting position for the check.
     * @param end The absolute ending position for the check.
     * @see InteractionComponent
     */
    void onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end);

protected slots:
     /**
      * Called when the thrown physics body hits an object. It will emit a sHit signal.
      * @param hit The hit object.
      * @param bullet The bullet object.
      */
    void onHit(dt::PhysicsBodyComponent* hit, dt::PhysicsBodyComponent* bullet);

	/**
      * 当目标爆炸时，对区域内的所有component进行操作
      * @param AdvanceTriggerAreaComponent
      * @param 区域内的component
      */
    void onTrigger(AdvanceTriggerAreaComponent* trigger_area, dt::Component* component);

private:
    static uint32_t autoid;
    QString mBulletMeshHandle;                              //!< The handle to the bullet's mesh.
    ParticleInfo mFireBack;                                 //!< 尾焰效果
    ParticleInfo mBomb;                                     //!< 爆炸效果
    bool mIsThrowable;                                      //!< 是否是范围攻击
};



#endif
