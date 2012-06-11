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

	enum PropType {         //道具类型
		VEHICLE,        //载具
		WEAPON,         //武器
		AMMO,           //弹药
		CRYSTAL,        //水晶
		FIRSTAIDKIT     //补给
	};

	Prop();

	Prop(const QString &name, PropType prop_type);

	~Prop();

	QString getName();

	void setName(QString name);

	/**
	  * 返回道具类型
	  * @param
	  * @returns 枚举类型ProType
	  */
	PropType getPropType(); 

private:
	QString mName;         //!< 道具名称 
	PropType mPropType;     //!< 道具类型
};

#endif