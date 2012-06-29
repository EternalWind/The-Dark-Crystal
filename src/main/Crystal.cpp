#include "Crystal.h"

Crystal::Crystal() {
}
	
Crystal::Crystal(const QString &prop_name, const QString &node_name, double unlock_time) 
			: Prop(prop_name, node_name, CRYSTAL),
              mUnlockTime(unlock_time),
              mUnlockProgress(0.0),
              mIsUnlocking(false) {}
	
Crystal::~Crystal() {
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

void Crystal::setUnlockProgress(const double unlock_progress) {
    if (mUnlockProgress != unlock_progress && mUnlockProgress < mUnlockTime) {
        double pre_progress = mUnlockProgress / mUnlockTime;

        if (unlock_progress <= mUnlockTime) {
            mUnlockProgress = unlock_progress;
        } else {
            mUnlockProgress = mUnlockTime;
        }

        emit sUnlockProgressChanged(this, pre_progress, mUnlockProgress / mUnlockTime);
    }
}

double Crystal::getUnlockProgressPercentage() const {
    return (float)mUnlockProgress / mUnlockTime;
}

void Crystal::setUnlockProgressPercentage(const double unlock_progress) {
    setUnlockProgress(unlock_progress * mUnlockTime);
}

bool Crystal::isUnlocked() const {
    return getUnlockProgress() == getUnlockTime();
}

void Crystal::beginUnlock() {
    mIsUnlocking = true;
}

void Crystal::onUpdate(double time_diff) {
    if (mIsUnlocking) {
        mIsUnlocking = false;
        
        if (mUnlockProgress != mUnlockTime) {
            setUnlockProgress(mUnlockProgress + time_diff);
        }
    } else {
        if (mUnlockProgress != 0.0) {
            setUnlockProgress(mUnlockProgress - time_diff);
        }
    }

    Node::onUpdate(time_diff);
}