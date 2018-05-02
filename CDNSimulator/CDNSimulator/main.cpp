#include "BAGenerator.h"
#include "CacheNode.h"
#include "User.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <memory.h>

// ����[a, b]��������
#define random(a, b) (rand() % (b - a + 1) + a)

using namespace std;

BAGenerator baGen;
CacheNode cacheNodes[1024];
User users[10240];
int getFileCost(int file, int nodeID_);  // �����û���ȡ�ļ�ʱ�������Ļ��ѣ�·����
double getPrg(double rg);  // ��ȡ�û��ƶ��뾶rg�����룩�ĸ���


int main() {
	int nodeIndex = 0;
	int userIndex = 0;
	int i = 1;
	int j = 1;
	int q = 1;
	// ��ʼ��id ���꣬�������1��1km
	for (i = 1; i <= 32; i++) {
		for (j = 1; j <= 32; j++) {
			cacheNodes[nodeIndex].setID(nodeIndex);
			// 1024���ڵ���ȷֲ���32*32��������
			cacheNodes[nodeIndex].setPos(i, j);
			for (q = 0; q < 10; q++) {
				users[userIndex].setID(userIndex);
				// 10240���û����ȷֲ���1024���ڵ���
				users[userIndex].setPosX(i);
				users[userIndex].setPosY(j);
				++userIndex;
			}
			++nodeIndex;
		}
	}

	// ����BA�ޱ������
	baGen.BANetworkGenerate();

	int nodeID;
	int file;  // �û�������ļ�
	int cost = 0;  // �û�������ļ����������������ľ���
    int allCost = 0;  // ������

    srand((unsigned)time(NULL));

    /*cout << std::left << setw(15) << "userID" << ": " 
        << setw(15) << "userPosX" << setw(15) << "userPosY"
        << setw(15) << "nodeID" << setw(15) << "file" 
        << setw(15) << "cost" << setw(15) << "allCost" << endl;*/

	// �û�δ�ƶ�״̬
    for (i = 0; i < 10240; i++) {
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

    cout << "0: " << fixed << setprecision(8) << double(allCost) / 10240 << endl;
    //cout << "allCost: " << allCost << " ";
    allCost = 0;

    for (q = 1; q <= 100; q++) {
        // �û�����ƶ�״̬
        for (i = 0; i < 10240; i++) {
            // ����ı��û���λ��
            double step = (double)random(1, 31);
            users[i].resetPos(step);

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

    // �����������BFS
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

double getPrg(double rg) {
    // ��������
    double rg0 = 1.99, br = 1.65, k = 350;
    double Prg = pow(rg + rg0, -1.0*br)*exp(-1 * rg / k);
    return Prg;
}