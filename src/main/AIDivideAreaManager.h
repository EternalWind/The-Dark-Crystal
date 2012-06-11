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
	 void loadMapInfo(QString fileName);
	 void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager单例化。
	   */
	 AIDivideAreaManager* get(); 
	 void addArea(uint16_t pre, Ogre::Vector3 position);	 
	 Ogre::Vector3 getPositionById(uint16_t id);
private:
	Ogre::Vector3 mPosition[1000];                         //<!每个area的中心点
	uint16_t mAreaNum;                               //<!区域数量   
	double mEdge[1000][1000];                        //<!区域之间的最短距离 一个地图最多只有1000个区域	
    
	AIDivideAreaManager(){}
    AIDivideAreaManager & operator = (const AIDivideAreaManager &){}
    AIDivideAreaManager(const AIDivideAreaManager &){}
};

#endif