#include "User.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>

using namespace std;

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
	// 随机生成旋转角度，随机种子在main.cpp中产生，不能重复生成
	int angle = rand() % 360;
    //cout << angle << " ";
	double PI = 3.14159265;
	positionX = positionX + distance_ * cos(angle*PI / 180.0);
	positionY = positionY + distance_ * sin(angle*PI / 180.0);
    // 超出范围的取边界值
    if (positionX > 32) positionX = 32;
    else if (positionX < 1) positionX = 1;
    if (positionY > 32) positionY = 32;
    else if (positionY < 1) positionY = 1;
}

// 确定用户离哪个节点近
int User::getNearNodeID()
{
    int nodeX, nodeY, nodeID;
    nodeX = (int)(positionX * 10 + 5) / 10;
    nodeY = (int)(positionY * 10 + 5) / 10;
    // 超出范围的取边界值
    if (nodeX > 32) nodeX = 32;
    else if (nodeX < 1) nodeX = 1;
    if (nodeY > 32) nodeY = 32;
    else if (nodeY < 1) nodeY = 1;
    // 确定用户所属于的节点ID
    nodeID = (nodeX - 1) * 32 + (nodeY - 1);
    return nodeID;
}
