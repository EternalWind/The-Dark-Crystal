#ifndef THE_DARK_CRYSTAL_PROP
#define THE_DARK_CRYSTAL_PROP


#include <Scene/Node.hpp>
#include <QString>
#include <cstdint>
#include <Graphics\MeshComponent.hpp>
#include <Physics\PhysicsBodyComponent.hpp>
/**
  * �����࣬��������ҩ��������ˮ���Ļ���
  */
class Prop : public dt::Node {
	Q_OBJECT
public:

	enum PropType {         //��������
		VEHICLE,        //�ؾ�
		WEAPON,         //����
		AMMO,           //��ҩ
		CRYSTAL,        //ˮ��
		FIRST_AID_KIT   //����
	};

	Prop();

	Prop(const QString &prop_name, const QString &node_name, PropType prop_type);

	~Prop();

	void onInitialize();

	QString getPropName() const;

	void setPropName(QString name);

	void setPropType(PropType prop_type);
	
	PropType getPropType() const;  

protected:
	QString mPropName;                              //!< ��������
	PropType mPropType;                             //!< ��������
};

#endif