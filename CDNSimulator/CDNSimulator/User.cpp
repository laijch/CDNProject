#include "User.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

User::User()
{
	id = 0;
	targetFile = 0;
	positionX = 0.0;
	positionY = 0.0;
}

User::~User()
{
}

void User::setID(int id_)
{
	id = id_;
}

void User::setTargetFile(int file_)
{
	targetFile = file_;
}

void User::setPosX(double x_)
{
	positionX = x_;
}

void User::setPosY(double y_)
{
	positionY = y_;
}

double User::getPosX()
{
	return positionX;
}

double User::getPosY()
{
	return positionY;
}

void User::resetPos(double distance_)
{
	// 随机生成旋转角度
	srand((unsigned)time(NULL));
	int angle = rand() % 360;
	double PI = 3.14159265;
	positionX = positionX + distance_ * cos(angle*PI / 180.0);
	positionY = positionY + distance_ * sin(angle*PI / 180.0);
}
