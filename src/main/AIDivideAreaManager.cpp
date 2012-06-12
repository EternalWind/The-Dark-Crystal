#include "AIDivideAreaManager.h"
#include "Utils/Utils.hpp"
#include <fstream>

using namespace std; 

void AIDivideAreaManager::loadMapInfo(QString fileName) {
	ifstream fin(dt::Utils::toStdString(fileName));
	// read //
	fin >> mAreaNum; 	
	for (uint16_t i = 0; i < mAreaNum; ++ i) 
		fin >> mPosition[i].x>> mPosition[i].y >> mPosition[i].z;

	for (uint16_t i = 0; i < (this->mAreaNum); ++ i) 
		for (uint16_t j = 0; j < mAreaNum;  ++ j)
			mEdge[i][j] = ( (i != j) ? 1e10 : 0 );

	uint16_t edge_num; 
	fin >> edge_num; 
	for (uint16_t i = 0; i < edge_num; ++ i) {
		uint16_t x, y; 		
		fin >> x >> y; 
		mEdge[x][y] = mEdge[y][x] = mPosition[x].distance(mPosition[y]); 
	}

	//floyed计算每两个点之间的最短距离

	for (uint16_t k = 0; k < edge_num; ++ k) 
		for (uint16_t i = 0; i < edge_num; ++ i) 
			for (uint16_t j = 0; j < edge_num; ++ j) 
				if (mEdge[i][k] + mEdge[k][j] < mEdge[i][j]) 
					mEdge[i][j] = mEdge[i][k] + mEdge[k][j]; 


}

void AIDivideAreaManager::initialize() {
}

void AIDivideAreaManager::deinitialize() {
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