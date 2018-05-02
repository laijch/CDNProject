#include "BAGenerator.h"
#include<stdio.h>  
#include<stdlib.h>  
#include<time.h>  
#include<string.h> 

// 网络规模，初始网络规模M_0，新节点连接的节点数M
#define NETWORK_SIZE 1024
#define M_0 4
#define M 3

BAGenerator::BAGenerator()
{
}

BAGenerator::~BAGenerator()
{
}

void BAGenerator::initial()
{
	if (!(decisionMaking = (NodePtr)malloc(sizeof(Node) * (NETWORK_SIZE + 1))))
	{
		printf("decisionMaking* malloc error\n");
		exit(0);
	}
	if (!(adjacentMatrix = (int**)malloc(sizeof(int*) * (NETWORK_SIZE + 1))))
	{
		printf("adjacentMatrix** malloc error\n");
		exit(0);
	}
	int i;
	for (i = 1; i <= NETWORK_SIZE; i++)
	{
		if (!(adjacentMatrix[i] = (int*)malloc(sizeof(int) * (NETWORK_SIZE + 1))))
		{
			printf("adjacentMatrix[%d]* malloc error\n", i);
			exit(0);
		}
	}
	if (!(initalNetwork = (int*)malloc(sizeof(int) * (M_0 + 1))))
	{
		printf("initalNetwork* malloc error\n");
		exit(0);
	}
}

/*
* 初始化：在NETWORK_SIZE中随机选择M_0个节点构成连通的网络。
* */
void BAGenerator::initalNetwork_M0_connected()
{
	int i, j, randomFirst, randomSecond;
	for (i = 1; i <= NETWORK_SIZE; i++)
		for (j = 1; j <= NETWORK_SIZE; j++)
			adjacentMatrix[i][j] = 0;
	// 随机产生M_0个节点  
	for (i = 1; i <= M_0; i++)
	{
		initalNetwork[i] = rand() % NETWORK_SIZE + 1;
		for (j = 1; j < i; j++)
			if (initalNetwork[i] == initalNetwork[j])
			{
				i--;
				break;
			}
	}
	for (i = 1; i < M_0; i++)
		adjacentMatrix[initalNetwork[i]][initalNetwork[i + 1]] = adjacentMatrix[initalNetwork[i + 1]][initalNetwork[i]] = 1;
	adjacentMatrix[initalNetwork[M_0]][initalNetwork[1]] = adjacentMatrix[initalNetwork[1]][initalNetwork[M_0]] = 1;

	//showAdjacentMatrix();  
	updateDecisionMakingData();
}

/*
* 通过adjacentMatrix更新decisionMaking数组
* */
void BAGenerator::updateDecisionMakingData()
{
	int i, j, totalDegree = 0;

	for (i = 1; i <= NETWORK_SIZE; i++)
		decisionMaking[i].degree = 0;
	for (i = 1; i <= NETWORK_SIZE; i++)
		for (j = 1; j <= NETWORK_SIZE; j++)
			decisionMaking[i].degree += adjacentMatrix[i][j];
	for (i = 1; i <= NETWORK_SIZE; i++)
		totalDegree += decisionMaking[i].degree;
	for (i = 1; i <= NETWORK_SIZE; i++)
		decisionMaking[i].weight = decisionMaking[i].degree / (double)totalDegree;
	decisionMaking[1].probabilityDistribution = decisionMaking[1].weight;
	for (i = 2; i <= NETWORK_SIZE; i++)
		decisionMaking[i].probabilityDistribution = decisionMaking[i - 1].probabilityDistribution + decisionMaking[i].weight;
}

/*
* 构造BA无标度网络模型
* */
void BAGenerator::generateFreeScaleNetwork()
{
	int i, k, j = 1, length = 0;
	int random_auxiliary_old[NETWORK_SIZE + 1];
	int random_auxiliary[NETWORK_SIZE + 1 - M_0];

	/*
	* 要保证每次引入一个<新的>的节点，所以要随机选择不重复的节点加入，并且把初始网络中的M_0个节点先删除
	* */
	for (i = 1; i <= NETWORK_SIZE; i++)
		random_auxiliary_old[i] = i;

	for (i = 1; i <= M_0; i++)
		random_auxiliary_old[initalNetwork[i]] = 0;
	for (i = 1; i <= NETWORK_SIZE; i++)
		if (random_auxiliary_old[i] != 0)
			random_auxiliary[j++] = random_auxiliary_old[i];

	/*
	* 添加新的节点构造无标度网络
	* */
	int new_node_index, new_node_value;
	double random_decision = 0.0;
	int targetNode;                 //表示找到的已经在网络中的将要连接的节点  
	length = NETWORK_SIZE - M_0;
	int flag;
	for (i = 1; i <= NETWORK_SIZE - M_0; i++)
	{
		new_node_index = rand() % length + 1;
		new_node_value = random_auxiliary[new_node_index];
		random_auxiliary[new_node_index] = random_auxiliary[length--];
		for (j = 1; j <= M; j++)        //根据概率连接到已存在网络中的M个节点，不可以重边，不可以自连。  
		{
			flag = 0;
			random_decision = (rand() % 1000) / (double)1000;
			for (k = 1; k <= NETWORK_SIZE; k++)
			{
				// 从第一个节点到最后一个节点比较probabilityDistribution和random_desction的大小，  
				// 由于probabilityDistribution是有序的，所以可以使用一些高级的算法来提高查找的效率.  
				if (decisionMaking[k].probabilityDistribution >= random_decision && decisionMaking[k].degree != 0 && adjacentMatrix[new_node_value][k] != 1)
				{
					/*
					*
					*  如何按照可能性大小来选择要连哪一个点：
					*         选择的已经在网络中的点是：随机产生的0-1之间的概率p，找这样的点：
					*         它的累加概率（probabilityDistribution）是大于p的最小的值所对应的点。
					*
					*/
					targetNode = k;
					flag = 1;
					break;
				}
			}
			if (flag == 0)
				/*
				* 之前少考虑了这种情况，因为总要选择一个网络中的点接入。但是当产生了比较大的随机概率p，可能
				* 在他后面（按probabilityDistribution来说）没有可选的点（要么选择过了，要么不在网络中），则重新开始
				*/
			{
				for (k = 1; k <= NETWORK_SIZE; k++)
				{
					if (decisionMaking[k].degree != 0 && adjacentMatrix[new_node_value][k] != 1)
					{
						targetNode = k;
						break;
					}
				}
			}
			//printf(" target node is %d\n", targetNode);  
			adjacentMatrix[new_node_value][targetNode] = adjacentMatrix[targetNode][new_node_value] = 1;
		}
		updateDecisionMakingData();     //else新选的加入节点和已有网络中的M个边都链接后再更新  
	}
}

void BAGenerator::showAdjacentMatrix()
{
	int i, j;
	int numberOfEdge = 0;
	printf("\tshow adjacentMatrix\n");
	for (i = 1; i <= NETWORK_SIZE; i++)
	{
		for (j = 1; j <= NETWORK_SIZE; j++)
		{
			/*printf("%d", adjacentMatrix[i][j]);*/
			if (adjacentMatrix[i][j] == 1)
				numberOfEdge++;
		}
		/*printf("\n");*/
	}
	printf("the number of edge is %d\n", numberOfEdge / 2);
}

void BAGenerator::BANetworkGenerate()
{
	srand((unsigned)time(NULL));
	initial();
	initalNetwork_M0_connected();
	generateFreeScaleNetwork();
}
