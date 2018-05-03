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

// ����[a, b]��������
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
void init();  // ��ʼ��
int getFileCost(int file, int nodeID_);  // �����û���ȡ�ļ�ʱ�������Ļ��ѣ�·����
double getAvgCostOfInitial();  // ��ȡ��ʼ״̬��ƽ������
double getAvgCostByRandomMove();  // ��ȡ�û�����ƶ�ʱ��ƽ������
double getAvgCostByPrgMove();  // ��ȡ�û������ʷֲ��ƶ�ʱ��ƽ������
double getPrg(double rg);  // ��ȡ�û��ƶ��뾶rg�����룩�ĸ���


int main() {
    init();

    // �����������
    srand((unsigned)time(NULL));

    /*cout << std::left << setw(15) << "userID" << ": " 
        << setw(15) << "userPosX" << setw(15) << "userPosY"
        << setw(15) << "nodeID" << setw(15) << "file" 
        << setw(15) << "cost" << setw(15) << "allCost" << endl;*/

    double avgCost;
    // (0) ��ʼ״̬�µ�ƽ������
    cout << "��ʼ״̬�µ�ƽ������" << endl;
    avgCost = getAvgCostOfInitial();
    // �����������txt��
    ofstream outfile;
    outfile.open("avgCost.txt");
    cout << "0: " << fixed << setprecision(8) << avgCost << endl;
    outfile << "0 " << fixed << setprecision(8) << avgCost << endl;

    int time = 50;

    // ѡ���ƶ�ģʽ
    int moveMode = 1;
    cout << "��ѡ���û����ƶ�ģʽ��1 - ����ƶ�  2 - �����ɷֲ��ƶ�" << endl;
    cin >> moveMode;
    switch (moveMode)
    {
        case 1: {
            // (1) �û������time��״̬�µ�ƽ������
            cout << "�û�����ƶ�״̬�µ�ƽ������" << endl;
            for (int i = 1; i <= time; i++) {
                avgCost = getAvgCostByRandomMove();
                //cout << i << ": " << fixed << setprecision(8) << avgCost << endl;
                outfile << i << " " << fixed << setprecision(8) << avgCost << endl;
            }
            break;
        }
        case 2: {
            // (2) �û������ʷֲ�����time��״̬�µ�ƽ������
            cout << "�û������ʷֲ��ƶ�״̬�µ�ƽ������" << endl;
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

    // �ر��ļ�
    outfile.close();

	return 0;
}

void init() {
    int nodeIndex = 0;
    int userIndex = 0;
    // ��ʼ��id ���꣬�������1��1km
    for (int i = 1; i <= GRID_LEN; i++) {
        for (int j = 1; j <= GRID_LEN; j++) {
            cacheNodes[nodeIndex].setID(nodeIndex);
            // NODE_NUM ���ڵ���ȷֲ���(GRID_LEN*GRID_LEN)��������
            cacheNodes[nodeIndex].setPos(i, j);
            for (int q = 0; q < USER_NODE_RATIO; q++) {
                users[userIndex].setID(userIndex);
                // USER_NUM ���û����ȷֲ���NODE_NUM ���ڵ���
                users[userIndex].setPosX(i);
                users[userIndex].setPosY(j);
                ++userIndex;
            }
            ++nodeIndex;
        }
    }

    // ����BA�ޱ������
    baGen.BANetworkGenerate();
}

int getFileCost(int file, int nodeID_) {
    int cost = 0;
    int nodeID = nodeID_;

    if (cacheNodes[nodeID].findFile(file)) {
        return 0;
    }

    // �����������BFS
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
            // ��ýڵ�������δ���ʹ�
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
    int file;  // �û�������ļ�
    int cost = 0;  // �û�������ļ����������������ľ���
    int allCost = 0;  // ������
    // �û�δ�ƶ�״̬
    for (int i = 0; i < USER_NUM; i++) {
        // ��ȡ�û����ڽڵ��ID
        nodeID = users[i].getNearNodeID();

        // �û���������ļ�
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
    int file;  // �û�������ļ�
    int cost = 0;  // �û�������ļ����������������ľ���
    int allCost = 0;  // ������
    // �û�����ƶ�״̬
    for (int i = 0; i < USER_NUM; i++) {
        // ����ı��û���λ��
        double step = (double)random(0, 31);
        users[i].resetPos(step);

        // ��ȡ�û����ڽڵ��ID
        nodeID = users[i].getNearNodeID();

        // �û���������ļ�
        file = random(1, 15);

        cost = getFileCost(file, nodeID);
        allCost += cost;
    }

    return (double(allCost) / USER_NUM);
}

double getAvgCostByPrgMove() {
    int nodeID;
    int file;  // �û�������ļ�
    int cost = 0;  // �û�������ļ����������������ľ���
    int allCost = 0;  // ������
    double prg;  // ĳ�����ĸ���
    int userNumOfStep[32];  // ĳ�������û�����
    userNumOfStep[31] = USER_NUM;
    // ������ΧΪ[0, 32)������
    for (int step = 0; step < 31; step++) {
        prg = getPrg((double)step);
        userNumOfStep[step] = (int)((double)USER_NUM * prg);
        userNumOfStep[31] = userNumOfStep[31] - userNumOfStep[step];  // �����ƶ�31�������û�����
    }

    // �û������ʷֲ��ƶ�״̬
    for (int i = 0; i < USER_NUM; i++) {
        int step = 0;
        // �����ʷֲ������ȡ�ƶ�����
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

        // ��ȡ�û����ڽڵ��ID
        nodeID = users[i].getNearNodeID();

        // �û���������ļ�
        file = random(1, 15);

        cost = getFileCost(file, nodeID);
        allCost += cost;
    }
    return (double(allCost) / USER_NUM);
}

double getPrg(double rg) {
    // ��������
    double rg0 = 1.99, br = 1.65, k = 350;  // ������1(1.99, 1.65, 350)
    double Prg = pow(rg + rg0, -1.0*br)*exp(-1 * rg / k);
    return Prg;
}