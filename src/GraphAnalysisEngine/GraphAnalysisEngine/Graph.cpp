/*
* �ļ���:Graph.cpp
* �ļ�˵��:ͼ�����࣬��װ��ͼ�ļ�����С�������ͽӽ����������ĶȵĲ���
*
* ����:��ͩͮ,����:2014��01��
* Copyright (c) 2014�� ��ͩͮ. All rights reserved.
*/

#include "stdafx.h"
#include "Graph.h"
#include <cstring>
#include <fstream>
#include <cmath>
using namespace std;

Graph::Graph()
{
	source = NULL;
	target = NULL;
	weight = NULL;
	matrix = NULL;
	b_source = NULL;
	b_target = NULL; 
	b_group = NULL;
	b_weight = NULL;
	minPath = NULL;
	minPathCount = NULL;
	SepcificPath = NULL;
	ClosenessCentrality = NULL;
	BetweennessCentrality = NULL;
}

Graph::~Graph()
{
	destroy();
}

void Graph::destroy()
{
	if (source)	delete []source;
	if (target) delete []target;
	if (weight) delete []weight;
	if (b_source) delete []b_source;
	if (b_target) delete []b_target;
	if (b_group) delete []b_group;
	if (b_weight) delete []b_weight;
	if (matrix)
	{
		for (int i = 0; i < node; i++)
			delete [](matrix[i]);
		delete []matrix;
	}
	if (minPath)
	{
		for (int i = 0; i < node; i++)
			delete [](minPath[i]);
		delete []minPath;
	}
	if (minPathCount)
	{
		for (int i = 0; i < node; i++)
			delete [](minPathCount[i]);
		delete []minPathCount;
	}
	if (SepcificPath)
	{
		for (int i = 0; i < node; i++)
			delete [](SepcificPath[i]);
		delete []SepcificPath;
	}
	if (ClosenessCentrality) delete []ClosenessCentrality;
	if (BetweennessCentrality) delete []BetweennessCentrality;
	source = NULL;
	target = NULL;
	weight = NULL;
	matrix = NULL;
	b_source = NULL;
	b_target = NULL; 
	b_group = NULL;
	b_weight = NULL;
	minPath = NULL;
	minPathCount = NULL;
	SepcificPath = NULL;
	ClosenessCentrality = NULL;
	BetweennessCentrality = NULL;
}

//���õ��뵼�����ļ���
void Graph::setInFile(char* fileName)
{
	destroy();
	strcpy(inFile, fileName);
	importGraph();
	sortGraph(source, target, weight, 0, edge - 1);
}

void Graph::setOutFile(char* fileName)
{
	strcpy(outFile, fileName);
}

//ͼ�ĵ��뵼��
//���룺txt��ʽ
bool Graph::importGraph()
{
	ifstream fileInput(inFile);
	if (!fileInput)
		return false;

	fileInput >> node >> edge;
	source = new int[edge];
	target = new int[edge];
	weight = new double[edge];
	matrix = new double*[node];
	for (int i = 0; i < node; i++)
	{
		matrix[i] = new double[node];
		for (int j = 0; j < node; j++)
			matrix[i][j] = DBL_MAX;
		matrix[i][i] = 0;
	}

	char* buf = new char[edge * 500];
	char* ptr = buf;
	fileInput.read(buf, edge * 500);

	for (int i = 0; i < edge; i++)
	{
		source[i] = strtol(ptr, &ptr, 0);
		target[i] = strtol(ptr, &ptr, 0);
		weight[i] = strtod(ptr, &ptr);
		matrix[source[i]][target[i]] = weight[i];
		matrix[target[i]][source[i]] = weight[i];
	}

	fileInput.close();
	delete []buf;
	return true;
}

//����
void Graph::sortGraph(int* s, int* t, double* w, int start, int end)
{
	int tmp;
	for (int i = start; i <= end; i++)
	{
		if (s[i] > t[i])
		{
			tmp = s[i];
			s[i] = t[i];
			t[i] = tmp;
		}
	}
	qSortEdge(s, t, w, start, end);
}

