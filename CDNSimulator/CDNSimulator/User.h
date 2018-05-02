#pragma once
class User
{
public:
	User();
	~User();
	void setID(int id_);
	void setTargetFile(int file_);
	void setPosX(double x_);
	void setPosY(double y_);
	double getPosX();
	double getPosY();
	void resetPos(double distance_);
    int getNearNodeID();

private:
	int id;
	int targetFile;
	double positionX;
	double positionY;
};
