#pragma once
#include<time.h>
#include<string.h>
#include<stdio.h>
int checklength(char aa[]);
int moneydegree(float money);
typedef struct Card {

	char names[18];//����
	char pass[8];//���
	int nstatus;//���ʼ���
	//char sexual[3];//�Ա�
	float balance;//����
	float tax;//��˰��Ϣ

	
}Card;
typedef struct CardNode
{
	Card data;
	struct CardNode *next;
}CardNode, *lpCardNode;

int i=0;

CardNode* head;
lpCardNode p, q;

/* ��ʼ������*/
int initCardList()
{
	head = (lpCardNode)malloc(sizeof(CardNode));
	head->next = NULL;

	return 1;
}

/*�ͷŽ��*/
void releaseCardList()
{
	p = head;

	while (p != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
}

void add()
{
	char name[18] = { '0' };
	char pname[1000];
	char password[8] = { '0' };
	char ppassword[1000];
	float money;

	Card card;

	printf("\n\n----------���ְ����Ϣ----------\n");
	printf("����������<1-5λ>:");
	scanf("%s", &pname);

	if (checklength(pname) > 10) {
		printf("��������5λ������������\n");
		return;
	}
	else if (checklength(pname) < 1) {
		printf("��������Ϊ�գ�����������\n");
		return;
	}
	else
	{
		strcpy(name, pname);
	}
	printf("��������<1-8λ>��");
	scanf("%s", &ppassword);
	if (checklength(ppassword) > 8) {
		printf("��ų���8λ������������\n");
		return;
	}
	else if (checklength(ppassword) < 1) {
		printf("��Ų���Ϊ�գ�����������\n");
		return;
	}
	else
	{
		strcpy(password, ppassword);
	}
	printf("�����뿪�����<RMB>��");
	scanf("%f", &money);
	if (money < 0) {
		printf("���ʲ���Ϊ0�����������룡\n");
		return;
	}

	strcpy(card.names, name);
	strcpy(card.pass, password);
	card.balance = money;
	card.nstatus = moneydegree(card.balance);
	
		printf("\n��ӳɹ�!");
		printf("\n\n��ӿ�����Ϣ����\n");
		printf("����\t\t���\t\t���ʵȼ�\t\t����\n");
		printf("%s\t\t%s\t\t%d\t\t%.2f\n", card.names, card.pass, card.nstatus, money);
		buildCard(card,&head);
}


int moneydegree(float money) {

	if (money <= 5000.0) {
		return 1;
	 }
	else if (money >= 8001.0 && money <= 17000.0) {
		return 2;
	}
	else if (money >= 17001.0 && money <= 30000.0) {
		return 3;
	}
	else if (money >=30001.0 && money <= 40000.0) {
		return 4;
	}
	else if (money >= 40001.0&& money <= 60000.0) {
		return 5;
	}
	else if (money >= 60001.0&& money <= 85000.0) {
		return 6;
	}
	else {
		return 7;
	}
}
/* ��ʼ�����������ú���������Ϣ����������*/
//���ļ��еĿ���Ϣ��������������
int buildCard(Card card,CardNode**cardList)
{
	CardNode* pCardNode= (lpCardNode)malloc(sizeof(CardNode));
	pCardNode->data = card;

	CardNode *ppCardNodeHead = *cardList;
	CardNode *ppCardNodeTail =*cardList;
	if (ppCardNodeHead == NULL) {
		//��һ�ſ���ͨ��ppCardNodeHead ����ָ̬�뷵��
		ppCardNodeHead = pCardNode;
		ppCardNodeTail = pCardNode;
		*cardList = pCardNode;
	}
	else {
		ppCardNodeTail = ppCardNodeHead;
		while (ppCardNodeTail->next != NULL)//�ҵ��������һ�����
			ppCardNodeTail = ppCardNodeTail->next;
		ppCardNodeTail->next = pCardNode;//���½��Ľ������������
		ppCardNodeTail = pCardNode;//βָ��ָ������β
	}
	ppCardNodeTail->next = NULL;

	i++;

	return 1;

}


void displayCard(CardNode* pCardNodeHead) {
	if (pCardNodeHead == NULL) {
		printf("%s\n ","һ�ſ���û�У�" );
		return;
	}
	printf("���\t����\t����\t���ʼ���\t��˰��Ϣ\t\n");
	CardNode* pCur = pCardNodeHead;//ͷ���
	while (pCur != NULL) {
		 printf("%s\t", pCur->data.pass);
		 printf("%s\t", pCur->data.names);
		// printf("%s\n", pCur->data.sexual);
		 printf("%.2f\t", pCur->data.balance);
		 printf("%d\n", pCur->data.nstatus);
		//printf("%s\n", pCur->data.tax);

		pCur = pCur->next;
	}
}
int change() {
	int i = 0;
	printf("��ѡ��Ҫ�޸ĵ���Ϣ��\n");
	printf("1.�޸�ְ������\n");
	printf("2.�޸�ְ�����\n");
	printf("3.�޸�ְ������\n");
	scanf("%d", &i);
	changeinfo(head, i);
}
int changeinfo(CardNode* pCardNodeHead, int num) {
	char name[18] = { '0' };
	char pname[1000];
	char ppname[1000];
	char password[8] = { '0' };
	char ppassword[1000] = { '0' };
	float money;
	printf("��������Ҫ�޸��˵�����<����Ϊ1-5>:");
	scanf("%s", &ppname);
	if (checklength(ppname) > 10)
	{
		printf("��������5λ������������\n");
		return;
	}
	switch (num) {
	case 1: {	
		printf("�������޸ĵ�����<����Ϊ1-5>:");
		scanf("%s", &pname);
		if (checklength(pname) > 10)
		{
			printf("��������5λ������������\n");
			return;
		}
		else {
			strcpy(name, pname);
		}

		CardNode* pCardNode;
		pCardNode = pCardNodeHead;
		while (strcmp(pCardNode->data.names, ppname)) {
			pCardNode = pCardNode->next;
		}
		strcpy(pCardNode->data.names, name);
		break;
	}
	case 2: {
		printf("�������޸ĵı��<����Ϊ1-8>:");
		scanf("%s", &ppassword);
		if (checklength(ppassword) > 8)
		{
			printf("��ų���8λ������������\n");
			return;
		}
		else {
			strcpy(password, ppassword);
		}
		CardNode* pCardNode;
		pCardNode = pCardNodeHead;
		while (strcmp(pCardNode->data.pass, password)) {
			pCardNode = pCardNode->next;
		}
		strcpy(pCardNode->data.pass, password);
		break;
	}
	case 3:{
		printf("�������޸ĵĹ��ʣ�");
		scanf("%f", &money);
		if (money<0)
		{
			printf("���ʲ���С��0��\n");
			return;
		}
		CardNode* pCardNode;
		pCardNode = pCardNodeHead;
		while (strcmp(pCardNode->data.names, ppname)) {
			pCardNode = pCardNode->next;
		}
		pCardNode->data.balance = money;
		pCardNode->data.nstatus = moneydegree(money);
		break;
    }
		  

    }
}
int checklength(char aa[])
{
	int count = 0;
	while (aa[count] != '\0') {
		count++;
	}
	return count;
}
float tax() {
	char name[18] = { '0' };
	char ppname[1000];
	printf("��������Ҫ��ѯ�˵�����<����Ϊ1-5>:");
	scanf("%s", &ppname);
	if (checklength(ppname) > 10)
	{
		printf("��������5λ������������\n");
		return;
	}
	CardNode* pCardNode;
	pCardNode = head;
	while (strcmp(pCardNode->data.names, ppname)) {
		pCardNode = pCardNode->next;
	}
	int tax = pCardNode->data.nstatus;
	float percent;
	switch (tax) {
	case 1:percent = 0; break;
	case 2:percent = 0.03; break;
	case 3:percent = 0.1; break; 
	case 4:percent = 0.2; break; 
	case 5:percent = 0.25; break; 
	case 6:percent = 0.3; break;
	case 7:percent = 0.35; break;
	}
	pCardNode->data.tax = pCardNode->data.balance*percent;
	printf("%s", pCardNode->data.names);
	printf("��Ҫ��%.2f��˰,˰��Ϊ%.2f\n", pCardNode->data.tax,percent);
}
void Moneydegree() {
	int num = 0;
	printf("��������Ҫ��ѯ�˵Ĺ��ʼ���<����Ϊ1-7>:");
	scanf("%d",&num);
	if (num>7)
	{
		printf("���𳬹�7������������\n");
		return;
	}
	CardNode* pCardNode;
	pCardNode = head;
	printf("���\t����\t����\t���ʼ���\t��˰��Ϣ\t\n");
	while (pCardNode) {
		if (pCardNode->data.nstatus == num) {
				printf("%s\t", pCardNode->data.pass);
				printf("%s\t", pCardNode->data.names);
				// printf("%s\n", pCur->data.sexual);
				printf("%.2f\t", pCardNode->data.balance);
				printf("%d\n", pCardNode->data.nstatus);
			//	printf("%s\n", pCardNode->data.tax);
			
		}
		pCardNode = pCardNode->next;
	}
	
}
void statictics() {
	float all=0;
	float average;
	CardNode* pCardNode;
	pCardNode = head;
	while (pCardNode!=NULL) {
		all += pCardNode->data.balance;
		pCardNode = pCardNode->next;
	}
	average = all / i;
	printf("����Ա���ܹ���Ϊ%.2f\n", all);
	printf("Ա��ƽ������Ϊ%.2f\n", average);


}
void sort() {
	CardNode* pCardNode;
	pCardNode = head;
	CardNode *pCardNode_1;
	float temp;
	char name[18] = { '0' };
	char pass[8] = { '0' };
	int num;
	for (; pCardNode != NULL; pCardNode = pCardNode->next) {
		for (pCardNode_1 = pCardNode->next; pCardNode_1 != NULL; pCardNode_1 = pCardNode_1->next) {
			if (pCardNode->data.balance < pCardNode_1->data.balance) {
				temp = pCardNode_1->data.balance;
				pCardNode_1->data.balance = pCardNode->data.balance;
				pCardNode->data.balance = temp;

				num= pCardNode_1->data.nstatus;
				pCardNode_1->data.nstatus = pCardNode->data.nstatus;
				pCardNode->data.nstatus = num;

				strcpy(name, pCardNode_1->data.names);
				strcpy(pCardNode_1->data.names, pCardNode->data.names);
				strcpy(pCardNode->data.names, name);

				strcpy(pass, pCardNode_1->data.pass);
				strcpy(pCardNode_1->data.pass, pCardNode->data.pass);
				strcpy(pCardNode->data.pass, pass);


			}
		}
	}
	CardNode* pCur;
	pCur = head;
	printf("���\t����\t����\t���ʼ���\t��˰��Ϣ\t\n");
	while (pCur != NULL) {
		printf("%s\t", pCur->data.pass);
		printf("%s\t", pCur->data.names);
		// printf("%s\n", pCur->data.sexual);
		printf("%.2f\t", pCur->data.balance);
		printf("%d\n", pCur->data.nstatus);
		//	printf("%s\n", pCardNode->data.tax);
		pCur = pCur->next;
	}



}