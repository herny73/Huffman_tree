//zmx实现哈夫曼树的一次勇敢的尝试
#pragma once
#include<iostream>
#include<cmath>
#include<string>
#include<cstring>
//#include"BiTree.cpp"
using namespace std;

template <class T>
struct HNode
{
	int weight;
	int parent;
	int lch;
	int rch;
};

template <class T>
struct HCode
{
	T data;
	string code;
};

template <class T>
class HuffmanTree
{
private:
	HNode<T> * HTree;
	HCode<T> * HCodeTable;
	int N;
public:
//	HuffmanTree();
//	void CreateHTree(int a[], int n, char name[]);
	void CreateTable(int a[], int n, T name[]);
	void CreateCodeTable();
	void Code(int i, string newcode);
	void Encode(T*s, char*d);
	void Decode(char*s, T*d);
	void Print(int x);
	void SelectMin(int &a, int&b, int l, int r);
	~HuffmanTree();
};

template<class T>
void HuffmanTree<T>::CreateTable(int a[], int n, T name[])
{
	N = n;
	HCodeTable = new HCode<T>[N+1];
	HTree = new HNode<T>[2 * N];//我这就不-1了，为了方便后面把0空出来
	for (int i = 0; i < N; i++)//初始化
	{
		HTree[i].weight = a[i];
		HTree[i].lch = HTree[i].rch = HTree[i].parent = -1;
		HCodeTable[i].data = name[i];
	}
	int x, y;
	for (int i = N; i < 2 * N - 1; i++)
	{
		SelectMin(x, y, 0, i);
	//	cout << x << " " << y << endl;
		HTree[x].parent = HTree[y].parent = i;
		HTree[i].lch = x;
		HTree[i].rch = y;
		HTree[i].weight = HTree[x].weight + HTree[y].weight;
		HTree[i].parent = -1;
	}
	for (int i = 0; i < 2 * N - 1; i++)
	{
		cout << "num\t" << i << "\tweight\t" << HTree[i].weight << "\tlch\t" << HTree[i].lch << "\trch\t" << HTree[i].rch  << "\tparent\t" << HTree[i].parent << "\tdata\t" << HCodeTable[i].data << endl;
	}
	//Print(2 * N - 2);
}
template<class T>
void HuffmanTree<T>::SelectMin(int & a, int & b, int l, int r)//在l到r之间 左闭右开 找到最小和次小的 没被标记的 用a和b传回去
{
	int mark = l;
	while (mark < r)
	{
		if (HTree[mark].parent == -1)
		{
			a = mark;
			
			break;
		}
		mark++;
	}
	mark++;
	while (mark < r)
	{
		if (HTree[mark].parent == -1)
		{
			b = mark;
			
			break;
		}
		mark++;
	}
	if (HTree[a].weight > HTree[b].weight)
		swap(a, b);
	for (int i = a; i < r; i++)
	{
		if (HTree[i].parent == -1)
			if (HTree[i].weight < HTree[a].weight)
				a = i;
	}
	for (int i = b; i < l; i++)
	{
		if (HTree[i].parent == -1)
			if (HTree[i].weight < HTree[b].weight)
				if (i != a)
					b = i;
	}
	return;
}
template<class T>
void HuffmanTree<T>::Code(int i, string newstring)//生成编码表
{
	if (HTree[i].lch==-1)//Huffman的节点只有没儿子和有两个儿子的所以左边没有右边一定没有
	{
		HCodeTable[i].code = newstring;
		return;
	}
	Code(HTree[i].lch, newstring + "0");
	Code(HTree[i].rch, newstring + "1");
}

template<class T>
void HuffmanTree<T>::CreateCodeTable()
{
	Code(2 * N - 2, "");
	for (int i = 0; i < N; i++)
	{
		cout << HCodeTable[i].data << "\tstands for\t" << HCodeTable[i].code << endl;
	}
}

