#ifndef THE_DARK_CRYSTAL_AI_DIVIDE_AREA_MANAGER
#define THE_DARK_CRYSTAL_AI_DIVIDE_AREA_MANAGER

#include <Core/Manager.hpp>
#include <OgreVector3.h>
#include <QString>

using namespace std; 


/**
  * 划分地图的区域以实现AI寻路
  */
class AIDivideAreaManager : public dt::Manager {
	Q_OBJECT
public:	
	/**
	  * 载入地图划分信息。
	  * @fileName 信息的文件名	  
	  */
	 void loadMapInfo(string fileName);
	 void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager单例化。
	   */
	 static AIDivideAreaManager* get(); 
	 void addArea(uint16_t pre, Ogre::Vector3 position);	 
	 Ogre::Vector3 getPositionById(uint16_t id);
     /**
       *返回可以到达目标最近的一个邻接点
       *@return -1表示当前没有节点可以到达目标节点。或者返回节点标号。
       */
     uint16_t getNxtClosestId(uint16_t cur, uint16_t des);
     /**
       *返回离传入节点最近的区域编号。
       */
     uint16_t getIdByPosition(Ogre::Vector3 position);
     /**
       *标记area区域的状态。
       */
     void markArea(uint16_t area, bool stats); 
     /**
       *得到两个区域相差多少个区域。
       */
     uint16_t getAreaNumBetween(uint16_t a, uint16_t b); 
     
private:
	Ogre::Vector3 mPosition[1000];                   //<!每个area的中心点
	uint16_t mAreaNum;                               //<!区域数量   
	double mEdge[1000][1000];                        //<!区域之间的最短距离 一个地图最多只有1000个区域	
    uint16_t mArea[1000][1000];                      //<!计算两个区域最少相邻多少个区域。
    vector<uint16_t> mNxtArea[1000];                 //<!相邻的节点。
    bool mHasGunFriend[1000];                        //<!标记该区域是否被队友占领，控制一个区域最多一个人，以免过于集中。 
    
    //单例化，把构造，复制构造都设成私有。
	AIDivideAreaManager(){}
    AIDivideAreaManager & operator = (const AIDivideAreaManager &){}
    AIDivideAreaManager(const AIDivideAreaManager &){}
};

#endif