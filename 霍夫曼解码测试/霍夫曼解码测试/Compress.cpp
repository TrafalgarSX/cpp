#include "pch.h"
#include<iostream>
#include"Compress.h"
#include<fstream>
using namespace std;
#define SIZE 256
int InitHead(const char *pFilename, HEAD &sHead) {
	strcpy(sHead.type, "HUF");
	sHead.length = 0;
	for (int i = 0; i < SIZE; i++) {
		sHead.weight[i] = 0;
	}

	//�Զ���������ʽ���ļ�
	FILE *in = fopen(pFilename, "rb");

	//ɨ���ļ����Ȩ��
	int ch;
	while ((ch = fgetc(in)) != EOF) {
		sHead.weight[ch]++;
		sHead.length++;
	}

	//�ر��ļ�
	fclose(in);
	in = NULL;
	return 1;
}


int Compress(const char *pFilename, int weight[256], int letter[256], int n) {

	HuffmanTree<int> ht(weight, letter, 256);

	ht.Huffman_Code();
	int nSize = 0;//������뻺������С��
	for (int i = 0; i < 256; i++) {
		nSize += weight[i] * strlen(ht.Code[i].bit);
	}
	nSize = (nSize % 8 ? nSize / 8 + 1 : nSize / 8);

	//�Ա����ļ�����ѹ��
	char * pBuffer = NULL;
	pBuffer = new char[nSize * sizeof(char)];
	memset(pBuffer, '\0', (nSize) * sizeof(char));
	Encode(pFilename, nSize, ht, pBuffer);
	if (!pBuffer) {
		return 0;
	}

	HEAD sHead;
	InitHead(pFilename, sHead);
	cout << "Ŀ���ļ���С" << sHead.length << "�ֽ�" << endl;
	int afterlen = WriteFile(pFilename, sHead, pBuffer, nSize);
	cout << "ѹ���ļ���С" << afterlen << "�ֽ�  \n����ͷ�ļ�sHead��С�� " << sizeof(sHead) << "�ֽ�";
	cout << "ѹ�����ʣ�" << (double)afterlen * 100 / sHead.length << "%" << endl;

	cout << "���룺";
	if (decode(pFilename, ht)) {
		cout << "����ɹ�!" << endl;
	}
	else {
		cout << "����ʧ�ܣ�" << endl;
	}

	return 1;

}
char Str2byte(const char *pBinStr) {//
	char b = 0x00;
	for (int i = 0; i < 8; i++) {
		b = b << 1;
		if (pBinStr[i] == '1') {
			b = b | 0x01;
		}
	}
	return b;
}
int Encode(const char * pFilename, const int nSize, HuffmanTree<int>&ht, char*pBuffer) {
	FILE *in = fopen(pFilename, "rb");

	if (!pBuffer) {
		cerr << "���ٻ�����ʧ��" << endl;
	}

	char cd[SIZE] = { 0 };
	int pos = 0;
	// unsigned char ch;
	int ch;
	while ((ch = fgetc(in)) != EOF) {
		strcat(cd, ht.Code[ch].bit);
		//cout << ++temp << endl;
		//ѹ������
		while (strlen(cd) >= 8) {
			pBuffer[pos++] = Str2byte(cd);
			for (int i = 0; i < SIZE - 8; i++) {
				cd[i] = cd[i + 8];
			}
		}
		//cout << pos << endl;
	}
	if (strlen(cd) > 0) {
		pBuffer[pos++] = Str2byte(cd);
	}
	cout << pos << endl;
	fclose(in);
	return 1;
}
int WriteFile(const char *pFilename, const HEAD sHead, const char *pBuffer, const int nSize) {
	//�����ļ���
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf");

	//�Զ���������ʽ���ļ�
	FILE *out = fopen(filename, "wb");

	//д�ļ�
	fwrite(&sHead, sizeof(HEAD), 1, out);

	//дѹ����ı���
	fwrite(pBuffer, sizeof(char), nSize, out);
	//�ر��ļ����ͷ��ļ�ָ��
	fclose(out);
	out = NULL;

	cout << "����ѹ���ļ���" << filename << endl;
	int len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}
int decode(const char*pFilename, HuffmanTree<int> &ht) {
	
	ht.mydecode();
	return 1;
}