template<class T>
void HuffmanTree<T>::Encode(T * s, char * d)
{
	cout << "encoding...\n" << endl;
	int n = 0;
	T*ss = s;
	int a[280] = { 0 };//这里只支持8位的一下的原编码
	T ch [280] = { 0 };
	while (*ss != '\0')
	{
		a[*ss]++; 
		ss++;
	}
	for (int i = 0; i < 280; i++)
	{
		if (a[i])
		{
			a[n] = a[i];
			ch[n] = i;
			n++;
		}
	}
	cout << "stats done!" << endl;
	cout << n << "characters/marks in total\n\nNow wo create a HuffmanTree,it looks like this:\n";
	CreateTable(a, n, ch);
	cout << "tree creation done!\n\n" << "Now let's look at the codes.\nEach binary number stands for a dedicated character" << endl;
	CreateCodeTable();
	cout << "code table creation done!\n" << endl;
	while (*s != '\0')
	{
		int pos = 0;
		for (; HCodeTable[pos].data != *s; pos++);//得到了编码所在的位置
		//cout << HCodeTable[pos].code;
		char* cstr = new char[20];
		strcpy_s(cstr, HCodeTable[pos].code.size()+1, HCodeTable[pos].code.c_str());
		//cout << cstr;
		strcpy_s(d, strlen(cstr)+1, cstr);//在d的结尾加上转码的字符 这句话我不太确定
		d += strlen(cstr); 
		s++;
	}
	*d = '\0';
}

template<class T>
void HuffmanTree<T>::Decode(char * s, T * d)
{
	
	while (*s != '\0')
	{
		int parent = 2 * N - 2;
		while (HTree[parent].lch != -1)
		{
			if (*s == '0')
				parent = HTree[parent].lch;
			else
				parent = HTree[parent].rch;
			s++;
		}
		*d = HCodeTable[parent].data;
		d++;
	}
	*d = '\0';
}

template<class T>
HuffmanTree<T>::~HuffmanTree()
{
	delete[] HTree;
	delete[] HCodeTable;
}

template<class T>
void HuffmanTree<T>::Print(int x)
{
	
	if (HTree[x].lch == -1)
	{
		cout << "num\t" << x << "\tweight\t" << HTree[x].weight << "\tdata\t" << HCodeTable[x].data << endl;
		return;
	}
	else
	{
		Print(HTree[x].lch);
		Print(HTree[x].rch);
	}
	
	
}

int main()
{
	cout << "hello world!" << endl
		<< "This is a demo of compressing text message using Huffman Tree.\nWritten by Herny Zhou,2019.4\n\n"
		//		<<"Now please select the function you wang to try by typing 'E' to ENCODE your message, or 'D'to DECODE your message"
		<< "Now let's start with a short message.\n"
		<<"Please notice that your message must be SHORTER than 200 characters and contains English lessers or marks ONLY\n"
		<< "Now please type in a sentence and finish with 'Enter'\n";
	unsigned char cstr1[200] = { 0 };
	int iint1[200] = { 0 };
	for (int i = 0;; i++)
	{
		*(cstr1 + i) = cin.get();
		*(iint1 + i) = (int)*(cstr1 + i);
		if (*(cstr1 + i) == '\n')
		{
			*(cstr1 + i) = '\0';
			break;
		}
	}
	cout << "\nRight, now I've got your sentence. it said:\n";
	cout << cstr1 << endl;
	int size_start = 0;
	for (; cstr1[size_start] != '\0'; size_start++);
	size_start *= 8;
	cout << size_start << "bits in total\n";
	cout << "\nAnd now we change it into their corrisponding int:\n";
	for (int i = 0; iint1[i] != 0; i++)
	{
		cout << iint1[i] << " ";
	}
	cout << endl << "All set,and now we encode\n\n";
	char cstr2[20000] = { 0 };
	int cstr3[200] = { 0 };
	HuffmanTree<int> hta;
	hta.Encode(iint1, cstr2);
	cout << "And finally, you have your 'compressed' message.\n It looks like this:\n";
	int size_fin = 0;
	cout << cstr2 << endl;
	for (; cstr2[size_fin] != '\0'; size_fin++);
	cout << size_fin << "bits in total\n";
	double com_rate1 = (double)size_fin / (double)size_start;
	
	cout << "So the compression rate is about" << com_rate1 << "if we do not take the room for storing a HuffmanTree and its coding table.\n" << endl;
	size_fin += 250;
	double com_rate2 = (double)(size_fin) / (double)size_start;
	cout << "If you take that into consideration, the compression rate would probable end up much higher.\nIn this Case, it's around" << com_rate2 << endl;
	cout << "\nI guess there's one more thing left, to see if your message can be revocered from those 0&1s.\n";
	hta.Decode(cstr2, cstr3);
	for (int i = 0; cstr3[i] != 0; i++)
	{
		cout << (unsigned char)cstr3[i];
	}
	cout << "It works! How amazing!";
	system("pause");

	return 0;
}