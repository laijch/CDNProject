#include "CacheNode.h"

CacheNode::CacheNode()
{
	id = 0;
	replaceFileNum = 0;
	degree = 0;
	weight = 0.0;
	probabilityDistribution = 0.0;
	positionX = 0.0;
	positionY = 0.0;
}

CacheNode::~CacheNode()
{
}

void CacheNode::setID(int id_)
{
	id = id_;
	// 当节点为源服务器时，储存全部（15个）文件
	if (id == 0) {
		fileNum = 15;
		for (int i = 0; i < 15; i++) {
			file[i] = i + 1;
		}
	}
	// 节点为缓存服务器时，只储存10个文件
	else if (id > 0) {
		fileNum = 0;
		for (int i = 0; i < 15; i++) {
			file[i] = 0;
		}
	}
}

void CacheNode::setDegree(int degree_)
{
	degree = degree_;
}

void CacheNode::setWeight(double weight_)
{
	weight = weight_;
}

void CacheNode::setProDis(double proDis_)
{
	probabilityDistribution = proDis_;
}

void CacheNode::setFileNum(int num_)
{
	int fileNum_ = fileNum + num_;
	if (fileNum_ <= 10 && fileNum_ >= 0) {
		fileNum = fileNum_;
	}
}

bool CacheNode::findFile(int file_)
{
	for (int i = 0; i < fileNum; i++) {
		if (file_ == file[i]) return true;
	}
	return false;
}

void CacheNode::LRUCache(int file_)
{
	file[replaceFileNum] = file_;
	++replaceFileNum;
	if (replaceFileNum == 10) replaceFileNum = 0;
	if (fileNum < 10) ++fileNum;
}

double CacheNode::getPosX()
{
	return positionX;
}

double CacheNode::getPosY()
{
	return positionY;
}

void CacheNode::setPos(double x_, double y_)
{
	positionX = x_;
	positionY = y_;
}
