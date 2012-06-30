#include "AIDivideAreaManager.h"
#include "Utils/Utils.hpp"
#include <fstream>
#include <string>
using namespace std; 

Ogre::Vector3 AIDivideAreaManager::getArea(uint16_t id) {
    return mPosition[id];
}
double AIDivideAreaManager::getRadius() {
    return mRadius;
}
void AIDivideAreaManager::addEdge(uint16_t a, uint16_t b) {
    mNxtArea[a].push_back(b); 
    mNxtArea[b].push_back(a);     
}
void AIDivideAreaManager::addArea(Ogre::Vector3 p, uint16_t id) {

    mPosition[id] = p;
    mAreaNum ++; 

}
uint16_t AIDivideAreaManager::getAreaNum() {
    return mAreaNum; 
}
bool AIDivideAreaManager::isSameArea(Ogre::Vector3 a, Ogre::Vector3 b) {
    int a_id = getIdByPosition(a); 
    int b_id = getIdByPosition(b); 
    return a_id == b_id;
}
void AIDivideAreaManager::loadMapInfo(string fileName) {
	ifstream fin(fileName);    
	// read //
	fin >> mAreaNum; 	    
	for (uint16_t i = 0; i < mAreaNum; ++ i) 
		fin >> mPosition[i].x>> mPosition[i].y >> mPosition[i].z;

	for (uint16_t i = 0; i < mAreaNum; ++ i) 
        for (uint16_t j = 0; j < mAreaNum;  ++ j) {			
            mArea[i][j] = 100000;
        }

	uint16_t edge_num; 
	fin >> edge_num; 
	for (uint16_t i = 0; i < edge_num; ++ i) {
		uint16_t x, y; 		
		fin >> x >> y; 		
        mArea[x][y] = mArea[y][x] = 1; 
        mNxtArea[x].push_back(y); 
        mNxtArea[y].push_back(x); 
	}

	//floyed计算每两个点之间的最短距离,计算两点最少经过多少个点连接。

	for (uint16_t k = 0; k < mAreaNum; ++ k) 
		for (uint16_t i = 0; i < mAreaNum; ++ i) 
			for (uint16_t j = 0; j < mAreaNum; ++ j) {				
                if (mArea[i][k] + mArea[k][j] < mArea[i][j])
                    mArea[i][j] = mArea[i][k] + mArea[k][j]; 
            }
        

}    

void AIDivideAreaManager::initialize() {  

}
void AIDivideAreaManager::afterLoadScene() {
    for (uint16_t i = 0; i < mAreaNum; ++ i) 
        for (uint16_t j = 0; j < mAreaNum;  ++ j) {			
            mArea[i][j] = ( (i != j) ? 100000 : 0);
        }
    for (uint16_t i = 0; i < mAreaNum; ++ i) {
        for (vector<uint16_t>::iterator iter = mNxtArea[i].begin(); 
                iter != mNxtArea[i].end(); iter ++) {
            mArea[i][*iter] = 1;             
        }
    }
    
    //floyed计算每两个点之间的最短距离,计算两点最少经过多少个点连接。

	for (uint16_t k = 0; k < mAreaNum; ++ k) 
		for (uint16_t i = 0; i < mAreaNum; ++ i) 
			for (uint16_t j = 0; j < mAreaNum; ++ j) {				
                if (mArea[i][k] + mArea[k][j] < mArea[i][j])
                    mArea[i][j] = mArea[i][k] + mArea[k][j]; 
            }
}
       
void AIDivideAreaManager::beforeLoadScene(double diameter, double part_dis) {
    mRadius = diameter / 2.0;
    mPartDis = part_dis; 
    mAreaNum = 0; 
    for (int i = 0; i < 1000; i ++) 
        mNxtArea[i].clear();
    memset(mPositionMark, 0, sizeof(mPositionMark));
}
void AIDivideAreaManager::deinitialize() {
}



uint16_t AIDivideAreaManager::getAreaNumBetween(uint16_t a, uint16_t b) {
    return mArea[a][b]; 
}

uint16_t AIDivideAreaManager::getNxtClosestId(uint16_t cur, uint16_t des) {
    uint16_t tmp = des;
    uint16_t tmp_dis = 10000; 
    if (cur == des) return tmp; 
    for (uint16_t i = 0; i < mNxtArea[cur].size(); i ++) { 
        uint16_t v = mNxtArea[cur][i];        
        if (mArea[cur][v] + mArea[v][des] < tmp_dis) {
            tmp_dis = mArea[cur][v] + mArea[v][des]; 
            tmp  = v; 
        }
    }
    return tmp; 
}
uint16_t AIDivideAreaManager::getIdByPosition(Ogre::Vector3 position) {
    double tmp_dis = 1e10;
    int res  = 0; 
    for (uint16_t i = 0; i < mAreaNum; ++ i) {
        double cur_dis = position.distance(mPosition[i]); 
        if (cur_dis < tmp_dis) {
            tmp_dis = cur_dis; 
            res = i; 
        }
    }
    return res; 
}

AIDivideAreaManager* AIDivideAreaManager::get() {
	static AIDivideAreaManager * singleton= new AIDivideAreaManager(); 
	return singleton;
}

Ogre::Vector3 AIDivideAreaManager::getPositionById(std::pair<uint16_t, uint16_t> id) {
    return mAreaPosition[id.first][id.second]; 
}

std::pair<uint16_t, uint16_t> AIDivideAreaManager::randomPosition(uint16_t area) {
    double tmp = mRadius * 2 / 100.0;
    Ogre::Vector3 p;
    p.y = 0; 
    while (1) {
        p.x = (rand() % 100 - 50) * tmp;
        p.z = (rand() % 100 - 50)* tmp;
        bool flag = 0; 
        for (int i = 0; i < 50; i ++) 
            if (mPositionMark[area][i]) {
                if (mAreaPosition[area][i].distance(p) < mPartDis) {
                    flag = 1; 
                    break; 
                }
            }
        if (flag) continue;        
        if (p.x*p.x + p.z*p.z <= mRadius * mRadius) break; 
    }   

  
    for (int i = 0; i < 50; i ++) 
        if (!mPositionMark[area][i]) {
            
            p.x += mPosition[area].x; 
            p.z += mPosition[area].z; 
            mPositionMark[area][i] = 1; 
            mAreaPosition[area][i] = p;

            
            return make_pair(area, i);
        }    
    //如果这个区域人太多，则不选择这个区域。
    return make_pair(-1,-1);
}

void AIDivideAreaManager::destroy(std::pair<uint16_t, uint16_t> cur_id) {
    mPositionMark[cur_id.first][cur_id.second] = 0; 
}