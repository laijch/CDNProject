#pragma once
struct Node;
typedef struct Node* NodePtr;
typedef struct Node {
	int degree;
	double weight;
	double probabilityDistribution;
}Node;

class BAGenerator
{
public:
	BAGenerator();
	~BAGenerator();
	void initial();
	void initalNetwork_M0_connected();
	void updateDecisionMakingData();
	void generateFreeScaleNetwork();
	void showAdjacentMatrix();
	void BANetworkGenerate();
	int** adjacentMatrix;

private:
	Node* decisionMaking;
	int* initalNetwork;
};