void Graph::qSortEdge(int* s, int* t, double* w, int start, int end)
{
	if (start >= end)	return;

	int i = start, j = end;
	int key = (i + j) / 2;
	int tmp_st;
	double tmp_w;
	while (i < j)
	{
		while (s[i] < s[key] || (s[i] == s[key] && t[i] < t[key]))
			i++;
		while (s[j] > s[key] || (s[j] == s[key] && t[j] > t[key]))
			j--;
		if (i <= j)
		{
			tmp_st = s[i];
			s[i] = s[j];
			s[j] = tmp_st;
			tmp_st = t[i];
			t[i] = t[j];
			t[j] = tmp_st;
			tmp_w = w[i];
			w[i] = w[j];
			w[j] = tmp_w;
			i++;
			j--;
		}
	}

	if (start < j)	qSortEdge(s, t, w, start, j);
	if (end > i)	qSortEdge(s, t, w, i, end);
}

//������json��ʽ
bool Graph::exportGraph()
{
	ofstream fileOutput(outFile);
	if (!fileOutput)
		return false;

	char* buf = new char[node * 35 + edge * 55];
	memset(buf, 0, sizeof(char) * (node * 35 + edge * 55));
	char* ptr = buf;

	ptr += sprintf(ptr, "{\n\"nodes\":[\n");
	for (int i = 0; i < node - 1; i++)
		ptr += sprintf(ptr, "\t{\"name\":\"%d\", \"group\":\"%d\"},\n",
		i, i / 100);
	ptr += sprintf(ptr, "\t{\"name\":\"%d\", \"group\":\"%d\"}\n\t],\n",
		node - 1, (node - 1) / 100);

	ptr += sprintf(ptr, "\t\"links\":[\n");
	for (int i = 0; i < edge - 1; i++)
		ptr += sprintf(ptr, "\t{\"source\":%d, \"target\":%d, \"value\":%lf},\n",
		source[i], target[i], weight[i]);
	ptr += sprintf(ptr, "\t{\"source\":%d, \"target\":%d, \"value\":%lf}\n\t]\n}\n",
		source[edge-1], target[edge-1], weight[edge-1]);

	fileOutput.write(buf, strlen(buf));
	fileOutput.close();
	delete []buf;
	return true;
}

//������С��������Prim�㷨��
//nΪ��ʼ�㣬��n < 0����������С����ɭ��
void Graph::generateMinSpanningTree(int s)
{
	int i;
	bool flag = true;		//���������������ɭ��
	if (s < 0)	flag = false;

	//����������֦
	if (!b_source)
		b_source = new int[node - 1];
	if (!b_target)
		b_target = new int[node - 1];
	if (!b_group)
		b_group = new int[node];
	memset(b_group, 0, sizeof(int) * node);
	if (!b_weight)
		b_weight = new double[node - 1];
	b_count = 0;

	//�����������Ľ�㼯��
	int* nodeSet = new int[node];
	//���ϼ���
	int s_count = 0;

	//��¼��㵽������СȨֵ�ıߵ�Ȩֵ
	double* minWeightToTree = new double[node];
	//��¼��㵽������СȨֵ�ı߹��������ϵĽ��
	int* sourceInTree = new int[node];

	//�����г�ʼ���Ϊs(��s < 0,����Ϊsource[0])
	if (s < 0)	s = source[0];
	nodeSet[s_count++] = s;
	b_group[s] = 1;
	//����s�����Ľ�㵽������СȨֵ��Ϊ��s�����ıߵ�Ȩֵ
	for (i = 0; i < node; i++)
	{
		minWeightToTree[i] = matrix[s][i];
		if (minWeightToTree[i] < DBL_MAX)
			sourceInTree[i] = s;
	}

	int n;					//��¼��ǰ�������ϵĽڵ��е����ľ�����С�ĵ�
	double w;				//��¼��ǰ�������ϵĽڵ��е�������С����
	int g = 1;				//��¼��ǰ�ǵڼ�����С������
	while (s_count < node)	//ѭ�����������н��δ�ӵ�����,��������С��node
	{
		n = -1;
		w = DBL_MAX;
		for (i = 0; i < node; i++)
		{
			if (minWeightToTree[i] > 0 && minWeightToTree[i] < w)
			{
				n = i;
				w = minWeightToTree[i];
			}
		}

		if (n < 0)
		{
			//��ǰ��С�������Ѵﵽ���
			//��s >= 0��ֹͣ
			if (flag)	break;
			//��s < 0��ʼ������һ����С������
			for (n = 0; n < node; n++)
				if (minWeightToTree[n] == DBL_MAX)	break;
			if (n == node)	
				exit(-1);
			//��n��Ϊ��һ����С�������ĵ�һ�����
			nodeSet[s_count++] = n;
			minWeightToTree[n] = 0;
			sourceInTree[n] = n;
			b_group[n] = ++g;
			//�����㵽������С�������
			for (i = 0; i < node; i++)
			{
				if (minWeightToTree[i] > matrix[n][i])
				{
					minWeightToTree[i] = matrix[n][i];
					sourceInTree[i] = n;
				}
			}
			continue;
		}

		//����С�߼���������
		b_source[b_count] = n < sourceInTree[n] ? n : sourceInTree[n];
		b_target[b_count] = n > sourceInTree[n] ? n : sourceInTree[n];
		b_weight[b_count++] = w;
		//����С����Ӧ�Ľ����뼯��
		nodeSet[s_count++] = n;
		//����С����Ӧ�Ľ�㵽���ľ�����Ϊ0�����õ��Ѽӵ�����
		minWeightToTree[n] = 0;
		//����С�ߵ������Ϊ��ǰ��С�������ı��
		b_group[n] = g;

		//�����㵽������С�������
		for (i = 0; i < node; i++)
		{
			if (minWeightToTree[i] > matrix[n][i])
			{
				minWeightToTree[i] = matrix[n][i];
				sourceInTree[i] = n;
			}
		}
	}

	delete []nodeSet;
	delete []minWeightToTree;
	delete []sourceInTree;
}

