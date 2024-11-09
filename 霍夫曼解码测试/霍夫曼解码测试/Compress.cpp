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

	//以二进制流形式打开文件
	FILE *in = fopen(pFilename, "rb");

	//扫描文件获得权重
	int ch;
	while ((ch = fgetc(in)) != EOF) {
		sHead.weight[ch]++;
		sHead.length++;
	}

	//关闭文件
	fclose(in);
	in = NULL;
	return 1;
}


int Compress(const char *pFilename, int weight[256], int letter[256], int n) {

	HuffmanTree<int> ht(weight, letter, 256);

	ht.Huffman_Code();
	int nSize = 0;//计算编码缓冲区大小；
	for (int i = 0; i < 256; i++) {
		nSize += weight[i] * strlen(ht.Code[i].bit);
	}
	nSize = (nSize % 8 ? nSize / 8 + 1 : nSize / 8);

	//对编码文件进行压缩
	char * pBuffer = NULL;
	pBuffer = new char[nSize * sizeof(char)];
	memset(pBuffer, '\0', (nSize) * sizeof(char));
	Encode(pFilename, nSize, ht, pBuffer);
	if (!pBuffer) {
		return 0;
	}

	HEAD sHead;
	InitHead(pFilename, sHead);
	cout << "目标文件大小" << sHead.length << "字节" << endl;
	int afterlen = WriteFile(pFilename, sHead, pBuffer, nSize);
	cout << "压缩文件大小" << afterlen << "字节  \n其中头文件sHead大小： " << sizeof(sHead) << "字节";
	cout << "压缩比率：" << (double)afterlen * 100 / sHead.length << "%" << endl;

	cout << "解码：";
	if (decode(pFilename, ht)) {
		cout << "解码成功!" << endl;
	}
	else {
		cout << "解码失败！" << endl;
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
		cerr << "开辟缓冲区失败" << endl;
	}

	char cd[SIZE] = { 0 };
	int pos = 0;
	// unsigned char ch;
	int ch;
	while ((ch = fgetc(in)) != EOF) {
		strcat(cd, ht.Code[ch].bit);
		//cout << ++temp << endl;
		//压缩编码
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
	//生成文件名
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf");

	//以二进制流形式打开文件
	FILE *out = fopen(filename, "wb");

	//写文件
	fwrite(&sHead, sizeof(HEAD), 1, out);

	//写压缩后的编码
	fwrite(pBuffer, sizeof(char), nSize, out);
	//关闭文件，释放文件指针
	fclose(out);
	out = NULL;

	cout << "生成压缩文件：" << filename << endl;
	int len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}
int decode(const char*pFilename, HuffmanTree<int> &ht) {
	
	ht.mydecode();
	return 1;
}