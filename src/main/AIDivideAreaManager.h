#ifndef THE_DARK_CRYSTAL_AI_DIVIDE_AREA_MANAGER
#define THE_DARK_CRYSTAL_AI_DIVIDE_AREA_MANAGER

#include <Core/Manager.hpp>
#include <OgreVector3.h>
#include <QString>

using namespace std; 


/**
  * ���ֵ�ͼ��������ʵ��AIѰ·
  */
class AIDivideAreaManager : public dt::Manager {
	Q_OBJECT
public:	
	/**
	  * �����ͼ������Ϣ��
	  * @fileName ��Ϣ���ļ���	  
	  */
	 void loadMapInfo(string fileName);
     void addEdge(uint16_t a, uint16_t b); 
     double getRadius();
     uint16_t getAreaNum();     
     bool isSameArea(Ogre::Vector3 a, Ogre::Vector3 b); 
     void addArea(Ogre::Vector3 p, uint16_t id); 
     Ogre::Vector3 getArea(uint16_t id);
     /**
       *sceneloader֮��һ��Ҫ����afterLoadScene��ʼ��AIDivideAreaManager��
       */

     void afterLoadScene(); 
     /**
       *sceneloader֮ǰһ��Ҫ����afterLoadScene��ʼ��AIDivideAreaManager��
       */
     void beforeLoadScene(double diameter, double part_dis);
     void destroy(std::pair<uint16_t, uint16_t> cur_id);
	 void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager��������
	   */
	 static AIDivideAreaManager* get(); 
	 void addArea(uint16_t pre, Ogre::Vector3 position);	 
	 Ogre::Vector3 getPositionById(std::pair<uint16_t, uint16_t> id);
     /**
       *���ؿ��Ե���Ŀ�������һ���ڽӵ�
       *@return -1��ʾ��ǰû�нڵ���Ե���Ŀ��ڵ㡣���߷��ؽڵ��š�
       */
     uint16_t getNxtClosestId(uint16_t cur, uint16_t des);
     /**
       *�����봫��ڵ�����������š�
       */
     uint16_t getIdByPosition(Ogre::Vector3 position);     
     /**
       *�õ��������������ٸ�����
       */
     uint16_t getAreaNumBetween(uint16_t a, uint16_t b); 

     std::pair<uint16_t, uint16_t> randomPosition(uint16_t area); 
     
private:
	Ogre::Vector3 mPosition[1000];                   //<!ÿ��area�����ĵ�
	uint16_t mAreaNum;                               //<!��������   	
    uint16_t mArea[1000][1000];                      //<!�������������������ڶ��ٸ�����
    Ogre::Vector3 mAreaPosition[1000][50];
    bool mPositionMark[1000][50];                              
    vector<uint16_t> mNxtArea[1000];                 //<!���ڵ�����   
    double mRadius;                                  //<!һ������İ뾶���ȡ�
    double mPartDis;                                 //<!����֮��Ӧ�ñ��־����Է���ײ����ڼ��С� 
    //���������ѹ��죬���ƹ��춼���˽�С�
	AIDivideAreaManager(){}
    AIDivideAreaManager & operator = (const AIDivideAreaManager &){}
    AIDivideAreaManager(const AIDivideAreaManager &){}

    
};

#endif