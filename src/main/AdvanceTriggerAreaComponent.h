
#ifndef THE_DARK_CRYSTAL_ADVANCETRIGGERAREACOMPONENT
#define THE_DARK_CRYSTAL_ADVANCETRIGGERAREACOMPONENT

#include <memory>

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Logic/ScriptComponent.hpp>

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


class AdvanceTriggerAreaComponent : public dt::Component {
    Q_OBJECT
public:
    /**
    * Constructor.
    * @param areaShape shape of the trigger area 
    * @param name The name of the component.
    */
    AdvanceTriggerAreaComponent(btCollisionShape* areaShape,
                         const QString name = "");

    void onInitialize();
    void onDeinitialize();
    void onEnable();
    void onDisable();
    void onUpdate(double time_diff);
    
    /**
    * Setter for the area shape
    * @param area Shape of the trigger area
    */
    void setAreaShape(btCollisionShape* areaShape);

signals:
    void triggered(AdvanceTriggerAreaComponent* trigger_area, dt::Component* component);

private:
    btCollisionShape* mArea; /// area entering which sends the Triggered signal
    btGhostObject* mObject;  /// object used to check collission

};


#endif