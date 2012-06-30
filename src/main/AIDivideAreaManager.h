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
     void addEdge(uint16_t a, uint16_t b); 
     double getRadius();
     uint16_t getAreaNum();     
     bool isSameArea(Ogre::Vector3 a, Ogre::Vector3 b); 
     void addArea(Ogre::Vector3 p, uint16_t id); 
     Ogre::Vector3 getArea(uint16_t id);
     /**
       *sceneloader之后一定要调用afterLoadScene初始化AIDivideAreaManager。
       */

     void afterLoadScene(); 
     /**
       *sceneloader之前一定要调用afterLoadScene初始化AIDivideAreaManager。
       */
     void beforeLoadScene(double diameter, double part_dis);
     void destroy(std::pair<uint16_t, uint16_t> cur_id);
	 void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager单例化。
	   */
	 static AIDivideAreaManager* get(); 
	 void addArea(uint16_t pre, Ogre::Vector3 position);	 
	 Ogre::Vector3 getPositionById(std::pair<uint16_t, uint16_t> id);
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
       *得到两个区域相差多少个区域。
       */
     uint16_t getAreaNumBetween(uint16_t a, uint16_t b); 

     std::pair<uint16_t, uint16_t> randomPosition(uint16_t area); 
     
private:
	Ogre::Vector3 mPosition[1000];                   //<!每个area的中心点
	uint16_t mAreaNum;                               //<!区域数量   	
    uint16_t mArea[1000][1000];                      //<!计算两个区域最少相邻多少个区域。
    Ogre::Vector3 mAreaPosition[1000][50];
    bool mPositionMark[1000][50];                              
    vector<uint16_t> mNxtArea[1000];                 //<!相邻的区域。   
    double mRadius;                                  //<!一个区域的半径长度。
    double mPartDis;                                 //<!队友之间应该保持距离以防相撞或过于集中。 
    //单例化，把构造，复制构造都设成私有。
	AIDivideAreaManager(){}
    AIDivideAreaManager & operator = (const AIDivideAreaManager &){}
    AIDivideAreaManager(const AIDivideAreaManager &){}

    
};

#endif