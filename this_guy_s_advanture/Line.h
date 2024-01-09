#pragma once
#include <cstddef>//不加这个NULL有时会报错(?)
class Line {
public:
	int v1;															//弧头顶点序号
	//int v;															//弧尾顶点序号
	int risk;														//弧的权值
	Line* nextLine;											//后继弧节点的指针
	Line()													//无参构造函数，创建一个空的弧节点
	{
		v1  = -1;	nextLine = NULL;
	}
	Line(int adj1, int w, Line* next = NULL) //有参构造函数
	{
		v1 = adj1;	risk = w;	nextLine = next;
	}
};
