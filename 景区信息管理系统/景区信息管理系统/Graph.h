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
	int num;//����ı��
	char name[StrMAX];//���������
	char desc[StrMAX];//����Ľ���
};
struct Edge {
	int vex1;//�ߵĵ�һ������
	int vex2;//�ߵĵڶ�������
	int weight;//�ߵ�Ȩֵ
	friend bool operator < (Edge a, Edge b)
	{
		return a.weight > b.weight;    //�ṹ���У�xС�����ȼ���     
	}
};
typedef struct Path {
	int Vexs[numMAX];//·���Ķ��㼯��
	Path *next;//·����ָ��
}*PathList;
class Graph {
private:
	int AdjMatrix[numMAX][numMAX];//�ڽӾ���
	Vex Vexs[numMAX];//������Ϣ����
	int VexNum=0;//��ǰͼ�Ķ������
	int EdgeNum=0;//��ǰ�ߵĸ���
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
	int** GetAdjMatrix() {//�����Ա���ж�ά����ʱ�ø÷���ʵ�ַ��ض�ά����
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
	void SetVexNum(int);//���õ�ǰͼ�Ķ������
	int GetVexNum(void);//���ص�ǰͼ�Ķ������
	int GetEdgeNum(void);
	int getFirstNeighbor(int);
	int getNextNeighbor(int v, int w);
	bool InsertEdge(Edge sedge);//�����
	Edge GetEdge(int vex1, int vex2);//�����������ı�
	int FindAdjSpots(int index, Edge *aEdge);//��ѯ������ָ�����������ı�
	void DFSTraverse(int nVex, PathList &pList);//�������
	void DFSTraverse2(int nVex, PathList &pList);//�������
	int FindShortPath(int nVexStart, int nVexEnd, int aPath[], int &number);//�ҵ����·��
	int FindMinTree(Edge aPath[]);//�ҵ���С������
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
		return false;//��������
	}
	Vexs[VexNum++] = sVex;
		return true;
}
bool Graph::InsertEdge(Edge sedge) {
	if (sedge.vex1<0||sedge.vex1>=VexNum||sedge.vex2<0||sedge.vex2>=VexNum) {
		//�±�Խ��
		return false;
	}
	//����ߵ���Ϣ
//	AdjMatrix[sedge.vex1][sedge.vex1] = 0;
	AdjMatrix[sedge.vex1][sedge.vex2] = sedge.weight;//�ڽӾ���ֵ
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
int Graph::FindAdjSpots(int index, Edge *aEdge) {//��ѯ������ָ�����������ı�
	int k = 0;
		for (int i = 0; i < VexNum; i++) {
			if (AdjMatrix[index][i]!=INT_MAX&&i!=index) {
			aEdge[k].weight = AdjMatrix[index][i];
			aEdge[k].vex1 = index;
			aEdge[k].vex2 = i;
			k++;
		}
	}
	return k;//���رߵ�����
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
int Graph::FindShortPath(int nVexStart, int nVexEnd, int aPath[], int &number) {//��̬�滮��������̬�滮����
	//aPath����󵽵����·��
	bool *S = new bool[VexNum];//���·�����㼯  �����������·�����յ�
	int dist[numMAX];
	for (int i = 0; i < VexNum; i++) {//�����ʼ��
		dist[i] = AdjMatrix[nVexStart][i];//����ʼ��nVexStart������i�����·������ ��ʼ��
		S[i] = false;
		if (i != nVexStart && dist[i] < INT_MAX) {//�������ʼ�㵽����i��·���� ��ʼ��ŵ�������i·���е���ʼ��
			aPath[i] = nVexStart;
		}
	 }

	S[nVexStart] = true;//��ʼ��nVexStart���붥�㼯��
	dist[nVexStart] = 0;
	int min = INT_MAX;
	for (int i = 0; i < VexNum - 1; i++) {
		min = INT_MAX;
		int u = nVexStart; //ѡ����S�о������·���Ķ���u
		for (int j = 0; j < VexNum; j++) {
			if (S[j] == false && dist[j] < min) {//�����û���ҵ�����ʼ�㵽����j�����·������
				u = j;
				min = dist[j];
			}
		}
			S[u] = true; //������u���뼯��S
			for (int k = 0; k < VexNum; k++) {//�޸�   �����ƹ�����u������ʼ�㵽����u�ﵽ���·����������������·�����ȣ����������޸�
				//��̬�滮����
				int w = AdjMatrix[u][k];
				if (S[k] == false && w < INT_MAX&&dist[u] + w < dist[k]) {
					//����kδ����S�����ƹ�u��������·��
					dist[k] = dist[u] + w;
					aPath[k] = u;//�޸ĵ�k�����·��
				}
		    }
	}
	return dist[nVexEnd];
}
int Graph::FindMinTree(Edge aPath[]) {//Prim�㷨 ̰�Ĳ���
	int u = 0;//Ĭ�ϴ�A��ʼ����������С������
	
	bool *Vmst = new bool[VexNum]; //��С���������㼯��
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
	//���������������  һ��·��

	bVisited[nVex] = true;
	pList->Vexs[nIndex] = nVex;
	nIndex++;
		for (int i = 0; i < VexNum; i++) {//����v�������ڽӵ�
			//����д��̫û�ж�������
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
//index  ������������Ϊ�˼�¼·����·���ǲ���������
void Graph::DFS2(int nVex, bool bVisited[], int &nIndex, PathList &pList) {
	//���������������   ����·��
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
			pathList->Vexs[i] = pList->Vexs[i];//������г�ʼ����Ϊʲô������
		}
		pList =pathList ;
	}
	else {
		for (int i = 0; i < VexNum; i++) {//����v�������ڽӵ�
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