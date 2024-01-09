#pragma once
#include"Line.h"
#include<string>
using namespace std;
class Node {
public:
	wstring data;										//城堡名称
	int wealth;											//城堡财富值
	Line* firstLine;									//指向弧链表的头指针
	int x, y;											//确定城堡在地图上的坐标
	Node()	     		       							//无参构造函数，创建一个空的顶点节点
	{
		firstLine = NULL;
	}
	Node(wstring& e, int vWeight, Line* line = NULL) //有参构造函数
	{
		data = e; wealth = vWeight; firstLine = line;
	}
	/*void GetX(int _x) { this->x = _x; }
	void GetY(int _y) { this->y = _y; }*/
};