//������С������
bool Graph::exportMinSpanningTree()
{
	ofstream fileOutput(outFile);
	if (!fileOutput)
		return false;

	char* buf = new char[node * 90];
	memset(buf, 0, sizeof(char) * node * 90);
	char* ptr = buf;

	ptr += sprintf(ptr, "{\n\"nodes\":[\n");
	for (int i = 0; i < node - 1; i++)
		ptr += sprintf(ptr, "\t{\"name\":\"%d\", \"group\":\"%d\"},\n",
		i, b_group[i]);
	ptr += sprintf(ptr, "\t{\"name\":\"%d\", \"group\":\"%d\"}\n\t],\n",
		node - 1, b_group[node - 1]);

	ptr += sprintf(ptr, "\t\"links\":[\n");
	for (int i = 0; i < b_count - 1; i++)
		ptr += sprintf(ptr, "\t{\"source\":%d, \"target\":%d, \"value\":%lf},\n",
		b_source[i], b_target[i], b_weight[i]);
	if (b_count == 0)
		ptr += sprintf(ptr, "\t]\n}\n");
	else
		ptr += sprintf(ptr, "\t{\"source\":%d, \"target\":%d, \"value\":%lf}\n\t]\n}\n",
		b_source[b_count - 1], b_target[b_count - 1], b_weight[b_count - 1]);

	fileOutput.write(buf, strlen(buf));
	fileOutput.close();
	delete []buf;
	return true;
}

//�����Ż���Floyd-Warshall�㷨�����·��
void Graph::calShortestPathByFloyd()
{
	//��ʼ��
	minPath = new double*[node];
	minPathCount = new int*[node];
	SepcificPath = new int*[node];

	for (int i = 0; i < node; i++)
	{
		minPath[i] = new double[node];
		minPathCount[i] = new int[node];
		SepcificPath[i] = new int[node];

		for (int j = 0; j < node; j++)
		{
			minPath[i][j] = DBL_MAX;
			minPathCount[i][j] = 0;
			SepcificPath[i][j] = -1;
		}
		minPath[i][i] = 0;
		SepcificPath[i][i] = i;
	}

	//��ֵ
	for (int i = 0; i < edge; i++)
	{
		if (weight[i] < minPath[source[i]][target[i]])
		{
			minPath[source[i]][target[i]] = weight[i];
			minPath[target[i]][source[i]] = weight[i];
			minPathCount[source[i]][target[i]]++;
			minPathCount[target[i]][source[i]]++;
			SepcificPath[source[i]][target[i]] = target[i];
			SepcificPath[target[i]][source[i]] = source[i];
		}
	}

	//�Ż���Floyd-Warshall�㷨
	double ki;
	int kic;
	for (int k = 0; k < node; k++)
	{
		for (int i = 0; i < node; i++)
		{
			if (minPath[i][k] == DBL_MAX || i == k)	continue;	//���������ܵı�
			ki = minPath[i][k];				//���ʹ�õ�ֵ���б��棬�ɼ��ٷ�������Ĵ���
			kic = minPathCount[i][k];
			for (int j = 0; j < i; j++)		//��������ĶԳ��ԣ�������һ����������
			{
				if (ki + minPath[k][j] == minPath[i][j])
				{
					minPathCount[i][j] += kic * minPathCount[k][j];
					minPathCount[j][i] = minPathCount[i][j];
				}
				else if (minPath[k][j] < DBL_MAX && ki + minPath[k][j] < minPath[i][j])
				{
					minPath[i][j] = ki + minPath[k][j];
					minPath[j][i] = minPath[i][j];
					minPathCount[i][j] = kic * minPathCount[k][j];
					minPathCount[j][i] = minPathCount[i][j];
					SepcificPath[i][j] = SepcificPath[i][k];
					SepcificPath[j][i] = SepcificPath[j][k];
				}
			}
		}
	}
}

