#ifndef THE_DARK_CRYSTAL_PROP
#define THE_DARK_CRYSTAL_PROP


#include <Scene/Node.hpp>
#include <QString>
#include <cstdint>
#include <Graphics\MeshComponent.hpp>
#include <Physics\PhysicsBodyComponent.hpp>
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
		FIRST_AID_KIT   //补给
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
	QString mPropName;                              //!< 道具名称
	PropType mPropType;                             //!< 道具类型
};

#endif