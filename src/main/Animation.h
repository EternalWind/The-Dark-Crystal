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
  * �����࣬���ڲ��Ź�������
  */
class Animation : public dt::Node {
	Q_OBJECT
public:
	Animation(const QString &animation_handle);

	~Animation();
	
	/**
	  * ��ʼ�������Ὺʼ���Ŷ���
	  */
	void onInitialize();

	void onDeinitialize();

	void setAnimationHandle(QString animation_handle);

	QString getAnimationHandle() const;

	/**
	  * ��ͷ��ʼ���Ŷ���
	  */
	void play();

	/**
	  * ֹͣ���Ŷ���
	  */
	void stop();
	
private:
	QString mAnimationHandle;   //!< �����ļ���
	OgreUtils::DirectShowControl *mDshowPtr;   //!< �������ſ���ָ��
};
#endif
