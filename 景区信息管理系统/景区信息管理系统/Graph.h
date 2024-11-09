#pragma once
#include<iostream>
#include<limits.h>
#include<algorithm>
#include<queue>
#define StrMAX 20
#define numMAX  20
#define MAX_VERTEX_NUM 20
const int MAX = INT_MAX;
using namespace std;
struct Vex {
	int num;//景点的编号
	char name[StrMAX];//景点的名字
	char desc[StrMAX];//景点的介绍
};
struct Edge {
	int vex1;//边的第一个顶点
	int vex2;//边的第二个顶点
	int weight;//边的权值
	friend bool operator < (Edge a, Edge b)
	{
		return a.weight > b.weight;    //结构体中，x小的优先级高     
	}
};
typedef struct Path {
	int Vexs[numMAX];//路径的顶点集合
	Path *next;//路径的指针
}*PathList;
class Graph {
private:
	int AdjMatrix[numMAX][numMAX];//邻接矩阵
	Vex Vexs[numMAX];//顶点信息数组
	int VexNum=0;//当前图的顶点个数
	int EdgeNum=0;//当前边的个数
public:
	Graph(int max =MAX) {
		for (int i = 0; i < numMAX; i++) {
			for (int j = 0; j < numMAX; j++) {
				AdjMatrix[i][j] = (i == j) ? 0 : INT_MAX;
			}
		}
			Vexs[numMAX] = { 0,0,0 };
			EdgeNum = 0;
			VexNum = 0;
	};
	~Graph();
	int** GetAdjMatrix() {//当类成员中有二维数组时用该方法实现返回二维数组
		int **b = new int*[numMAX];
		for (int i= 0; i < numMAX; i++) {
			b[i] = new int[numMAX];
		}
		for (int i = 0; i < numMAX; i++) {
			for (int j = 0; j < numMAX; j++) {
				b[i][j] = AdjMatrix[i][j];
			}
		}
		return b;
	}
	bool InsertVex(Vex svex);
	Vex GetVex(int index);
	void SetVexNum(int);//设置当前图的顶点个数
	int GetVexNum(void);//返回当前图的顶点个数
	int GetEdgeNum(void);
	int getFirstNeighbor(int);
	int getNextNeighbor(int v, int w);
	bool InsertEdge(Edge sedge);//插入边
	Edge GetEdge(int vex1, int vex2);//获得两个顶点的边
	int FindAdjSpots(int index, Edge *aEdge);//查询所有与指定顶点相连的边
	void DFSTraverse(int nVex, PathList &pList);//深度搜索
	void DFSTraverse2(int nVex, PathList &pList);//深度搜索
	int FindShortPath(int nVexStart, int nVexEnd, int aPath[], int &number);//找到最短路径
	int FindMinTree(Edge aPath[]);//找到最小生成树
private:
	void DFS(int nVex, bool bVisted[], int &nIndex, PathList &pList);
	void DFS2(int nVex, bool bVisted[], int &nIndex, PathList &pList);

};

Graph::~Graph() {
	delete AdjMatrix;
	delete Vexs;
}
bool Graph::InsertVex(Vex sVex) {
	if (VexNum == MAX_VERTEX_NUM) {
		return false;//顶点已满
	}
	Vexs[VexNum++] = sVex;
		return true;
}
bool Graph::InsertEdge(Edge sedge) {
	if (sedge.vex1<0||sedge.vex1>=VexNum||sedge.vex2<0||sedge.vex2>=VexNum) {
		//下标越界
		return false;
	}
	//插入边的信息
//	AdjMatrix[sedge.vex1][sedge.vex1] = 0;
	AdjMatrix[sedge.vex1][sedge.vex2] = sedge.weight;//邻接矩阵赋值
	AdjMatrix[sedge.vex2][sedge.vex1] = sedge.weight;
	EdgeNum++;
	return true;
}
Vex Graph::GetVex(int v) {
	return Vexs[v];
}
void Graph::SetVexNum(int v) {
	VexNum = v;
}
int Graph::GetVexNum(void) {
	return VexNum;
}
int Graph::GetEdgeNum(void) {
	return EdgeNum;
}
Edge Graph::GetEdge(int vex1, int vex2) {
	Edge edge;
	edge.vex1 = vex1;
	edge.vex2 = vex2;
	edge.weight = AdjMatrix[vex1][vex2];
	return   edge;
}
int Graph::getFirstNeighbor(int v) {
	if (v != -1) {
		for(int i=0;i<VexNum;i++){
			if (AdjMatrix[v][i] != INT_MAX && i != v) {
				return i;
			}
		}
	}
	return -1;
}
int Graph::getNextNeighbor(int v, int w) {
	if (v != -1) {
		for (int i = w+1; i < VexNum; i++) {
			if (AdjMatrix[v][i] != INT_MAX && i != v) {
				return i;
			}
		}
	}
	return -1;
}
int Graph::FindAdjSpots(int index, Edge *aEdge) {//查询所有与指定顶点相连的边
	int k = 0;
		for (int i = 0; i < VexNum; i++) {
			if (AdjMatrix[index][i]!=INT_MAX&&i!=index) {
			aEdge[k].weight = AdjMatrix[index][i];
			aEdge[k].vex1 = index;
			aEdge[k].vex2 = i;
			k++;
		}
	}
	return k;//返回边的条数
}

