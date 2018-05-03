#include "BAGenerator.h"
#include "CacheNode.h"
#include "User.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <math.h>

// 生成[a, b]间的随机数
#define random(a, b) (rand() % (b - a + 1) + a)
#define GRID_LEN 32
#define NODE_NUM 1024
#define USER_NUM 102400
#define USER_NODE_RATIO 100
#define MAX_DISTANCE 2000

using namespace std;

BAGenerator baGen;
CacheNode cacheNodes[NODE_NUM];
User users[USER_NUM];
void init();  // 初始化
int getFileCost(int file, int nodeID_);  // 计算用户获取文件时所经过的花费（路径）
double getAvgCostOfInitial();  // 获取初始状态的平均消耗
double getAvgCostByRandomMove();  // 获取用户随机移动时的平均消耗
double getAvgCostByPrgMove();  // 获取用户按幂率分布移动时的平均消耗
double getPrg(double rg);  // 获取用户移动半径rg（距离）的概率


int main() {
    init();

    // 生成随机种子
    srand((unsigned)time(NULL));

    /*cout << std::left << setw(15) << "userID" << ": " 
        << setw(15) << "userPosX" << setw(15) << "userPosY"
        << setw(15) << "nodeID" << setw(15) << "file" 
        << setw(15) << "cost" << setw(15) << "allCost" << endl;*/

    double avgCost;
    // (0) 初始状态下的平均消耗
    cout << "初始状态下的平均消耗" << endl;
    avgCost = getAvgCostOfInitial();
    // 将数据输出至txt中
    ofstream outfile;
    outfile.open("avgCost.txt");
    cout << "0: " << fixed << setprecision(8) << avgCost << endl;
    outfile << "0 " << fixed << setprecision(8) << avgCost << endl;

    int time = 50;

    // 选择移动模式
    int moveMode = 1;
    cout << "请选择用户的移动模式：1 - 随机移动  2 - 按幂律分布移动" << endl;
    cin >> moveMode;
    switch (moveMode)
    {
        case 1: {
            // (1) 用户随机走time步状态下的平均消耗
            cout << "用户随机移动状态下的平均消耗" << endl;
            for (int i = 1; i <= time; i++) {
                avgCost = getAvgCostByRandomMove();
                //cout << i << ": " << fixed << setprecision(8) << avgCost << endl;
                outfile << i << " " << fixed << setprecision(8) << avgCost << endl;
            }
            break;
        }
        case 2: {
            // (2) 用户按幂率分布行走time步状态下的平均消耗
            cout << "用户按幂率分布移动状态下的平均消耗" << endl;
            for (int i = 1; i <= time; i++) {
                avgCost = getAvgCostByPrgMove();
                //cout << i << ": " << fixed << setprecision(8) << avgCost << endl;
                outfile << i << " " << fixed << setprecision(8) << avgCost << endl;
            }
            break;
        }
        default:
            break;
    }

    // 关闭文件
    outfile.close();

	return 0;
}

void init() {
    int nodeIndex = 0;
    int userIndex = 0;
    // 初始化id 坐标，坐标比例1：1km
    for (int i = 1; i <= GRID_LEN; i++) {
        for (int j = 1; j <= GRID_LEN; j++) {
            cacheNodes[nodeIndex].setID(nodeIndex);
            // NODE_NUM 个节点均匀分布在(GRID_LEN*GRID_LEN)的网格中
            cacheNodes[nodeIndex].setPos(i, j);
            for (int q = 0; q < USER_NODE_RATIO; q++) {
                users[userIndex].setID(userIndex);
                // USER_NUM 个用户均匀分布在NODE_NUM 个节点上
                users[userIndex].setPosX(i);
                users[userIndex].setPosY(j);
                ++userIndex;
            }
            ++nodeIndex;
        }
    }

    // 生成BA无标度网络
    baGen.BANetworkGenerate();
}

int getFileCost(int file, int nodeID_) {
    int cost = 0;
    int nodeID = nodeID_;

    if (cacheNodes[nodeID].findFile(file)) {
        return 0;
    }

    // 广度优先搜索BFS
    bool visitedNodes[NODE_NUM];
    int distance[NODE_NUM];
    for (int i = 0; i < NODE_NUM; i++) {
        visitedNodes[i] = false;
        distance[i] = MAX_DISTANCE;
    }

    queue<int> nodeQueue;
    nodeQueue.push(nodeID);
    visitedNodes[nodeID] = true;
    distance[nodeID] = 0;
    while (!nodeQueue.empty()) {
        nodeID = nodeQueue.front();
        nodeQueue.pop();
        for (int i = 1; i <= NODE_NUM; i++) {
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

double getAvgCostOfInitial() {
    int nodeID;
    int file;  // 用户请求的文件
    int cost = 0;  // 用户请求的文件在网络中所经历的距离
    int allCost = 0;  // 总消耗
    // 用户未移动状态
    for (int i = 0; i < USER_NUM; i++) {
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

    return (double(allCost) / USER_NUM);
}

double getAvgCostByRandomMove() {
    int nodeID;
    int file;  // 用户请求的文件
    int cost = 0;  // 用户请求的文件在网络中所经历的距离
    int allCost = 0;  // 总消耗
    // 用户随机移动状态
    for (int i = 0; i < USER_NUM; i++) {
        // 随机改变用户的位置
        double step = (double)random(0, 31);
        users[i].resetPos(step);

        // 获取用户所在节点的ID
        nodeID = users[i].getNearNodeID();

        // 用户随机请求文件
        file = random(1, 15);

        cost = getFileCost(file, nodeID);
        allCost += cost;
    }

    return (double(allCost) / USER_NUM);
}

double getAvgCostByPrgMove() {
    int nodeID;
    int file;  // 用户请求的文件
    int cost = 0;  // 用户请求的文件在网络中所经历的距离
    int allCost = 0;  // 总消耗
    double prg;  // 某步长的概率
    int userNumOfStep[32];  // 某步长的用户数量
    userNumOfStep[31] = USER_NUM;
    // 步长范围为[0, 32)，整数
    for (int step = 0; step < 31; step++) {
        prg = getPrg((double)step);
        userNumOfStep[step] = (int)((double)USER_NUM * prg);
        userNumOfStep[31] = userNumOfStep[31] - userNumOfStep[step];  // 计算移动31步长的用户数量
    }

    // 用户按幂率分布移动状态
    for (int i = 0; i < USER_NUM; i++) {
        int step = 0;
        // 按幂率分布随机获取移动步长
        while (1) {
            step = random(0, 31);
            if (userNumOfStep[step] != 0) {
                userNumOfStep[step]--;
                break;
            }
            else {
                int step = random(0, 31);
            }
        }

        users[i].resetPos((double)step);

        // 获取用户所在节点的ID
        nodeID = users[i].getNearNodeID();

        // 用户随机请求文件
        file = random(1, 15);

        cost = getFileCost(file, nodeID);
        allCost += cost;
    }
    return (double(allCost) / USER_NUM);
}

double getPrg(double rg) {
    // 参数设置
    double rg0 = 1.99, br = 1.65, k = 350;  // 参数组1(1.99, 1.65, 350)
    double Prg = pow(rg + rg0, -1.0*br)*exp(-1 * rg / k);
    return Prg;
}