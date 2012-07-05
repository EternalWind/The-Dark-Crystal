#include "Crystal.h"

#include "BattleState.h"

Crystal::Crystal() {
}
	
Crystal::Crystal(const QString &prop_name, const QString &node_name, double unlock_time) 
			: Prop(prop_name, node_name, CRYSTAL),
              mUnlockTime(unlock_time),
              mUnlockProgress(0.0),
              mIsUnlocking(false) {}
	
Crystal::~Crystal() {
}

void Crystal::onInitialize() {
    Prop::onInitialize();

    this->removeComponent("physics_body");
    auto physics_body = this->addComponent(new dt::PhysicsBodyComponent("prop_mesh", "physics_body", dt::PhysicsBodyComponent::CONVEX, 0.0));

    dt::State* state = this->getState();

    connect(this, SIGNAL(sUnlockCrystalProgressChanged(uint16_t)),
        state, SLOT(__onUnlockCrystalProgressChanged(uint16_t)));
}

void Crystal::onDeinitialize() {
    dt::State* state = this->getState();

    disconnect(this, SIGNAL(sUnlockCrystalProgressChanged(uint16_t)), 
        state, SLOT(__onUnlockCrystalProgressChanged(uint16_t)));

    Prop::onDeinitialize();
}

double Crystal::getUnlockTime() const {
	return mUnlockTime;
}

void Crystal::setUnlockTime(double unlock_time) {	
	mUnlockTime = unlock_time;
}

double Crystal::getUnlockProgress() const {
    return mUnlockProgress;
}

void Crystal::setUnlockProgress(const double& unlock_progress) {
    mUnlockProgress = unlock_progress;
    if (mUnlockProgress < 0.0) {
        mUnlockProgress = 0.0;
    }

    emit sUnlockCrystalProgressChanged(getUnlockProgressPercentage());
}

uint16_t Crystal::getUnlockProgressPercentage() const {
    uint16_t ret = (uint16_t)(100 * (mUnlockProgress / mUnlockTime + 0.001f));

    if (ret > 100) {
        ret = 100;
    }
    return ret;
}

bool Crystal::hasUnlocked() const {
    return getUnlockProgressPercentage() >= 100;
}

void Crystal::beginUnlock() {
    mIsUnlocking = true;
}

void Crystal::onUpdate(double time_diff) {
    mIsUpdatingAfterChange = (time_diff == 0);

    if (mIsUnlocking) {
        mIsUnlocking = false;
        setUnlockProgress(mUnlockProgress + time_diff);      
    } else {
        // 解锁到一半遇到什么问题了？突然中断了？大丈夫？
        setUnlockProgress(mUnlockProgress - 2 * time_diff);
    }

    Node::onUpdate(time_diff);
}


bool Crystal::isUnlocking() {
    return mIsUnlocking;
}