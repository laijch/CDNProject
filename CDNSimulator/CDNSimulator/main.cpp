#include "BAGenerator.h"
#include "CacheNode.h"
#include "User.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// ����[a, b]��������
#define random(a, b) (rand() % (b - a + 1) + a)

using namespace std;

int main() {
	CacheNode cacheNodes[1024];
	User users[10240];
	BAGenerator baGen;
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

	double userX, userY;
	int nodeX, nodeY, nodeID;
	int file;  // �û�������ļ�
	int cost = 0;  // �û�������ļ����������������ľ���
	// �û�δ�ƶ�״̬
	for (i = 0; i < 10240; i++) {
		userX = users[i].getPosX();
		userY = users[i].getPosY();
		// ȷ���û����ĸ��ڵ��
		nodeX = (int)(userX * 10 + 5) / 10;
		nodeY = (int)(userY * 10 + 5) / 10;
		if (nodeX > 32) nodeX = 32;
		else if (nodeX < 1) nodeX = 1;
		if (nodeY > 32) nodeY = 32;
		else if (nodeY < 1) nodeY = 1;
        // ȷ���û������ڵĽڵ�ID
		nodeID = (nodeX - 1) * 32 + (nodeY - 1);

		// �û���������ļ�
		srand((unsigned)time(NULL));
		file = random(1, 15);

        // �ýڵ��������ļ�
		if (cacheNodes[nodeID].findFile(file)) {
			++cost;
        }
        // �ýڵ��������ļ�
        else {
            
        }
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