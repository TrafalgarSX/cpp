#pragma once
#include"Graph.h"
#include<fstream>
#include<limits.h>
using namespace std;
class Tourism {
private:
	Graph graph;
public:
	Tourism();
	~Tourism();
	//建立景点信息库
	void CreateGraph();
	void ShowGraph();
	//查询景点信息
	void GetSpotInfo();
	//景点导航
	void TravelPaht(int type);
	//找最短路径
	void FindShortPath(void);
	//电路规划
	void DesignPaht(void);
};
Tourism::Tourism() {
	
}
Tourism::~Tourism() {
	graph.~Graph();
}
void Tourism::ShowGraph() {
	cout << "顶点数目:" << graph.GetVexNum() << endl;
	cout << "-----------顶点--------------" << endl;
	int vexnum = graph.GetVexNum();
	for (int i = 0; i < vexnum; i++) {
		cout << graph.GetVex(i).num <<"-"<< graph.GetVex(i).name <<endl;
	}
	cout << "----------边-------------" << endl;
	ifstream File;
	File.open("D:/Edge.txt", ios::in);
	while (!File.eof()) {
		int v1, v2, weight;
		File >> v1 >> v2 >> weight;
		cout << "<v" << v1 << "," << "v" << v2 << "> " << weight<<endl;
	}

}
void Tourism::CreateGraph() {//建立景点信息库
	//init graph
	ifstream File;
	char buffer[100];
	int vexnum;

	//set the graph vex
	File.open("D:/Vex.txt", ios::in);
	if (File) {
		File >> vexnum;
		Vex vex;
		for (int i = 0; i < vexnum; i++) {
			File >> vex.num>> vex.name >> vex.desc;
			graph.InsertVex(vex);
		}
		graph.SetVexNum(vexnum);
	}
	else {
		cout << "点信息存储文件打开失败！" << endl;
	}
	File.close();

	//set the graph edge
	File.open("D:/Edge.txt", ios::in);
	if (File) {
		while (!File.eof()) {
			Edge edge;
			File >> edge.vex1 >> edge.vex2 >> edge.weight;
			graph.InsertEdge(edge);

		}
	}
	else {
		cout << "边存储文件打开失败！" << endl;
	}
	File.close();
}
void Tourism::GetSpotInfo() {//查询景点信息
	cout<< endl; cout << endl;
	int i;
	cout << "请输入想要查询的顶点编号:";
	cin >> i;
	Vex v = graph.GetVex(i);
	cout << v.name <<endl;
	cout << v.desc << endl;
	Edge aEdge[numMAX];
	int num = graph.FindAdjSpots(i, aEdge);
	cout << "---------周边景区----------" << endl;
	for (i = 0; i < num; i++) {
		Edge e = aEdge[i];
		Vex start = graph.GetVex(e.vex1);
		Vex end = graph.GetVex(e.vex2);
		cout << start.name << "->" << end.name << "   " << e.weight << "米" << endl;
	}
}
//
void Tourism::TravelPaht(int type) {//景点导航
	cout << endl << endl;
	int i;
//	cout << "=======旅游景点导航=======" << endl;
	for (int i = 0; i < graph.GetVexNum(); i++) {
		Vex v = graph.GetVex(i);
		cout << v.num << "-" << v.name<< "--" << v.desc << endl;
	}
	cout << "请输入起始点编号：";
	cin >> i;
	if (i<0 || i>graph.GetVexNum()) {
		cout << "输入顶点有错!\n";
		return;
	}
	PathList pathList = (PathList)malloc(sizeof(Path));
	pathList->next = NULL;
	PathList pathHead = pathList;
	//调用深度优先导航路线
	int num;
	if (type == 1) {
	  graph.DFSTraverse(i, pathList);
	} 
	else {
	  graph.DFSTraverse2(i, pathList);
	}
	//输出导航路线
	cout << "导游路线：" << endl;
	i = 1;
	pathList = pathHead;
	int VexNum = graph.GetVexNum();
	while (pathList) {
		if (pathList->Vexs[VexNum-1] != -1) {
			cout << "路线" << i << ":";
			for (int j = 0; j < VexNum; j++) {
				if (j == VexNum - 1) {
					cout << graph.GetVex(pathList->Vexs[j]).name;
				}
				else {
					cout << graph.GetVex(pathList->Vexs[j]).name << "->";
				}
			}
			cout << endl;
		}
			pathList = pathList->next;
			i++;
	}
	cout << endl << endl
		;
	free(pathList);
	pathList = NULL;
}
void Tourism::FindShortPath() {//找最短路径
	cout << endl << endl;
//	cout << "======搜索最短路径=====" << endl;
	for (int i = 0; i < graph.GetVexNum(); i++) {
		Vex v = graph.GetVex(i);
		cout << v.num << "-" << v.name << "--" << v.desc << endl;
	}
	int startVex, endVex;
	cout << "请输入起始点编号：";
	cin >> startVex;
	cout << "请输入终止点编号：";
	cin >> endVex;

	int aPath[numMAX] = { INT_MAX };
	int number = 0;
	int shortPath = graph.FindShortPath(startVex, endVex, aPath, number);

	//打印结果
	int n = graph.GetVexNum();
	if (shortPath != INT_MAX) {
		cout << "最短路线为：";
		int *d = new int[n];
		int j = endVex,k=0;
		while (j != startVex) {
			d[k] = j;
			k++;
			j = aPath[j];
		}
		d[k] = startVex;
		reverse(d, d + k+1);
		for (int i = 0; i <= k; i++) {
			cout << graph.GetVex(d[i]).name;
			if (i!=k) {
				cout << "->";
			}
		}
		cout << endl;
		cout << "最短距离为：" << shortPath << endl;
	}
	else {
		cout << "两点之间无通路!" << endl;
	}
}
void Tourism::DesignPaht(void) {//电路规划
	Edge aPath[numMAX];
	int sum= graph.FindMinTree(aPath);

	//输出结果
	cout << endl << endl;
//	cout << "===铺设电路规划====" << endl;
	cout << "在以下两点间铺设电路" << endl;
	for (int i = 0; i <graph.GetVexNum()-1; i++) {
	   cout << graph.GetVex(aPath[i].vex1).name << "-";
	   cout << graph.GetVex(aPath[i].vex2).name << "   ";
	   cout << aPath[i].weight << "米" << endl;
	}
	cout << "铺设电路的总长度为：" << sum << "米"<<endl;
}
