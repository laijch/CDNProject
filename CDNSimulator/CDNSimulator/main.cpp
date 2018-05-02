#include "BAGenerator.h"
#include "CacheNode.h"
#include "User.h"
#include <iostream>
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

	// �û�δ�ƶ�״̬
    for (i = 0; i < 10240; i++) {
        // ��ȡ�û����ڽڵ��ID
        nodeID = users[i].getNearNodeID();

        // �û���������ļ�
        srand((unsigned)time(NULL));
        file = random(1, 15);

        cost = cost + getFileCost(file, nodeID);
    }

    cout << cost / 10240;

    // �û�����ƶ�״̬
    for (i = 0; i < 10240; i++) {
        // ����ı��û���λ��
        srand((unsigned)time(NULL));
        users[i].resetPos(random(1, 32));

        // ��ȡ�û����ڽڵ��ID
        nodeID = users[i].getNearNodeID();

        // �û���������ļ�
        file = random(1, 15);

        cost = cost + getFileCost(file, nodeID);
    }

    cout << cost / 10240;

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
    //// �ýڵ��������ļ�
    //if (cacheNodes[nodeID].findFile(file)) {
    //    accessLength = 0;  // �������·������
    //    return 1;
    //}
    //// �ýڵ��������ļ���Ѱ���и��ļ��Ľڵ�
    //else {
    //    ++accessLength;
    //    for (int j = 1; j <= 1024; j++) {
    //        // ��ýڵ�����
    //        if (baGen.adjacentMatrix[nodeID][j] == 1 && nodeID != j) {
    //            for (int i = 0; i < accessLength; i++) {
    //                if (j == accessNodes[i]) {
    //                    return 0;
    //                }
    //            }
    //            cost = cost + getFileCost(file, j) + 1;
    //        }
    //    }
    //}

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