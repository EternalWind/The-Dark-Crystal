#ifndef THE_DARK_CRYSTAL_PROP
#define THE_DARK_CRYSTAL_PROP


#include <Scene/Node.hpp>
#include <QString>
#include <cstdint>

/**
  * 道具类，武器、弹药、补给和水晶的基类
  */
class Prop : public dt::Node {
	Q_OBJECT
public:

	Prop();

	Prop(const QString &name);

	~Prop();

	enum ProType {          //道具类型
		VEHICLE,        //载具
		WEAPON,         //武器
		AMMO,           //弹药
		CRYSTAL         //水晶
	};

	QString getName();

	void setName(QString name);

	/**
	  * 返回道具类型
	  * @param
	  * @returns 枚举类型ProType
	  */
	ProType getPropType(); 

private:
	QString mName;         //!< 道具名称 
	ProType mPropType;     //!< 道具类型
};

#endif