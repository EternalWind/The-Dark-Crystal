#include "AIDivideAreaManager.h"
#include "Utils/Utils.hpp"
#include <fstream>
#include <string>
using namespace std; 

void AIDivideAreaManager::addEdge(uint16_t a, uint16_t b) {
    mNxtArea[a].push_back(b); 
    mNxtArea[b].push_back(a);     
}
void AIDivideAreaManager::addArea(Ogre::Vector3 p, uint16_t id) {
    mPosition[id] = p; 
	mAreaNum ++;
}

void AIDivideAreaManager::loadMapInfo(string fileName) {
	ifstream fin(fileName);    
	// read //
	fin >> mAreaNum; 	    
	for (uint16_t i = 0; i < mAreaNum; ++ i) 
		fin >> mPosition[i].x>> mPosition[i].y >> mPosition[i].z;

	for (uint16_t i = 0; i < mAreaNum; ++ i) 
        for (uint16_t j = 0; j < mAreaNum;  ++ j) {
			mEdge[i][j] = ( (i != j) ? 1e10 : 0 );
            mArea[i][j] = 100000;
        }

	uint16_t edge_num; 
	fin >> edge_num; 
	for (uint16_t i = 0; i < edge_num; ++ i) {
		uint16_t x, y; 		
		fin >> x >> y; 
		mEdge[x][y] = mEdge[y][x] = mPosition[x].distance(mPosition[y]); 
        mArea[x][y] = mArea[y][x] = 1; 
        mNxtArea[x].push_back(y); 
        mNxtArea[y].push_back(x); 
	}

	//floyed计算每两个点之间的最短距离,计算两点最少经过多少个点连接。

	for (uint16_t k = 0; k < mAreaNum; ++ k) 
		for (uint16_t i = 0; i < mAreaNum; ++ i) 
			for (uint16_t j = 0; j < mAreaNum; ++ j) {
				if (mEdge[i][k] + mEdge[k][j] < mEdge[i][j]) 
					mEdge[i][j] = mEdge[i][k] + mEdge[k][j]; 
                if (mArea[i][k] + mArea[k][j] < mArea[i][j])
                    mArea[i][j] = mArea[i][k] + mArea[k][j]; 
            }

        memset(mHasGunFriend, 0, sizeof(mHasGunFriend)); 

}    

void AIDivideAreaManager::initialize() {  

}
void AIDivideAreaManager::afterLoadScene() {
    for (uint16_t i = 0; i < mAreaNum; ++ i) 
        for (uint16_t j = 0; j < mAreaNum;  ++ j) {
			mEdge[i][j] = ( (i != j) ? 1e10 : 0 );
            mArea[i][j] = ( (i != j) ? 100000 : 0);
        }
    for (uint16_t i = 0; i < mAreaNum; ++ i) {
        for (vector<uint16_t>::iterator iter = mNxtArea[i].begin(); 
                iter != mNxtArea[i].end(); iter ++) {
            mArea[i][*iter] = 1; 
            mEdge[i][*iter] = mPosition[i].distance(mPosition[*iter]);
        }
    }
    
    //floyed计算每两个点之间的最短距离,计算两点最少经过多少个点连接。

	for (uint16_t k = 0; k < mAreaNum; ++ k) 
		for (uint16_t i = 0; i < mAreaNum; ++ i) 
			for (uint16_t j = 0; j < mAreaNum; ++ j) {
				if (mEdge[i][k] + mEdge[k][j] < mEdge[i][j]) 
					mEdge[i][j] = mEdge[i][k] + mEdge[k][j]; 
                if (mArea[i][k] + mArea[k][j] < mArea[i][j])
                    mArea[i][j] = mArea[i][k] + mArea[k][j]; 
            }

        memset(mHasGunFriend, 0, sizeof(mHasGunFriend)); 
}
void AIDivideAreaManager::deinitialize() {
}

void AIDivideAreaManager::markArea(uint16_t area, bool stats) {
    mHasGunFriend[area] = stats; 
}

uint16_t AIDivideAreaManager::getAreaNumBetween(uint16_t a, uint16_t b) {
    return mArea[a][b]; 
}

uint16_t AIDivideAreaManager::getNxtClosestId(uint16_t cur, uint16_t des) {
    uint16_t tmp = -1;
    double tmp_dis = 1e10; 
    for (uint16_t i = 0; i < mNxtArea[cur].size(); i ++) { 
        uint16_t v = mNxtArea[cur][i]; 
        if (mHasGunFriend[v]) continue; 
        if (mEdge[cur][v] + mEdge[v][des] < tmp_dis) {
            tmp_dis = mEdge[cur][v] + mEdge[v][des]; 
            tmp  = i; 
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

void AIDivideAreaManager::addArea(uint16_t pre, Ogre::Vector3 position) {
	mPosition[mAreaNum] = position;
	double dis = position.distance(mPosition[mAreaNum]);	
	for (uint16_t i = 0; i < mAreaNum; ++ i) 
		mEdge[i][mAreaNum] = mEdge[mAreaNum][i] = dis + mEdge[i][pre];  
	++ mAreaNum;
}

Ogre::Vector3 AIDivideAreaManager::getPositionById(uint16_t id) {
	return mPosition[id]; 
}