void Graph::DFSTraverse(int nVex, PathList &pList) {
	int nIndex = 0;
	bool bVisited[numMAX] = { false };
	DFS(nVex, bVisited, nIndex, pList);
}
void Graph::DFSTraverse2(int nVex, PathList &pList) {
	int nIndex = 0;
	bool bVisited[numMAX] = { false };
	DFS2(nVex, bVisited, nIndex, pList);

}
int Graph::FindShortPath(int nVexStart, int nVexEnd, int aPath[], int &number) {//动态规划？？？动态规划函数
	//aPath存放求到的最短路径
	bool *S = new bool[VexNum];//最短路径顶点集  存放已求出最短路径的终点
	int dist[numMAX];
	for (int i = 0; i < VexNum; i++) {//数组初始化
		dist[i] = AdjMatrix[nVexStart][i];//从起始点nVexStart到顶点i的最短路径长度 初始化
		S[i] = false;
		if (i != nVexStart && dist[i] < INT_MAX) {//如果从起始点到顶点i有路径则将 起始点放到到顶点i路径中的起始点
			aPath[i] = nVexStart;
		}
	 }

	S[nVexStart] = true;//起始点nVexStart加入顶点集合
	dist[nVexStart] = 0;
	int min = INT_MAX;
	for (int i = 0; i < VexNum - 1; i++) {
		min = INT_MAX;
		int u = nVexStart; //选不在S中具有最短路径的顶点u
		for (int j = 0; j < VexNum; j++) {
			if (S[j] == false && dist[j] < min) {//如果还没有找到从起始点到顶点j的最短路径而且
				u = j;
				min = dist[j];
			}
		}
			S[u] = true; //将顶点u加入集合S
			for (int k = 0; k < VexNum; k++) {//修改   尝试绕过顶点u（从起始点到顶点u达到最短路径）到达各个顶点的路径长度，若缩短则修改
				//动态规划函数
				int w = AdjMatrix[u][k];
				if (S[k] == false && w < INT_MAX&&dist[u] + w < dist[k]) {
					//顶点k未加入S，且绕过u可以缩短路径
					dist[k] = dist[u] + w;
					aPath[k] = u;//修改到k的最短路径
				}
		    }
	}
	return dist[nVexEnd];
}
int Graph::FindMinTree(Edge aPath[]) {//Prim算法 贪心策略
	int u = 0;//默认从A开始遍历生成最小生成树
	
	bool *Vmst = new bool[VexNum]; //最小生成树顶点集合
	int v,min=INT_MAX,count=0,sum=0;
	for (int i = 0; i < VexNum; i++) {
		Vmst[i] = false;
	}
	Vmst[u] = true;
	priority_queue<Edge> queEdge;
	do {
		v = getFirstNeighbor(u);
		Edge edge ;
		edge.vex1 = edge.vex2 = edge.weight = INT_MAX;
		while (v!=-1) {
			if (Vmst[v] != true) {
				edge = GetEdge(u, v);
				queEdge.push(edge);
			}
			v = getNextNeighbor(u, v);
		}
		while (queEdge.empty() == false && count < VexNum-1) {
			edge = queEdge.top();
			queEdge.pop();
			if (Vmst[edge.vex2] == false) {
				aPath[count] = edge;
				u = edge.vex2;
				Vmst[u] = true;
				sum += edge.weight;
				count++;
				break;
			}
	}
	} while (count < VexNum-1);
	return sum;
}
void Graph::DFS(int nVex, bool bVisited[], int &nIndex, PathList &pList) {
	//深度优先搜索遍历  一条路径

	bVisited[nVex] = true;
	pList->Vexs[nIndex] = nVex;
	nIndex++;
		for (int i = 0; i < VexNum; i++) {//搜索v的所有邻接点
			//这种写法太没有动脑子了
			if ((AdjMatrix[nVex][i] != INT_MAX&&i!=nVex) && !bVisited[i]) {
				DFS(i, bVisited, nIndex, pList);
				if (nIndex == VexNum) {
					return;
				}
				bVisited[i] = false;
				nIndex--;
			}
		}
}
//index  是引用类型是为了记录路径，路径是不断增长的
void Graph::DFS2(int nVex, bool bVisited[], int &nIndex, PathList &pList) {
	//深度优先搜索遍历   多条路径
	bVisited[nVex] = true;
	pList->Vexs[nIndex++] = nVex;
	if (nIndex == VexNum) {
		/*PathList pathList = (PathList)malloc(sizeof(Path));
		pathList->next = NULL;
		pList->next = pathList;
		for (int i = 0; i < VexNum; i++) {
			pathList->Vexs[i] = 0;
		}*/
		PathList pathList = new Path;
		pathList->next = NULL;
		pList->next = pathList;
		for (int i = 0; i < VexNum; i++) {
			pathList->Vexs[i] = pList->Vexs[i];//必须进行初始化。为什么？？？
		}
		pList =pathList ;
	}
	else {
		for (int i = 0; i < VexNum; i++) {//搜索v的所有邻接点
			if ((AdjMatrix[nVex][i] != INT_MAX&&i!=nVex) && !bVisited[i]) {
				DFS2(i, bVisited, nIndex, pList);

				bVisited[i] = false;
				pList->Vexs[nIndex] = -1;
				nIndex--;
			}
		}
	}
}
////bool alltrue;
//for (int i = 0; i < VexNum; i++) {
//	if (bVisited[i] == false) {
//		alltrue = false;
//		break;
//	}
//	else {
//		alltrue = true;
//	}
//}
//if (alltrue) {
//	return;
//}