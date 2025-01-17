// 景区信息管理系统.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"Tourism.h"
#include"Graph.h"
#include<stdio.h>
using namespace std;
int main()
{
	int nSelection;
	int num;
	char rubbish[1000] = {0};
	Tourism tourism;
	nSelection = -1;
	cout << "========景区信息管理系统==============" << endl;
	do {
		cout << "1.创建景区景点图" << endl;
		cout << "2.查询景点信息" << endl;
		cout << "3.旅游景点导航" << endl;
		cout << "4.搜索最短路径" << endl;
		cout << "5.铺设电路规划" << endl;
		cout << "0.退出   " << endl;
		cout << endl;
		cout << "请选择菜单项编号（0~5）：";
		nSelection = -1;
		num = scanf_s("%d", &nSelection);
		while (num == 0)
		{
			scanf("%s", rubbish);
			printf("\n\n请选择正确的菜单编号:\n\n ");
			cout << "1.创建景区景点图" << endl;
			cout << "2.查询景点信息" << endl;
			cout << "3.旅游景点导航" << endl;
			cout << "4.搜索最短路径" << endl;
			cout << "5.铺设电路规划" << endl;
			cout << "0.退出   " << endl;
			cout << endl;
			cout << "请选择菜单项编号（0~5）：";
			num = scanf_s("%d", &nSelection);
		}
		switch (nSelection) {
		case 1: {
			cout << "------创建景区景点图-------" << endl;
			tourism.CreateGraph();
			tourism.ShowGraph();
		
			break;
		}
		case 2: {
			cout << "------查询景点信息-------" << endl;
			tourism.GetSpotInfo();
			break;
		}
		case 3: {
			cout << "------旅游景点导航-------" << endl; 
			int type;
			cout << "提供一条路径输入：1，多条路径输入2；" << endl;
			scanf_s("%d", &type);
			tourism.TravelPaht(type);
			break;
		}
		case 4: {
			cout << "------搜索最短路径-------" << endl;
			tourism.FindShortPath();
			break;
		}
		case 5: {
			cout << "------铺设电路规划-------" << endl;
			tourism.DesignPaht();
			break;
		}
	
		case 0:
		{

			cout << "谢谢您使用本系统！" << endl;
			exit(0);
		}
		default:
		{
			cout << "输入的菜单编号错误！\n" << endl;
			break;
		}
		}
	} while (nSelection != 0);

}
