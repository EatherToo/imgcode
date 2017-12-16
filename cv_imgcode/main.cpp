#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<math.h>
#include<string.h>
#include"Haffman.h"
using namespace std;
using namespace cv;
list<string> code;
double* calHist(Mat& src)
{
	int col = src.cols;//得到原图对的长宽
	int row = src.rows;

	double* hist = (double*)malloc(sizeof(double)*256);//储存每种灰度的个数
	for (int i = 0; i < 256; i++)
	{
		hist[i] = 0.0;//全初始化为0
	}

	for (int i = 0; i < row; i++)
	{
		uchar* data = src.ptr<uchar>(i);
		for (int j = 0; j < col; j++)
		{
			hist[data[j]]++;//每种灰度被遍历到是都会自动加一
		}
	}

	double all  = col*row;
	for (int i = 0; i < 256; i++)
	{
		hist[i] = hist[i] / all;
	}
	return hist;
}

void order(Haffman* haff,int start,int end)
{
	for (int i = start; i < end+1; i++)
	{
		for (int j = i; j < end+1; j++)
		{
			if (haff[i].weight > haff[j].weight)
			{
				double temp = haff[i].weight;
				haff[i].weight = haff[j].weight;
				haff[j].weight = temp;

				int temp_ = haff[i].parent;
				haff[i].parent = haff[j].parent;
				haff[j].parent = temp_;

				temp_ = haff[i].right;
				haff[i].right = haff[j].right;
				haff[j].right = temp_;

				temp_ = haff[i].left;
				haff[i].left = haff[j].left;
				haff[j].left = temp_;

				temp_ = haff[i].flag;
				haff[i].flag = haff[j].flag;
				haff[j].flag = temp_;
			}
		}
	}
}

void search(Haffman haffman[], int root, string s)
{

	if (haffman[root].left == 0 && haffman[root].right == 0)
	{
		cout << haffman[root].flag << ": " << s << endl;
		//code.push_back(s);
	}
	else
	{
		search(haffman, haffman[root].left, s + "1");
		search(haffman, haffman[root].right, s + "0");
	}
	return;
}

string* calHaffman(double* hist)
{

	Haffman haffman[256*2];//对于哈夫曼树来说，总结点个数未 2*n-1

	//初始化所有哈夫曼的叶子节点
	for (int i = 1; i <= 256; i++)
	{
		haffman[i].weight = hist[i-1];
		haffman[i].flag = i;
		haffman[i].left = 0;
		haffman[i].right = 0;
		haffman[i].parent = 0;
	}
	for (int i = 257; i <= 256*2-1; i++)
	{
		haffman[i].weight = 0;
		haffman[i].flag = 0;
		haffman[i].left = 0;
		haffman[i].right = 0;
		haffman[i].parent = 0;
	}
	order(haffman, 1, 256);
	haffman[1].weight = 0.0;
	haffman[2].weight = 0.0;
	for (int i = 256+1; i < 256 * 2 - 1; i++)
	{
		int s1, s2;
		order(haffman, 3, i-1);
		for (int j = 3; j <= i; j++)
		{
			if (haffman[j].parent == 0)
			{
				s1 = j;
				break;
			}
		}
		
		for (int j = 3; j <= i; j++)
		{
			if (haffman[j].parent == 0&&j!=s1)
			{
				s2 = j;
				break;
			}
		}

		haffman[i].weight = haffman[s1].weight + haffman[s2].weight;
		haffman[s1].parent = i;
		haffman[s2].parent = i;
		haffman[i].left = s1;
		haffman[i].right = s2;
	}
	haffman[510].right = 1;

	search(haffman, 510, "");

	string a = "   ";
	
	return &a;



	
}



int main()
{
	Mat jeff = imread("jeff.jpg", 0);
	imshow("jeff", jeff);
	double* hist = calHist(jeff);
	double H = 0.0;
	for (int i = 0; i < 256; i++)
	{
		if (hist[i] == 0.0)
			H += 0.0;
		else
			H += hist[i] * log(hist[i]);
	}
	H = -H;
	cout << "jeff的一元灰度熵：" << H << endl;
	calHaffman(hist);

	




	Mat view = imread("view.jpg", 0);
	imshow("view", view);
	hist = calHist(view);
	H = 0.0;
	for (int i = 0; i < 256; i++)
	{
		if(hist[i]==0.0)
			H += 0.0;
		else
			H += hist[i] * log(hist[i]);
	}
	H = -H;
	cout << "风景的一元灰度熵：" << H << endl;

	Mat flower = imread("flower.jpg", 0);
	imshow("flower", flower);
	hist = calHist(flower);
	H = 0.0;
	for (int i = 0; i < 256; i++)
	{
		if (hist[i] == 0.0)
			H += 0.0;
		else
			H += hist[i] * log(hist[i]);
	}
	H = -H;
	cout << "花的一元灰度熵：" << H << endl;
	waitKey();
}