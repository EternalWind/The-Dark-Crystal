#include "Animation.h"

#include <quuid.h>

OgreUtils::DirectShowManager * Animation::mDShow = nullptr;

Animation::Animation(const QString &animation_handle)
				: mAnimationHandle(animation_handle), mDshowPtr(nullptr), mHasSent(false) {
}

Animation::~Animation() {
}

void Animation::onInitialize() {
	//Node::onInitialize();
	auto camnode = this->addChildNode(new dt::Node("camnode"));
	auto cc = camnode->addComponent(new dt::CameraComponent("cam"));
	cc->lookAt(Ogre::Vector3(0, 0, 0));
	Ogre::Viewport *vp = cc->getCamera()->getViewport();

    if (mDShow == nullptr) {
        mDShow = new OgreUtils::DirectShowManager(vp);
    }
	Ogre::Root::getSingletonPtr()->addFrameListener(mDShow);

	QFileInfo info = dt::ResourceManager::get()->findFile(mAnimationHandle);
	QString tmp = info.absoluteFilePath();
	
/*	Ogre::Root::getSingletonPtr()->addFrameListener(new MyFrameListener());
*/

    mDshowPtr = OgreUtils::DirectShowManager::getSingleton().createDirectshowControl(dt::Utils::toStdString((QUuid::createUuid().toString() + "videotest")), dt::Utils::toStdString(tmp),640,480);
}

void Animation::onDeinitialize() {
	//Node::onDeinitialize();
	//mDshowPtr->Destroy();
	
	Ogre::Root::getSingleton().removeFrameListener(mDShow);
	/*if (mDshowPtr != nullptr) {
		delete mDshowPtr;
		mDshowPtr = nullptr;
	}*/
	
    mDShow->DestroyAll();
    delete mDShow;
    mDShow = nullptr;
}

void Animation::onUpdate(double time_diff) {
    if (mDShow->HasStopped() && !mHasSent) {
        emit sAnimationStopped();
        mHasSent = true;
    }

    dt::Node::onUpdate(time_diff);
}

void Animation::setAnimationHandle(QString animation_handle) {
	mAnimationHandle = animation_handle;
}

QString Animation::getAnimationHandle() const {
	return mAnimationHandle;
}

void Animation::play() {
	mDshowPtr->PlayMovie();
}

void Animation::stop() {
	mDshowPtr->StopMovie();
}

