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

	Prop();

	Prop(const QString &name);

	~Prop();

	enum ProType {          //��������
		VEHICLE,        //�ؾ�
		WEAPON,         //����
		AMMO,           //��ҩ
		CRYSTAL         //ˮ��
	};

	QString getName();

	void setName(QString name);

	/**
	  * ���ص�������
	  * @param
	  * @returns ö������ProType
	  */
	ProType getPropType(); 

private:
	QString mName;         //!< �������� 
	ProType mPropType;     //!< ��������
};

#endif