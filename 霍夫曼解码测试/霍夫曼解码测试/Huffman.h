#pragma once
#include<iostream>
#include<string>
#include"Compress.h"
using namespace std;
const int MaxN = 256;
struct HEAD {
	char type[4];//�ļ�����
	int length;//ԭ�ļ�����  �ֽ���
	int weight[256];//Ȩ��
};
template < class T> struct TreeNode {
	//unsigned char letter='\0';
	int  letter;
	T data;//�ڵ�ֵ 
	TreeNode<T> *left, *right, *parent;//���Һ���
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

template < class T> class MinHeap {//��С��     private:
	T *heap;//������ //�ڴ˳�������װ��Tree Node<T>����ָ������飻
	int maxSize;//��������� 
	int currentSize;//��ǰ���� 
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
	void siftDown(int start, int m) {//���µ��������� 
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
	void siftUp(int start) {//���ϵ��������� 
		int i = start;
		int j = (i - 1) / 2;
		T temp = heap[i];//TreeNode<T>���͵�ָ��
		while (i > 0) {
			if (*temp >= *heap[j]) break;//������
			heap[i] = heap[j];
			i = j;
			j = (i - 1) / 2;

		}
		heap[i] = temp;
	}
	bool insert(const T& x) {//����ָ����ָ��Ԫ�� 
		if (currentSize == maxSize) return false;
		heap[currentSize] = x;//x��ָ�����ָ�� heap��ָ������
		siftUp(currentSize);
		currentSize++;
		return true;
	}
	bool remove(T& x) {//ɾ��Ԫ�� 
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
	T currentSize;//�������ã�
	void mydecode();//����ok
	void Decode(string &a, TreeNode<T>* node, int i);//����ok
	void Huffman_Code();
	TreeNode<T> *root;
	HuffmanCode<T> *Code;
	HuffmanTree(T w[], int letter[], int n) {//����Huffman�� 
		currentSize = n;
		TreeNode<T> *temp, *first, *second, *parent;
		MinHeap <TreeNode<T>*>hp;
		Code = new HuffmanCode<T>[n];

		for (int i = 0; i < n; i++) {
			temp = new TreeNode<T>;//tempָ��TreeNode<T>���͵Ľ�㲢������TreeNode�Ĺ��캯����
			temp->data = w[i];
			temp->letter = letter[i];
			Code[i].dataptr = temp;
			hp.insert(temp);
		}
		for (int i = 0; i < n - 1; i++) {
			first = new TreeNode<T>;//ָ����
			second = new TreeNode<T >;//ָ����
			hp.remove(first);
			hp.remove(second);
			parent = new TreeNode<T>;//ָ����
			merge(first, second, parent);
			hp.insert(parent);
		}
		root = parent;//TreeNode<T>���͵�ָ��
	}
	void merge(TreeNode<T> *first, TreeNode<T> *second, TreeNode<T>*&parent) {//ѡȡ������С��Ȩ�ڵ�ϲ� 
		parent->left = first;
		parent->right = second;
		parent->data = first->data + second->data;
		first->parent = second->parent = parent;
		parent->parent = NULL;
	}
	~HuffmanTree() {
		destroy(root);
	}
	void destroy(TreeNode<T> *subTree) {//�ݹ�ɾ����subTreeΪ�������н�� 
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
	//cout << endl << "�����" << endl;
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
		cout << "���ļ�����!" << endl;
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
		writefile.write(node->letter, 1);//��ȡ�ļ��е���Ϣ
		if (i == a.size()) {
			writefile.close();
			exit(0);
		}
		Decode(a, root, i);
	}
	
}