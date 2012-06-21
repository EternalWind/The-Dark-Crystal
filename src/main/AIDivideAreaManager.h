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
	 void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager��������
	   */
	 static AIDivideAreaManager* get(); 
	 void addArea(uint16_t pre, Ogre::Vector3 position);	 
	 Ogre::Vector3 getPositionById(uint16_t id);
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
       *���area�����״̬��
       */
     void markArea(uint16_t area, bool stats); 
     /**
       *�õ��������������ٸ�����
       */
     uint16_t getAreaNumBetween(uint16_t a, uint16_t b); 
     
private:
	Ogre::Vector3 mPosition[1000];                   //<!ÿ��area�����ĵ�
	uint16_t mAreaNum;                               //<!��������   
	double mEdge[1000][1000];                        //<!����֮�����̾��� һ����ͼ���ֻ��1000������	
    uint16_t mArea[1000][1000];                      //<!�������������������ڶ��ٸ�����
    vector<uint16_t> mNxtArea[1000];                 //<!���ڵĽڵ㡣
    bool mHasGunFriend[1000];                        //<!��Ǹ������Ƿ񱻶���ռ�죬����һ���������һ���ˣ�������ڼ��С� 
    
    //���������ѹ��죬���ƹ��춼���˽�С�
	AIDivideAreaManager(){}
    AIDivideAreaManager & operator = (const AIDivideAreaManager &){}
    AIDivideAreaManager(const AIDivideAreaManager &){}
};

#endif