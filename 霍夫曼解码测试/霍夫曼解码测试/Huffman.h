#pragma once
#include<iostream>
#include<string>
#include"Compress.h"
using namespace std;
const int MaxN = 256;
struct HEAD {
	char type[4];//文件类型
	int length;//原文件长度  字节数
	int weight[256];//权重
};
template < class T> struct TreeNode {
	//unsigned char letter='\0';
	int  letter;
	T data;//节点值 
	TreeNode<T> *left, *right, *parent;//左右孩子
	TreeNode() {
		left = NULL;
		right = NULL;
		parent = NULL;
	}
	TreeNode(T x, TreeNode<T> *l = NULL, TreeNode<T> *r = NULL, TreeNode<T> *p = NULL) {
		data = x;
		left = l;
		right = r;
		parent = p;

	}
	bool operator <=(TreeNode<T> &r) {
		return data <= r.data;

	}
	bool operator <(TreeNode<T> &r) {
		return data < r.data;

	}
	bool operator >=(TreeNode<T> &r) {
		return data >= r.data;

	}
	bool operator >(TreeNode<T> &r) {
		return data > r.data;

	}

};

template < class T> class MinHeap {//最小堆     private:
	T *heap;//堆数组 //在此程序中是装有Tree Node<T>类型指针的数组；
	int maxSize;//堆最大容量 
	int currentSize;//当前容量 
public:
	MinHeap(int sz = 256) {
		maxSize = sz;
		heap = new T[maxSize];
		currentSize = 0;
	}
	~MinHeap() {
		delete[]heap;
	}
	T* getHeap() {
		return heap;
	}
	void siftDown(int start, int m) {//向下调整堆数组 
		int i = start;
		int j = 2 * i + 1;
		T temp = heap[i];
		while (j <= m) {
			if (j<m&&*heap[j]>*heap[j + 1]) j++;
			if (*temp <= *heap[j]) break;
			heap[i] = heap[j];
			i = j;
			j = 2 * j + 1;
		}
		heap[i] = temp;
	}
	void siftUp(int start) {//向上调整堆数组 
		int i = start;
		int j = (i - 1) / 2;
		T temp = heap[i];//TreeNode<T>类型的指针
		while (i > 0) {
			if (*temp >= *heap[j]) break;//解引用
			heap[i] = heap[j];
			i = j;
			j = (i - 1) / 2;

		}
		heap[i] = temp;
	}
	bool insert(const T& x) {//插入指向结点指针元素 
		if (currentSize == maxSize) return false;
		heap[currentSize] = x;//x是指向结点的指针 heap是指针数组
		siftUp(currentSize);
		currentSize++;
		return true;
	}
	bool remove(T& x) {//删除元素 
		if (!currentSize) return false;
		x = heap[0];
		heap[0] = heap[currentSize - 1];
		currentSize--;
		siftDown(0, currentSize - 1);
		return true;
	}
};
template<class T>	class HuffmanCode {
public:
	TreeNode<T> *dataptr;
	char bit[MaxN] = { 0 };
	int start = 0;
};
template <class T>class HuffmanTree{
public:
	T currentSize;//编码中用；
	void mydecode();//译码ok
	void Decode(string &a, TreeNode<T>* node, int i);//译码ok
	void Huffman_Code();
	TreeNode<T> *root;
	HuffmanCode<T> *Code;
	HuffmanTree(T w[], int letter[], int n) {//构造Huffman树 
		currentSize = n;
		TreeNode<T> *temp, *first, *second, *parent;
		MinHeap <TreeNode<T>*>hp;
		Code = new HuffmanCode<T>[n];

		for (int i = 0; i < n; i++) {
			temp = new TreeNode<T>;//temp指向TreeNode<T>类型的结点并调用了TreeNode的构造函数；
			temp->data = w[i];
			temp->letter = letter[i];
			Code[i].dataptr = temp;
			hp.insert(temp);
		}
		for (int i = 0; i < n - 1; i++) {
			first = new TreeNode<T>;//指向结点
			second = new TreeNode<T >;//指向结点
			hp.remove(first);
			hp.remove(second);
			parent = new TreeNode<T>;//指向结点
			merge(first, second, parent);
			hp.insert(parent);
		}
		root = parent;//TreeNode<T>类型的指针
	}
	void merge(TreeNode<T> *first, TreeNode<T> *second, TreeNode<T>*&parent) {//选取两个最小带权节点合并 
		parent->left = first;
		parent->right = second;
		parent->data = first->data + second->data;
		first->parent = second->parent = parent;
		parent->parent = NULL;
	}
	~HuffmanTree() {
		destroy(root);
	}
	void destroy(TreeNode<T> *subTree) {//递归删除以subTree为根的所有结点 
		if (subTree != NULL) {
			destroy(subTree->left);
			destroy(subTree->right);
			delete subTree;
			subTree = NULL;
		}

	}

	TreeNode<T> *getRoot() {
		return root;
	}

};

template <class T>void  HuffmanTree <T> ::Huffman_Code()
{
	TreeNode <T> *child, *parent;
	for (int i = 0; i < currentSize; i++)
	{
		child = Code[i].dataptr;
		parent = child->parent;
		int j = 0;
		while (parent != NULL)
		{
			if (parent->left == child) {
				Code[i].bit[j] = '0';
				j++;
			}
			else {
				Code[i].bit[j] = '1';
				j++;
			}
			child = parent;
			parent = parent->parent;
		}
		Code[i].start = j - 1;
	}
	//cout << endl << "输出：" << endl;
	for (int i = 0; i < currentSize; i++)
	{
		reverse(Code[i].bit, Code[i].bit + Code[i].start + 1);
		if (Code[i].dataptr->data != 0) {
			printf("0x%02X", Code[i].dataptr->letter);
			cout << ":";
			cout << Code[i].bit;
			cout << endl;
		}


	}
}

template <class T> void HuffmanTree<T>::mydecode() {
	ifstream openfile("d:\\pic.bmp.huf", ios::in | ios::binary);
	if (openfile.fail())
	{
		cout << "打开文件错误!" << endl;
		exit(0);
	}
	HEAD head;
	string a;
	//openfile.seekp(sizeof(Card)*nCardIndex, ios_base::beg);
	openfile.read((char*)head, sizeof(head));
	int *buffer = new int[head.length];
	openfile.read(buffer, head.length);
	char buffers[8] = { 0 };
	for (int i = 0; i < head.length; i++) {
		itoa(buffer[i], buffers, 2);
		strcat(a, buffers);
	}
	openfile.close();
	static int i = 0;
	Decode(a, root, i);
}

template <class T> void HuffmanTree<T>::Decode(string &a, TreeNode<T>* node, int i) {
	string pFilename="d:\depic.bmp";
	ofstream writefile(pFilename, ios::out | ios::binary);
	if (node->left || node->right) {
		if (a[i] == '0') {
			i++;
			Decode(a, node->left, i);
		}
		if (a[i] == '1') {
			i++;
			Decode(a, node->right, i);
		}
	}
	else {
	//	cout << node->letter;
		writefile.write(node->letter, 1);//读取文件中的信息
		if (i == a.size()) {
			writefile.close();
			exit(0);
		}
		Decode(a, root, i);
	}
	
}