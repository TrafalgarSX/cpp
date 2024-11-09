#include<stdio.h>
#include"function.h"

int main() {

	

	
	int num;
	int nSelection = -1;
	char rubbish[1000];
	
	printf("\t\t★欢迎进入职工信息管理系统★\n");
	printf("\t\t-----------职工信息系统菜单------------\n");
	
	do {
		printf("1.添加职工信息\n");
		printf("2.查询职工信息\n");
		printf("3.修改职工信息\n");
		printf("4.职工工资统计\n");
		printf("5.工资级别\n");
		printf("6.工资税收\n");
		printf("7.工资排序\n");
		printf("0.退出\n");
		printf("请选择菜单项编号（0~7）：");
	
		nSelection = -1;
		num = scanf("%d", &nSelection);
		while (num == 0)
		{
			scanf("%s", rubbish);
			printf("\n\n请选择正确的菜单编号:\n\n ");
			printf("1.添加职工信息\n");
			printf("2.查询职工信息\n");
			printf("3.修改职工信息\n");
			printf("4.职工工资统计\n");
			printf("5.工资级别\n");
			printf("6.工资税收\n");
			printf("7.工资排序\n");
			printf("0.退出\n");
			printf("请选择菜单项编号（0~7）：");

			num = scanf("%d", &nSelection);


		}
		switch (nSelection) {
		case 1: {
			printf("------添加职工信息-------\n");
			add();
			break;
		}
		case 2: {
			printf( "------查询职工信息-------\n" );
			displayCard(head);
			break;
		}
		case 3: {
			printf("------修改职工信息-------\n");
			change();
			break;
		}
		case 4: {
			printf( "------职工工资统计-------\n" );
			statictics();
			break;
		}
		case 5: {
			printf("------工资级别相同的人-------\n");
			Moneydegree();
			break;
		}
		case 6: {
		printf( "------工资税收-------\n" );
		tax();
			break;
		}
		case 7: {
			printf("-------工资排序------------\n");
			sort();
			break;
		}
		case 0:
		{
			
			printf( "谢谢您使用本系统");
			exit(0);
		}
		default:
		{
			printf( "输入的菜单编号错误！\n" );
			break;
		}
		}
	} while (nSelection != 0);
}