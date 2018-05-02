#include "BAGenerator.h"
#include "CacheNode.h"
#include "User.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// 生成[a, b]间的随机数
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

	double userX, userY;
	int nodeX, nodeY, nodeID;
	int file;  // 用户请求的文件
	int cost = 0;  // 用户请求的文件在网络中所经历的距离
	// 用户未移动状态
	for (i = 0; i < 10240; i++) {
		userX = users[i].getPosX();
		userY = users[i].getPosY();
		// 确定用户离哪个节点近
		nodeX = (int)(userX * 10 + 5) / 10;
		nodeY = (int)(userY * 10 + 5) / 10;
		if (nodeX > 32) nodeX = 32;
		else if (nodeX < 1) nodeX = 1;
		if (nodeY > 32) nodeY = 32;
		else if (nodeY < 1) nodeY = 1;
        // 确定用户所属于的节点ID
		nodeID = (nodeX - 1) * 32 + (nodeY - 1);

		// 用户随机请求文件
		srand((unsigned)time(NULL));
		file = random(1, 15);

        // 该节点有请求文件
		if (cacheNodes[nodeID].findFile(file)) {
			++cost;
        }
        // 该节点无请求文件
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