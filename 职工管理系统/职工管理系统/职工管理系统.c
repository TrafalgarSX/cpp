#include<stdio.h>
#include"function.h"

int main() {

	

	
	int num;
	int nSelection = -1;
	char rubbish[1000];
	
	printf("\t\t�ﻶӭ����ְ����Ϣ����ϵͳ��\n");
	printf("\t\t-----------ְ����Ϣϵͳ�˵�------------\n");
	
	do {
		printf("1.���ְ����Ϣ\n");
		printf("2.��ѯְ����Ϣ\n");
		printf("3.�޸�ְ����Ϣ\n");
		printf("4.ְ������ͳ��\n");
		printf("5.���ʼ���\n");
		printf("6.����˰��\n");
		printf("7.��������\n");
		printf("0.�˳�\n");
		printf("��ѡ��˵����ţ�0~7����");
	
		nSelection = -1;
		num = scanf("%d", &nSelection);
		while (num == 0)
		{
			scanf("%s", rubbish);
			printf("\n\n��ѡ����ȷ�Ĳ˵����:\n\n ");
			printf("1.���ְ����Ϣ\n");
			printf("2.��ѯְ����Ϣ\n");
			printf("3.�޸�ְ����Ϣ\n");
			printf("4.ְ������ͳ��\n");
			printf("5.���ʼ���\n");
			printf("6.����˰��\n");
			printf("7.��������\n");
			printf("0.�˳�\n");
			printf("��ѡ��˵����ţ�0~7����");

			num = scanf("%d", &nSelection);


		}
		switch (nSelection) {
		case 1: {
			printf("------���ְ����Ϣ-------\n");
			add();
			break;
		}
		case 2: {
			printf( "------��ѯְ����Ϣ-------\n" );
			displayCard(head);
			break;
		}
		case 3: {
			printf("------�޸�ְ����Ϣ-------\n");
			change();
			break;
		}
		case 4: {
			printf( "------ְ������ͳ��-------\n" );
			statictics();
			break;
		}
		case 5: {
			printf("------���ʼ�����ͬ����-------\n");
			Moneydegree();
			break;
		}
		case 6: {
		printf( "------����˰��-------\n" );
		tax();
			break;
		}
		case 7: {
			printf("-------��������------------\n");
			sort();
			break;
		}
		case 0:
		{
			
			printf( "лл��ʹ�ñ�ϵͳ");
			exit(0);
		}
		default:
		{
			printf( "����Ĳ˵���Ŵ���\n" );
			break;
		}
		}
	} while (nSelection != 0);
}