#pragma once
class User
{
public:
	User(int id_);
	~User();
	void setTargetFile(int file_);
	void setPosX(double x_);
	void setPosY(double y_);
	void resetPos(double distance_);

private:
	int id;
	int targetFile;
	double positionX;
	double positionY;
};