//����ӽ����Ķ�
void Graph::calClosenessCentrality()
{
	//��ʼ��
	ClosenessCentrality = new double[node];
	memset(ClosenessCentrality, 0, sizeof(double) * node);

	//����ӽ����Ķ�
	for (int i = 0; i < node; i++)
		for (int j = 0; j < node; j++)
			if (j != i && minPath[i][j] != DBL_MAX)
				ClosenessCentrality[i] += pow(2.0 , -minPath[i][j]);

	//��һ��
	double Min = DBL_MAX, Max = 0;
	for (int i = 0; i < node; i++)
	{
		if (ClosenessCentrality[i] < Min) Min = ClosenessCentrality[i];
		if (ClosenessCentrality[i] > Max) Max = ClosenessCentrality[i];
	}
	double delta = Max - Min;
	for (int i = 0; i < node; i++)
		ClosenessCentrality[i] = (ClosenessCentrality[i] - Min) /delta;
}

//����������Ķ�
void Graph::calBetweennessCentrality()
{
	//��ʼ��
	BetweennessCentrality = new double[node];
	memset(BetweennessCentrality, 0, sizeof(double) * node);

	//����������Ķ�
	int ki;
	for (int k = 0; k < node; k++)
	{
		for (int i = 0; i < node; i++)
		{
			if (i == k || minPathCount[i][k] == 0)	continue;	//���������ܵı�
			ki = minPathCount[i][k];		//���ʹ�õ�ֵ���б��棬�ɼ��ٷ�������Ĵ���
			for (int j = 0; j < i; j++)		//��������ĶԳ��Լ�����һ����������
			{
				if (minPathCount[i][j] == 0)	continue;
				if (minPath[i][k] + minPath[k][j] == minPath[i][j])
					BetweennessCentrality[k] += (2*((double)(minPathCount[i][k] * minPathCount[k][j])
					/ minPathCount[i][j]));
			}
		}
	}

	//��һ��
	double Min = DBL_MAX, Max = 0;
	for (int i = 0; i < node; i++)
	{
		if (BetweennessCentrality[i] < Min) Min = BetweennessCentrality[i];
		if (BetweennessCentrality[i] > Max) Max = BetweennessCentrality[i];
	}
	double delta = Max - Min;
	for (int i = 0; i < node; i++)
		BetweennessCentrality[i] = (BetweennessCentrality[i] - Min) / delta;
}

//����������Ķ�����
bool Graph::exportCentralityToTXT()
{
	ofstream fileOutput(outFile);
	if (!fileOutput)
		return false;

	char* buf = new char[node * 35];
	memset(buf, 0, sizeof(char) * node * 35);
	char* ptr = buf;

	ptr += sprintf(ptr, "%6s\t%10s\t%10s\n", "�����", "�ӽ����Ķ�", "�������Ķ�");
	for (int i = 0; i < node; i++)
		ptr += sprintf(ptr, "%6d\t%10.6lf\t%10.6lf\n", i, ClosenessCentrality[i], BetweennessCentrality[i]);
	fileOutput.write(buf, strlen(buf));
	return true;
}

bool Graph::exportBCentralityToJSON()
{
	generateMinSpanningTree();
	for(int i = 0; i < node; i++)
		b_group[i] = (int)(BetweennessCentrality[i] * 19);
	return exportMinSpanningTree();
}

