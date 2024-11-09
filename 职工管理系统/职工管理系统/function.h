#pragma once
#include<time.h>
#include<string.h>
#include<stdio.h>
int checklength(char aa[]);
int moneydegree(float money);
typedef struct Card {

	char names[18];//姓名
	char pass[8];//编号
	int nstatus;//工资级别
	//char sexual[3];//性别
	float balance;//工资
	float tax;//交税信息

	
}Card;
typedef struct CardNode
{
	Card data;
	struct CardNode *next;
}CardNode, *lpCardNode;

int i=0;

CardNode* head;
lpCardNode p, q;

/* 初始化链表*/
int initCardList()
{
	head = (lpCardNode)malloc(sizeof(CardNode));
	head->next = NULL;

	return 1;
}

/*释放结点*/
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

	printf("\n\n----------添加职工信息----------\n");
	printf("请输入姓名<1-5位>:");
	scanf("%s", &pname);

	if (checklength(pname) > 10) {
		printf("姓名超出5位，请重新输入\n");
		return;
	}
	else if (checklength(pname) < 1) {
		printf("姓名不能为空，请重新输入\n");
		return;
	}
	else
	{
		strcpy(name, pname);
	}
	printf("请输入编号<1-8位>：");
	scanf("%s", &ppassword);
	if (checklength(ppassword) > 8) {
		printf("编号超出8位，请重新输入\n");
		return;
	}
	else if (checklength(ppassword) < 1) {
		printf("编号不能为空，请重新输入\n");
		return;
	}
	else
	{
		strcpy(password, ppassword);
	}
	printf("请输入开卡金额<RMB>：");
	scanf("%f", &money);
	if (money < 0) {
		printf("工资不能为0！请重新输入！\n");
		return;
	}

	strcpy(card.names, name);
	strcpy(card.pass, password);
	card.balance = money;
	card.nstatus = moneydegree(card.balance);
	
		printf("\n添加成功!");
		printf("\n\n添加卡的信息如下\n");
		printf("姓名\t\t编号\t\t工资等级\t\t工资\n");
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
/* 初始化链表，并调用函数将卡信息存入链表中*/
//将文件中的卡信息保存在链表里面
int buildCard(Card card,CardNode**cardList)
{
	CardNode* pCardNode= (lpCardNode)malloc(sizeof(CardNode));
	pCardNode->data = card;

	CardNode *ppCardNodeHead = *cardList;
	CardNode *ppCardNodeTail =*cardList;
	if (ppCardNodeHead == NULL) {
		//第一张卡，通过ppCardNodeHead 将表态指针返回
		ppCardNodeHead = pCardNode;
		ppCardNodeTail = pCardNode;
		*cardList = pCardNode;
	}
	else {
		ppCardNodeTail = ppCardNodeHead;
		while (ppCardNodeTail->next != NULL)//找到链表最后一个结点
			ppCardNodeTail = ppCardNodeTail->next;
		ppCardNodeTail->next = pCardNode;//将新建的结点链入链表中
		ppCardNodeTail = pCardNode;//尾指针指到链表尾
	}
	ppCardNodeTail->next = NULL;

	i++;

	return 1;

}


void displayCard(CardNode* pCardNodeHead) {
	if (pCardNodeHead == NULL) {
		printf("%s\n ","一张卡都没有！" );
		return;
	}
	printf("编号\t姓名\t工资\t工资级别\t交税信息\t\n");
	CardNode* pCur = pCardNodeHead;//头结点
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
	printf("请选择要修改的信息：\n");
	printf("1.修改职工姓名\n");
	printf("2.修改职工编号\n");
	printf("3.修改职工工资\n");
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
	printf("请输入需要修改人的姓名<长度为1-5>:");
	scanf("%s", &ppname);
	if (checklength(ppname) > 10)
	{
		printf("姓名超出5位，请重新输入\n");
		return;
	}
	switch (num) {
	case 1: {	
		printf("请输入修改的姓名<长度为1-5>:");
		scanf("%s", &pname);
		if (checklength(pname) > 10)
		{
			printf("姓名超出5位，请重新输入\n");
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
		printf("请输入修改的编号<长度为1-8>:");
		scanf("%s", &ppassword);
		if (checklength(ppassword) > 8)
		{
			printf("编号超出8位，请重新输入\n");
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
		printf("请输入修改的工资：");
		scanf("%f", &money);
		if (money<0)
		{
			printf("工资不能小于0！\n");
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
	printf("请输入需要查询人的姓名<长度为1-5>:");
	scanf("%s", &ppname);
	if (checklength(ppname) > 10)
	{
		printf("姓名超出5位，请重新输入\n");
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
	printf("需要交%.2f的税,税率为%.2f\n", pCardNode->data.tax,percent);
}
void Moneydegree() {
	int num = 0;
	printf("请输入需要查询人的工资级别<级别为1-7>:");
	scanf("%d",&num);
	if (num>7)
	{
		printf("级别超过7，请重新输入\n");
		return;
	}
	CardNode* pCardNode;
	pCardNode = head;
	printf("编号\t姓名\t工资\t工资级别\t交税信息\t\n");
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
	printf("所有员工总工资为%.2f\n", all);
	printf("员工平均工资为%.2f\n", average);


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
	printf("编号\t姓名\t工资\t工资级别\t交税信息\t\n");
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