#ifndef THE_DARK_CRYSTAL_ATTACK_DETECT_COMPONENT
#define THE_DARK_CRYSTAL_ATTACK_DETECT_COMPONENT

#include <Logic/InteractionComponent.hpp>

class AttackDetectComponent : public dt::InteractionComponent {
	Q_OBJECT

public:
	AttackDetectComponent(const QString &name);

protected:
    /*
     * Called when Check() is called. It will emit a sCheck signal.
     * @param start The absolute starting position for the check.
     * @param end The absolute ending position for the check.
     * @see InteractionComponent
     */
    void onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end);


};

#endif