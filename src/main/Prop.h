#ifndef THE_DARK_CRYSTAL_PROP
#define THE_DARK_CRYSTAL_PROP


#include <Scene/Node.hpp>
#include <QString>
#include <cstdint>

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

	Prop(const QString &name, PropType prop_type);

	~Prop();

	QString getName() const;

	void setName(QString name);

	void setPropType(PropType prop_type);
	
	PropType getPropType() const;  

private:
	QString mName;          //!< �������� 
	PropType mPropType;     //!< ��������
};

#endif