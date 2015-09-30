/*
* 文件名:Graph.h
* 文件说明:图分析类，封装了图的计算最小生成树和接近、介数中心度的操作
*
* 作者:刘桐彤,日期:2014年01月
* Copyright (c) 2014年 刘桐彤. All rights reserved.
*/

#ifndef GRAPH_H
#define GRAPH_H

class Graph
{
public:
	//导入导出的文件名
	char inFile[256];
	char outFile[256];

	int node;
	int edge;
	//利用边列表存储
	int* source;
	int* target;
	double* weight;
	//邻接矩阵
	double** matrix;

	//最小生成树的存储
	int b_count;
	int* b_source;
	int* b_target;
	int* b_group;
	double* b_weight;

	//最短路径长度矩阵
	double** minPath;
	//最短路径路径矩阵
	int** SepcificPath;
	//最短路径个数矩阵
	int** minPathCount;
	//中心度
	double* ClosenessCentrality;
	double* BetweennessCentrality;

public:
	Graph();
	~Graph();
	void destroy();

	//设置导入导出的JSON文件名
	void setInFile(char* fileName);
	void setOutFile(char* fileName);

	//图的导入导出
	//导入：txt格式
	bool importGraph();
	//排序
	static void sortGraph(int* s, int* t, double* w, int start, int end);
	static void qSortEdge(int* s, int* t, double* w, int start, int end);
	//导出：json格式
	bool exportGraph();

	//生成最小生成树or森林（Prim算法）
	//n为起始点，若n < 0，则生成最小生成森林
	void generateMinSpanningTree(int s = -1);
	//导出最小生成s
	bool exportMinSpanningTree();

	//利用优化的Floyd-warshall算法求最短路径
	void calShortestPathByFloyd();
	//计算接近中心度
	void calClosenessCentrality();
	//计算介数中心度
	void calBetweennessCentrality();
	//导出结点中心度数据
	bool exportCentralityToTXT();
	bool exportBCentralityToJSON();
	bool exportCCentralityToJSON();
	//网络导出API
	void printGraphInJSON(char* outputfile);
	//最小生成树生成与导出API，导出的json文件中的边最小生成树的树枝
	//n为起始点，若n < 0，则生成最小生成森林
	void printMinSpanningTree(char* outputfile, int n = -1);
	//结点接近中心度和介数中心度导出到TXT的API
	void printCentralityToTXT(char* outputfile);
	//结点接近中心度和介数中心度导出到JSON的API
	void printBCentralityToJSON(char* outputfile);
	void printCCentralityToJSON(char* outputfile);
	//导出两点间的最短路径API,导出的json文件中的边即为s到e的最短路径
	bool printShortestPath(char* outputfile, int s, int e);
};

#endif