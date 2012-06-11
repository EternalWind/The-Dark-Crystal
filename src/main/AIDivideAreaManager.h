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
	 void loadMapInfo(QString fileName);
	 void initialize();
     void deinitialize();
	 /**
	   *AIDivideAreaManager��������
	   */
	 AIDivideAreaManager* get(); 
	 void addArea(uint16_t pre, Ogre::Vector3 position);	 
	 Ogre::Vector3 getPositionById(uint16_t id);
private:
	Ogre::Vector3 mPosition[1000];                         //<!ÿ��area�����ĵ�
	uint16_t mAreaNum;                               //<!��������   
	double mEdge[1000][1000];                        //<!����֮�����̾��� һ����ͼ���ֻ��1000������	
    
	AIDivideAreaManager(){}
    AIDivideAreaManager & operator = (const AIDivideAreaManager &){}
    AIDivideAreaManager(const AIDivideAreaManager &){}
};

#endif