#include "Animation.h"

Animation::Animation(const QString &animation_handle)
				: mAnimationHandle(animation_handle), mDshowPtr(nullptr) {
}

Animation::~Animation() {
}

void Animation::onInitialize() {
	//Node::onInitialize();
	auto camnode = this->addChildNode(new dt::Node("camnode"));
	auto cc = camnode->addComponent(new dt::CameraComponent("cam"));
	cc->lookAt(Ogre::Vector3(0, 0, 0));
	Ogre::Viewport *vp = cc->getCamera()->getViewport();
	Ogre::Root::getSingletonPtr()->addFrameListener(mDShow = new OgreUtils::DirectShowManager(vp));

	QFileInfo info = dt::ResourceManager::get()->findFile(mAnimationHandle);
	QString tmp = info.absoluteFilePath();
	
/*	Ogre::Root::getSingletonPtr()->addFrameListener(new MyFrameListener());
*/
	
	mDshowPtr = OgreUtils::DirectShowManager::getSingleton().createDirectshowControl("videotest",tmp.toStdString(),640,480);
}

void Animation::onDeinitialize() {
	//Node::onDeinitialize();
	//mDshowPtr->Destroy();
	
	Ogre::Root::getSingleton().removeFrameListener(mDShow);
	this->kill();
	if (mDshowPtr != nullptr) {
		delete mDshowPtr;
		mDshowPtr = nullptr;
	}
	
  
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

