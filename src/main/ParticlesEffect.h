#ifndef THE_DARK_CRYSTAL_PARTICLES_EFFECT
#define THE_DARK_CRYSTAL_PARTICLES_EFFECT


#include <Scene/Node.hpp>
#include <QString>
#include <cstdint>

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
	ParticlesEffect(const QString node_name, const QString &material_handle, const QString &sound_handle);

	~ParticlesEffect();

	void onInitialize();

	void onDeinitialize();

	void onUpdate(double time_diff);

private:
	QString mMaterialHandle;                        //!< �����ļ���
	QString mSoundHandle;                           //!< ��ը���ļ���
	double mRuntime;                                //!< ��ըЧ������ʱ��
	
};

#endif