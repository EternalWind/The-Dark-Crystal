#ifndef THE_DARK_CRYSTAL_PARTICLES_EFFECT
#define THE_DARK_CRYSTAL_PARTICLES_EFFECT


#include <Scene/Node.hpp>
#include <QString>
#include <cstdint>
#include <OgreParticleAffector.h>

struct ParticleInfo {
	float TimeToLiveL;
	float TimeToLiveR;
	float time0;
	Ogre::Vector3 colour0;
	float time1;
	Ogre::Vector3 colour1;
	float time2;
	Ogre::Vector3 colour2;
	float degree;
	QString MaterialHandle;
};

/**
  * ����Ч���࣬���ڱ�ըЧ��
  */
class ParticlesEffect : public dt::Node {

	Q_OBJECT

public:

	/**
      * ���캯���� 
      * @param node_name Node  ���������
      * @param material_handle ���ʾ��
      */
	ParticlesEffect(const QString node_name, const QString &material_handle, const QString &sound_handle, const ParticleInfo &part_parm);

	~ParticlesEffect();

	void onInitialize();

	void onDeinitialize();

	void onUpdate(double time_diff);

private:
	QString mMaterialHandle;                        //!< �����ļ���
	QString mSoundHandle;                           //!< ��ը���ļ���
	double mRuntime;                                //!< ��ըЧ������ʱ��
	ParticleInfo mPartParm;                         //!< ���Ӳ���
};

#endif