bool Graph::exportCCentralityToJSON()
{
	generateMinSpanningTree();
	for(int i = 0; i < node; i++)
		b_group[i] = (int)(ClosenessCentrality[i] * 19);
	return exportMinSpanningTree();
}


//���絼��API
void Graph::printGraphInJSON(char* outputfile)
{
	setOutFile(outputfile);
	//����json�ļ�������ǰ��
	exportGraph();
}

//��С�����������뵼��API��������json�ļ��еı���С����������֦
//nΪ��ʼ�㣬��n < 0����������С����ɭ��
void Graph::printMinSpanningTree(char* outputfile, int n)
{
	setOutFile(outputfile);
	//������С������
	generateMinSpanningTree(n);
	//��С�������ı�����
	sortGraph(b_source, b_target, b_weight, 0, b_count - 1);
	//������С��������json�ļ�
	exportMinSpanningTree();
}

//���ӽ����ĶȺͽ������Ķȵ���API
void Graph::printCentralityToTXT(char* outputfile)
{
	setOutFile(outputfile);
	//�������·������
	if (!minPath)	//��ֹ�ظ�����
		calShortestPathByFloyd();
	//����ӽ����Ķ�
	if (!ClosenessCentrality)
		calClosenessCentrality();
	//����������Ķ�
	if (!BetweennessCentrality)
		calBetweennessCentrality();
	//����������Ķ�����
	exportCentralityToTXT();
}

void Graph::printBCentralityToJSON(char* outputfile)
{
	setOutFile(outputfile);
	//�������·������
	if (!minPath)	//��ֹ�ظ�����
		calShortestPathByFloyd();
	//����������Ķ�
	if (!BetweennessCentrality)
		calBetweennessCentrality();
	//����������Ķ�����
	exportBCentralityToJSON();
}

void Graph::printCCentralityToJSON(char* outputfile)
{
	setOutFile(outputfile);
	//�������·������
	if (!minPath)	//��ֹ�ظ�����
		calShortestPathByFloyd();
	//����ӽ����Ķ�
	if (!ClosenessCentrality)
		calClosenessCentrality();
	//����������Ķ�����
	exportCCentralityToJSON();
}

//�������������·��API,������json�ļ��еı߼�Ϊs��e�����·��
bool Graph::printShortestPath(char* outputfile, int s, int e)
{
	setOutFile(outputfile);
	ofstream fileOutput(outFile);

	//�������·������
	if (!minPath)	//��ֹ�ظ�����
		calShortestPathByFloyd();

	int* group = new int[node];
	memset(group, 0, sizeof(int) * node);

	if (SepcificPath[s][e] < 0)	return false;

	int k = s;
	group[k] = 1;
	while (k != e)
	{
		k = SepcificPath[k][e];
		group[k] = 1;
	}

	char* buf = new char[node * 90];
	memset(buf, 0, sizeof(char) * (node * 90));
	char* ptr = buf;

	ptr += sprintf(ptr, "{\n\"nodes\":[\n");
	for (int i = 0; i < node - 1; i++)
		ptr += sprintf(ptr, "\t{\"name\":\"%d\", \"group\":\"%d\"},\n",
		i, group[i]);
	ptr += sprintf(ptr, "\t{\"name\":\"%d\", \"group\":\"%d\"}\n\t],\n",
		node - 1, group[node - 1]);

	ptr += sprintf(ptr, "\t\"links\":[\n");
	if (minPath[s][e] == DBL_MAX || s == e)
		ptr += sprintf(ptr, "\t]\n}\n");
	else
	{
		k = s;
		while (SepcificPath[k][e] != e)
		{
			ptr += sprintf(ptr, "\t{\"source\":%d, \"target\":%d, \"value\":%lf},\n",
				k, SepcificPath[k][e], minPath[k][SepcificPath[k][e]]);
			k = SepcificPath[k][e];
		}
		ptr += sprintf(ptr, "\t{\"source\":%d, \"target\":%d, \"value\":%lf}\n\t]\n}\n",
			k, SepcificPath[k][e], minPath[k][SepcificPath[k][e]]);
	}
	fileOutput.write(buf, strlen(buf));
	fileOutput.close();

	delete []buf;
	delete []group;

	return true;
}