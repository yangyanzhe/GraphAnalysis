/*
* 文件名:Graph.cpp
* 文件说明:图分析类，封装了图的计算最小生成树和接近、介数中心度的操作
*
* 作者:刘桐彤,日期:2014年01月
* Copyright (c) 2014年 刘桐彤. All rights reserved.
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

//设置导入导出的文件名
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

//图的导入导出
//导入：txt格式
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

//排序
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

//导出：json格式
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

//生成最小生成树（Prim算法）
//n为起始点，若n < 0，则生成最小生成森林
void Graph::generateMinSpanningTree(int s)
{
	int i;
	bool flag = true;		//标记是生成树还是森林
	if (s < 0)	flag = false;

	//生成树的树枝
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

	//生成树关联的结点集合
	int* nodeSet = new int[node];
	//集合计数
	int s_count = 0;

	//记录结点到树的最小权值的边的权值
	double* minWeightToTree = new double[node];
	//记录结点到树的最小权值的边关联的树上的结点
	int* sourceInTree = new int[node];

	//集合中初始结点为s(若s < 0,则设为source[0])
	if (s < 0)	s = source[0];
	nodeSet[s_count++] = s;
	b_group[s] = 1;
	//将与s关联的结点到树的最小权值设为与s相连的边的权值
	for (i = 0; i < node; i++)
	{
		minWeightToTree[i] = matrix[s][i];
		if (minWeightToTree[i] < DBL_MAX)
			sourceInTree[i] = s;
	}

	int n;					//记录当前不在树上的节点中到树的距离最小的点
	double w;				//记录当前不在树上的节点中到树的最小距离
	int g = 1;				//记录当前是第几棵最小生成树
	while (s_count < node)	//循环条件：仍有结点未加到树上,即结点个数小于node
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
			//当前最小生成树已达到最大
			//若s >= 0则停止
			if (flag)	break;
			//若s < 0开始生成下一棵最小生成树
			for (n = 0; n < node; n++)
				if (minWeightToTree[n] == DBL_MAX)	break;
			if (n == node)	
				exit(-1);
			//将n作为下一棵最小生成树的第一个结点
			nodeSet[s_count++] = n;
			minWeightToTree[n] = 0;
			sourceInTree[n] = n;
			b_group[n] = ++g;
			//将各点到树的最小距离更新
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

		//把最小边加入生成树
		b_source[b_count] = n < sourceInTree[n] ? n : sourceInTree[n];
		b_target[b_count] = n > sourceInTree[n] ? n : sourceInTree[n];
		b_weight[b_count++] = w;
		//把最小边相应的结点加入集合
		nodeSet[s_count++] = n;
		//将最小边相应的结点到树的距离设为0，即该点已加到树上
		minWeightToTree[n] = 0;
		//将最小边的组号设为当前最小生成树的编号
		b_group[n] = g;

		//将各点到树的最小距离更新
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

//导出最小生成树
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

//利用优化的Floyd-Warshall算法求最短路径
void Graph::calShortestPathByFloyd()
{
	//初始化
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

	//赋值
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

	//优化的Floyd-Warshall算法
	double ki;
	int kic;
	for (int k = 0; k < node; k++)
	{
		for (int i = 0; i < node; i++)
		{
			if (minPath[i][k] == DBL_MAX || i == k)	continue;	//跳过不可能的边
			ki = minPath[i][k];				//多次使用的值进行保存，可减少访问数组的次数
			kic = minPathCount[i][k];
			for (int j = 0; j < i; j++)		//利用数组的对称性，减少了一半的运算次数
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

//计算接近中心度
void Graph::calClosenessCentrality()
{
	//初始化
	ClosenessCentrality = new double[node];
	memset(ClosenessCentrality, 0, sizeof(double) * node);

	//计算接近中心度
	for (int i = 0; i < node; i++)
		for (int j = 0; j < node; j++)
			if (j != i && minPath[i][j] != DBL_MAX)
				ClosenessCentrality[i] += pow(2.0 , -minPath[i][j]);

	//归一化
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

//计算介数中心度
void Graph::calBetweennessCentrality()
{
	//初始化
	BetweennessCentrality = new double[node];
	memset(BetweennessCentrality, 0, sizeof(double) * node);

	//计算介数中心度
	int ki;
	for (int k = 0; k < node; k++)
	{
		for (int i = 0; i < node; i++)
		{
			if (i == k || minPathCount[i][k] == 0)	continue;	//跳过不可能的边
			ki = minPathCount[i][k];		//多次使用的值进行保存，可减少访问数组的次数
			for (int j = 0; j < i; j++)		//利用数组的对称性减少了一半的运算次数
			{
				if (minPathCount[i][j] == 0)	continue;
				if (minPath[i][k] + minPath[k][j] == minPath[i][j])
					BetweennessCentrality[k] += (2*((double)(minPathCount[i][k] * minPathCount[k][j])
					/ minPathCount[i][j]));
			}
		}
	}

	//归一化
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

//导出结点中心度数据
bool Graph::exportCentralityToTXT()
{
	ofstream fileOutput(outFile);
	if (!fileOutput)
		return false;

	char* buf = new char[node * 35];
	memset(buf, 0, sizeof(char) * node * 35);
	char* ptr = buf;

	ptr += sprintf(ptr, "%6s\t%10s\t%10s\n", "点序号", "接近中心度", "介数中心度");
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


//网络导出API
void Graph::printGraphInJSON(char* outputfile)
{
	setOutFile(outputfile);
	//导出json文件（处理前）
	exportGraph();
}

//最小生成树生成与导出API，导出的json文件中的边最小生成树的树枝
//n为起始点，若n < 0，则生成最小生成森林
void Graph::printMinSpanningTree(char* outputfile, int n)
{
	setOutFile(outputfile);
	//计算最小生成树
	generateMinSpanningTree(n);
	//最小生成树的边排序
	sortGraph(b_source, b_target, b_weight, 0, b_count - 1);
	//导出最小生成树的json文件
	exportMinSpanningTree();
}

//结点接近中心度和介数中心度导出API
void Graph::printCentralityToTXT(char* outputfile)
{
	setOutFile(outputfile);
	//生成最短路径矩阵
	if (!minPath)	//防止重复计算
		calShortestPathByFloyd();
	//计算接近中心度
	if (!ClosenessCentrality)
		calClosenessCentrality();
	//计算介数中心度
	if (!BetweennessCentrality)
		calBetweennessCentrality();
	//导出结点中心度数据
	exportCentralityToTXT();
}

void Graph::printBCentralityToJSON(char* outputfile)
{
	setOutFile(outputfile);
	//生成最短路径矩阵
	if (!minPath)	//防止重复计算
		calShortestPathByFloyd();
	//计算介数中心度
	if (!BetweennessCentrality)
		calBetweennessCentrality();
	//导出结点中心度数据
	exportBCentralityToJSON();
}

void Graph::printCCentralityToJSON(char* outputfile)
{
	setOutFile(outputfile);
	//生成最短路径矩阵
	if (!minPath)	//防止重复计算
		calShortestPathByFloyd();
	//计算接近中心度
	if (!ClosenessCentrality)
		calClosenessCentrality();
	//导出结点中心度数据
	exportCCentralityToJSON();
}

//导出两点间的最短路径API,导出的json文件中的边即为s到e的最短路径
bool Graph::printShortestPath(char* outputfile, int s, int e)
{
	setOutFile(outputfile);
	ofstream fileOutput(outFile);

	//生成最短路径矩阵
	if (!minPath)	//防止重复计算
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