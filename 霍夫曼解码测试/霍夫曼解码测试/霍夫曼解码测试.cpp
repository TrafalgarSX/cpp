// 霍夫曼解码测试.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include"Huffman.h"
#include<iostream>
#include<string>
#include<stdlib.h>
#include"Compress.h"
using std::string;
using namespace std;
class inf {
public:
	char ch;
	int num;
};
int main() {
	cout << "=========Huffman文件压缩=================" << endl;
	cout << "请输入文件名：";
	char filename[256];
	cin >> filename;
	int weight[256] = { 0 };
	FILE *in = fopen(filename, "rb");
	//char ch;
	int ch;
	while ((ch = getc(in)) != EOF) {

		weight[ch]++;
	}
	fclose(in);
	cout << "Byte" << " Weight" << endl;
	int temp = 0;
	for (int i = 0; i < 256; i++) {
		printf("0x%02X %d\n", i, weight[i]);
		temp += weight[i];
	}
	cout << temp << endl;
	int letter[256];
	for (int i = 0; i < 256; i++) {
		letter[i] = i;
		//printf("0x%02X\n", letter[i]);
	}
	if (Compress(filename, weight, letter, 256)) {
		cout << "压缩成功！" << endl;
	}
	else {
		cout << "压缩失败！" << endl;
	}



}