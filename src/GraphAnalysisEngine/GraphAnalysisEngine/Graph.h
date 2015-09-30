/*
* �ļ���:Graph.h
* �ļ�˵��:ͼ�����࣬��װ��ͼ�ļ�����С�������ͽӽ����������ĶȵĲ���
*
* ����:��ͩͮ,����:2014��01��
* Copyright (c) 2014�� ��ͩͮ. All rights reserved.
*/

#ifndef GRAPH_H
#define GRAPH_H

class Graph
{
public:
	//���뵼�����ļ���
	char inFile[256];
	char outFile[256];

	int node;
	int edge;
	//���ñ��б�洢
	int* source;
	int* target;
	double* weight;
	//�ڽӾ���
	double** matrix;

	//��С�������Ĵ洢
	int b_count;
	int* b_source;
	int* b_target;
	int* b_group;
	double* b_weight;

	//���·�����Ⱦ���
	double** minPath;
	//���·��·������
	int** SepcificPath;
	//���·����������
	int** minPathCount;
	//���Ķ�
	double* ClosenessCentrality;
	double* BetweennessCentrality;

public:
	Graph();
	~Graph();
	void destroy();

	//���õ��뵼����JSON�ļ���
	void setInFile(char* fileName);
	void setOutFile(char* fileName);

	//ͼ�ĵ��뵼��
	//���룺txt��ʽ
	bool importGraph();
	//����
	static void sortGraph(int* s, int* t, double* w, int start, int end);
	static void qSortEdge(int* s, int* t, double* w, int start, int end);
	//������json��ʽ
	bool exportGraph();

	//������С������orɭ�֣�Prim�㷨��
	//nΪ��ʼ�㣬��n < 0����������С����ɭ��
	void generateMinSpanningTree(int s = -1);
	//������С����s
	bool exportMinSpanningTree();

	//�����Ż���Floyd-warshall�㷨�����·��
	void calShortestPathByFloyd();
	//����ӽ����Ķ�
	void calClosenessCentrality();
	//����������Ķ�
	void calBetweennessCentrality();
	//����������Ķ�����
	bool exportCentralityToTXT();
	bool exportBCentralityToJSON();
	bool exportCCentralityToJSON();
	//���絼��API
	void printGraphInJSON(char* outputfile);
	//��С�����������뵼��API��������json�ļ��еı���С����������֦
	//nΪ��ʼ�㣬��n < 0����������С����ɭ��
	void printMinSpanningTree(char* outputfile, int n = -1);
	//���ӽ����ĶȺͽ������Ķȵ�����TXT��API
	void printCentralityToTXT(char* outputfile);
	//���ӽ����ĶȺͽ������Ķȵ�����JSON��API
	void printBCentralityToJSON(char* outputfile);
	void printCCentralityToJSON(char* outputfile);
	//�������������·��API,������json�ļ��еı߼�Ϊs��e�����·��
	bool printShortestPath(char* outputfile, int s, int e);
};

#endif