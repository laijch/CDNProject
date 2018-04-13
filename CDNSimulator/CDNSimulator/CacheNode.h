#pragma once

class CacheNode
{
public:
	CacheNode(int id_);
	~CacheNode();
	void setDegree(int degree_);
	void setWeight(double weight_);
	void setProDis(double proDis_);
	void setFileNum(int num_);
	bool findFile(int file_);
	void LRUCache(int file_);
	double getPosX();
	double getPosY();
	void setPos(double x_, double y_);

private:
	int id;
	int degree;
	double weight;
	double probabilityDistribution;
	int fileNum;
	int replaceFileNum;
	int file[15];
	double positionX;
	double positionY;
};
