#ifndef THE_DARK_CRYSTAL_ANIMATION
#define THE_DARK_CRYSTAL_ANIMATION

#include <QString>
#include <Scene/Node.hpp>
#include <qdir.h>
#include <qfile.h>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/StateManager.hpp>
#include "UtilsOgreDshow.h"
#include <OgreRoot.h>

/**
  * 动画类，用于播放过场动画
  */
class Animation : public dt::Node {
	Q_OBJECT
public:
	Animation(const QString &animation_handle);

	~Animation();
	
	/**
	  * 初始化，并会开始播放动画
	  */
	void onInitialize();

	void onDeinitialize();

	void setAnimationHandle(QString animation_handle);

	QString getAnimationHandle() const;

	/**
	  * 从头开始播放动画
	  */
	void play();

	/**
	  * 停止播放动画
	  */
	void stop();
	
private:
	QString mAnimationHandle;   //!< 动画文件名
	OgreUtils::DirectShowControl *mDshowPtr;   //!< 动画播放控制指针
};
#endif
