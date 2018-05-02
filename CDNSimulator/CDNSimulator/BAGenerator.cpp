#include "BAGenerator.h"
#include<stdio.h>  
#include<stdlib.h>  
#include<time.h>  
#include<string.h> 

// �����ģ����ʼ�����ģM_0���½ڵ����ӵĽڵ���M
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
* ��ʼ������NETWORK_SIZE�����ѡ��M_0���ڵ㹹����ͨ�����硣
* */
void BAGenerator::initalNetwork_M0_connected()
{
	int i, j, randomFirst, randomSecond;
	for (i = 1; i <= NETWORK_SIZE; i++)
		for (j = 1; j <= NETWORK_SIZE; j++)
			adjacentMatrix[i][j] = 0;
	// �������M_0���ڵ�  
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
* ͨ��adjacentMatrix����decisionMaking����
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
* ����BA�ޱ������ģ��
* */
void BAGenerator::generateFreeScaleNetwork()
{
	int i, k, j = 1, length = 0;
	int random_auxiliary_old[NETWORK_SIZE + 1];
	int random_auxiliary[NETWORK_SIZE + 1 - M_0];

	/*
	* Ҫ��֤ÿ������һ��<�µ�>�Ľڵ㣬����Ҫ���ѡ���ظ��Ľڵ���룬���Ұѳ�ʼ�����е�M_0���ڵ���ɾ��
	* */
	for (i = 1; i <= NETWORK_SIZE; i++)
		random_auxiliary_old[i] = i;

	for (i = 1; i <= M_0; i++)
		random_auxiliary_old[initalNetwork[i]] = 0;
	for (i = 1; i <= NETWORK_SIZE; i++)
		if (random_auxiliary_old[i] != 0)
			random_auxiliary[j++] = random_auxiliary_old[i];

	/*
	* ����µĽڵ㹹���ޱ������
	* */
	int new_node_index, new_node_value;
	double random_decision = 0.0;
	int targetNode;                 //��ʾ�ҵ����Ѿ��������еĽ�Ҫ���ӵĽڵ�  
	length = NETWORK_SIZE - M_0;
	int flag;
	for (i = 1; i <= NETWORK_SIZE - M_0; i++)
	{
		new_node_index = rand() % length + 1;
		new_node_value = random_auxiliary[new_node_index];
		random_auxiliary[new_node_index] = random_auxiliary[length--];
		for (j = 1; j <= M; j++)        //���ݸ������ӵ��Ѵ��������е�M���ڵ㣬�������رߣ�������������  
		{
			flag = 0;
			random_decision = (rand() % 1000) / (double)1000;
			for (k = 1; k <= NETWORK_SIZE; k++)
			{
				// �ӵ�һ���ڵ㵽���һ���ڵ�Ƚ�probabilityDistribution��random_desction�Ĵ�С��  
				// ����probabilityDistribution������ģ����Կ���ʹ��һЩ�߼����㷨����߲��ҵ�Ч��.  
				if (decisionMaking[k].probabilityDistribution >= random_decision && decisionMaking[k].degree != 0 && adjacentMatrix[new_node_value][k] != 1)
				{
					/*
					*
					*  ��ΰ��տ����Դ�С��ѡ��Ҫ����һ���㣺
					*         ѡ����Ѿ��������еĵ��ǣ����������0-1֮��ĸ���p���������ĵ㣺
					*         �����ۼӸ��ʣ�probabilityDistribution���Ǵ���p����С��ֵ����Ӧ�ĵ㡣
					*
					*/
					targetNode = k;
					flag = 1;
					break;
				}
			}
			if (flag == 0)
				/*
				* ֮ǰ�ٿ����������������Ϊ��Ҫѡ��һ�������еĵ���롣���ǵ������˱Ƚϴ���������p������
				* �������棨��probabilityDistribution��˵��û�п�ѡ�ĵ㣨Ҫôѡ����ˣ�Ҫô���������У��������¿�ʼ
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
		updateDecisionMakingData();     //else��ѡ�ļ���ڵ�����������е�M���߶����Ӻ��ٸ���  
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
