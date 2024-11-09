#pragma once
#include<iostream>
#include<string>
#include"Huffman.h"
using namespace std;

int InitHead(const char *pFilename, HEAD &sHead);
char Str2byte(const char *pBinStr);
int Encode(const char * pFilename, const int nSize, HuffmanTree<int> &ht, char*pBuffer);
int WriteFile(const char *pFilename, const HEAD sHead, const char *pBuffer, const int nSize);
int Compress(const char *pFilename, int weight[256], int letter[256], int n);
int decode(const char*pFilename, HuffmanTree<int> &ht);

