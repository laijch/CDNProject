#include "BAGenerator.h"
#include "CacheNode.h"
#include "User.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <memory.h>

// 生成[a, b]间的随机数
#define random(a, b) (rand() % (b - a + 1) + a)

using namespace std;

BAGenerator baGen;
CacheNode cacheNodes[1024];
User users[10240];
int getFileCost(int file, int nodeID_);  // 计算用户获取文件时所经过的花费（路径）
double getPrg(double rg);  // 获取用户移动半径rg（距离）的概率


int main() {
	int nodeIndex = 0;
	int userIndex = 0;
	int i = 1;
	int j = 1;
	int q = 1;
	// 初始化id 坐标，坐标比例1：1km
	for (i = 1; i <= 32; i++) {
		for (j = 1; j <= 32; j++) {
			cacheNodes[nodeIndex].setID(nodeIndex);
			// 1024个节点均匀分布在32*32的网格中
			cacheNodes[nodeIndex].setPos(i, j);
			for (q = 0; q < 10; q++) {
				users[userIndex].setID(userIndex);
				// 10240个用户均匀分布在1024个节点上
				users[userIndex].setPosX(i);
				users[userIndex].setPosY(j);
				++userIndex;
			}
			++nodeIndex;
		}
	}

	// 生成BA无标度网络
	baGen.BANetworkGenerate();

	int nodeID;
	int file;  // 用户请求的文件
	int cost = 0;  // 用户请求的文件在网络中所经历的距离
    int allCost = 0;  // 总消耗

    srand((unsigned)time(NULL));

    /*cout << std::left << setw(15) << "userID" << ": " 
        << setw(15) << "userPosX" << setw(15) << "userPosY"
        << setw(15) << "nodeID" << setw(15) << "file" 
        << setw(15) << "cost" << setw(15) << "allCost" << endl;*/

	// 用户未移动状态
    for (i = 0; i < 10240; i++) {
        // 获取用户所在节点的ID
        nodeID = users[i].getNearNodeID();

        // 用户随机请求文件
        file = random(1, 15);
        
        cost = getFileCost(file, nodeID);
        allCost += cost;

        /*cout << std::left << setw(15) << i << ": " 
            << setw(15) << users[i].getPosX() << setw(15) << users[i].getPosY() 
            << setw(15) << nodeID << setw(15) << file 
            << setw(15) << cost << setw(15) << allCost << endl;*/
    }

    cout << "0: " << fixed << setprecision(8) << double(allCost) / 10240 << endl;
    //cout << "allCost: " << allCost << " ";
    allCost = 0;

    for (q = 1; q <= 100; q++) {
        // 用户随机移动状态
        for (i = 0; i < 10240; i++) {
            // 随机改变用户的位置
            double step = (double)random(1, 31);
            users[i].resetPos(step);

            // 获取用户所在节点的ID
            nodeID = users[i].getNearNodeID();

            // 用户随机请求文件
            file = random(1, 15);

            cost = getFileCost(file, nodeID);
            allCost += cost;

            /*cout << std::left << setw(15) << i << ": "
                << setw(15) << users[i].getPosX() << setw(15) << users[i].getPosY()
                << setw(15) << nodeID << setw(15) << file
                << setw(15) << cost << setw(15) << allCost << endl;*/
        }

        cout << q << ": " << fixed << setprecision(8) << double(allCost) / 10240 << endl;
        //cout << "allCost: " << allCost << " ";
        allCost = 0;
    }

	/*for (i = 0; i < 10; i++) {
		for (j = 0; j < 15; j++) {
			cout << cacheNodes[i].file[j] << ' ';
		}
		cout << endl;
	}*/
	/*int num;
	for (i = 1; i <= 10; i++) {
		num = 0;
		for (j = 1; j <= 1024; j++) {
			if (baGen.adjacentMatrix[i][j] != 0) {
				num += baGen.adjacentMatrix[i][j];
				cout << j << ' ';
			}
		}
		cout << num << endl;
	}*/
	return 0;
}

int getFileCost(int file, int nodeID_) {
    int cost = 0;
    int nodeID = nodeID_;

    if (cacheNodes[nodeID].findFile(file)) {
        return 0;
    }

    // 广度优先搜索BFS
    bool visitedNodes[1024];
    int distance[1024];
    for (int i = 0; i < 1024; i++) {
        visitedNodes[i] = false;
        distance[i] = 2000;
    }

    queue<int> nodeQueue;
    nodeQueue.push(nodeID);
    visitedNodes[nodeID] = true;
    distance[nodeID] = 0;
    while (!nodeQueue.empty()) {
        nodeID = nodeQueue.front();
        nodeQueue.pop();
        for (int i = 1; i <= 1024; i++) {
            // 与该节点相连且未访问过
            if ((baGen.adjacentMatrix[nodeID + 1][i] == 1) && ((nodeID + 1) != i) 
                && (!visitedNodes[i - 1])) {
                visitedNodes[i - 1] = true;
                distance[i - 1] = distance[nodeID] + 1;
                nodeQueue.push(i - 1);
                if (cacheNodes[i - 1].findFile(file)) {
                    cacheNodes[nodeID].LRUCache(file); // LCD + LRU
                    return distance[i - 1];
                }
            }
        }
    }
    return cost;
}

double getPrg(double rg) {
    // 参数设置
    double rg0 = 1.99, br = 1.65, k = 350;
    double Prg = pow(rg + rg0, -1.0*br)*exp(-1 * rg / k);
    return Prg;
}