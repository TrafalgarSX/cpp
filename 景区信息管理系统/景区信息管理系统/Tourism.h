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
	//����������Ϣ��
	void CreateGraph();
	void ShowGraph();
	//��ѯ������Ϣ
	void GetSpotInfo();
	//���㵼��
	void TravelPaht(int type);
	//�����·��
	void FindShortPath(void);
	//��·�滮
	void DesignPaht(void);
};
Tourism::Tourism() {
	
}
Tourism::~Tourism() {
	graph.~Graph();
}
void Tourism::ShowGraph() {
	cout << "������Ŀ:" << graph.GetVexNum() << endl;
	cout << "-----------����--------------" << endl;
	int vexnum = graph.GetVexNum();
	for (int i = 0; i < vexnum; i++) {
		cout << graph.GetVex(i).num <<"-"<< graph.GetVex(i).name <<endl;
	}
	cout << "----------��-------------" << endl;
	ifstream File;
	File.open("D:/Edge.txt", ios::in);
	while (!File.eof()) {
		int v1, v2, weight;
		File >> v1 >> v2 >> weight;
		cout << "<v" << v1 << "," << "v" << v2 << "> " << weight<<endl;
	}

}
void Tourism::CreateGraph() {//����������Ϣ��
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
		cout << "����Ϣ�洢�ļ���ʧ�ܣ�" << endl;
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
		cout << "�ߴ洢�ļ���ʧ�ܣ�" << endl;
	}
	File.close();
}
void Tourism::GetSpotInfo() {//��ѯ������Ϣ
	cout<< endl; cout << endl;
	int i;
	cout << "��������Ҫ��ѯ�Ķ�����:";
	cin >> i;
	Vex v = graph.GetVex(i);
	cout << v.name <<endl;
	cout << v.desc << endl;
	Edge aEdge[numMAX];
	int num = graph.FindAdjSpots(i, aEdge);
	cout << "---------�ܱ߾���----------" << endl;
	for (i = 0; i < num; i++) {
		Edge e = aEdge[i];
		Vex start = graph.GetVex(e.vex1);
		Vex end = graph.GetVex(e.vex2);
		cout << start.name << "->" << end.name << "   " << e.weight << "��" << endl;
	}
}
//
void Tourism::TravelPaht(int type) {//���㵼��
	cout << endl << endl;
	int i;
//	cout << "=======���ξ��㵼��=======" << endl;
	for (int i = 0; i < graph.GetVexNum(); i++) {
		Vex v = graph.GetVex(i);
		cout << v.num << "-" << v.name<< "--" << v.desc << endl;
	}
	cout << "��������ʼ���ţ�";
	cin >> i;
	if (i<0 || i>graph.GetVexNum()) {
		cout << "���붥���д�!\n";
		return;
	}
	PathList pathList = (PathList)malloc(sizeof(Path));
	pathList->next = NULL;
	PathList pathHead = pathList;
	//����������ȵ���·��
	int num;
	if (type == 1) {
	  graph.DFSTraverse(i, pathList);
	} 
	else {
	  graph.DFSTraverse2(i, pathList);
	}
	//�������·��
	cout << "����·�ߣ�" << endl;
	i = 1;
	pathList = pathHead;
	int VexNum = graph.GetVexNum();
	while (pathList) {
		if (pathList->Vexs[VexNum-1] != -1) {
			cout << "·��" << i << ":";
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
void Tourism::FindShortPath() {//�����·��
	cout << endl << endl;
//	cout << "======�������·��=====" << endl;
	for (int i = 0; i < graph.GetVexNum(); i++) {
		Vex v = graph.GetVex(i);
		cout << v.num << "-" << v.name << "--" << v.desc << endl;
	}
	int startVex, endVex;
	cout << "��������ʼ���ţ�";
	cin >> startVex;
	cout << "��������ֹ���ţ�";
	cin >> endVex;

	int aPath[numMAX] = { INT_MAX };
	int number = 0;
	int shortPath = graph.FindShortPath(startVex, endVex, aPath, number);

	//��ӡ���
	int n = graph.GetVexNum();
	if (shortPath != INT_MAX) {
		cout << "���·��Ϊ��";
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
		cout << "��̾���Ϊ��" << shortPath << endl;
	}
	else {
		cout << "����֮����ͨ·!" << endl;
	}
}
void Tourism::DesignPaht(void) {//��·�滮
	Edge aPath[numMAX];
	int sum= graph.FindMinTree(aPath);

	//������
	cout << endl << endl;
//	cout << "===�����·�滮====" << endl;
	cout << "����������������·" << endl;
	for (int i = 0; i <graph.GetVexNum()-1; i++) {
	   cout << graph.GetVex(aPath[i].vex1).name << "-";
	   cout << graph.GetVex(aPath[i].vex2).name << "   ";
	   cout << aPath[i].weight << "��" << endl;
	}
	cout << "�����·���ܳ���Ϊ��" << sum << "��"<<endl;
}
