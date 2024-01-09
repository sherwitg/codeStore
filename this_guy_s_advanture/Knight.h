#pragma once
#include"draw.h"
#include<sstream>
class Knight
{
protected:
	int x, y;							//角色显示坐标
	int fixedHP = 100, fixedWealth = 0;	//固定游戏参数
	int hitPoints;						//当前角色生命值
	int wealthValue;					//当前角色财富值
	int count = 0;						//记录走过的城堡次序(path数组下标)
	int* path;							//记录前进的路径
	bool* vis;							//指向访问标志数组的指针
	//int myLuckCount = 0;				//幸运使用次数计数
public:
	IMAGE cor;
	Knight(int _x = 0, int _y = 0, int hp = 100, int wv = 0)
	{
		this->x = _x;
		this->y = _y;
		this->fixedHP = this->hitPoints = hp;
		this->fixedWealth = this->wealthValue = wv;
		path = new int[100];//每个城堡可以前往不止一次，定义一个尽量大的路径数组
		vis = new bool[100] {0};
		path[0] = 0;
	}
	int GetPath(int i) { return path[i]; }			//获取战士走过的城堡序号
	int GetCount() const { return count; }			//获取战士走过的城堡次序(path数组下标)
	int GetCurPos() const { return path[count]; }	//获取当前位置
	int GetHP() const { return this->hitPoints; }	//获取当前生命值
	int GetWV() const { return this->wealthValue; }	//获取当前财富值
	void UpdateGame()
	{
		this->hitPoints = this->fixedHP;
		this->wealthValue = this->fixedWealth;
		path = new int[100];//每个城堡可以前往不止一次，定义一个尽量大的路径数组
		vis = new bool[100] {0};
		path[0] = 0;
		count = 0;
	}
	int goNext(int harm, int wealth, int v)		
	{
		//去往下一个城堡
		if (!vis[v])
		{
			path[++count] = v;
			this->hitPoints -= harm;				//去往下一个城堡v时减少生命值
			this->wealthValue += wealth;			//若该城堡未被探索过，获取财富值
			vis[v] = 1;
		}
		else
		{
			path[++count] = v;
			this->hitPoints -= harm;
		}
		//判断当前角色情况
		if (this->hitPoints <= 0)
		{
			MessageBoxEx(GetHWnd(), L"很遗憾，勇士已死亡", L"游戏结果:"
				, MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return -1;
		}
		else if (v == 14)
			return 1;

		return 0;
	}
	void Display(COLORREF COLOR)	//展示勇士当前位置，生命值，财富值
	{
		//putimage(0, 0, &before);
		draw::draw(x, y - 15, &cor);
		wchar_t strHP[100];// 定义一个宽字符字符串
		wchar_t strWV[20000];
		swprintf_s(strHP, L"当前生命值: %d", this->hitPoints);//将字符串输出到字符数组
		swprintf_s(strWV, L"当前财富值: %d", this->wealthValue);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		write(widgetLen / 2, widgetWid-100, strHP);
		write(widgetLen / 2, widgetWid-100 + 30, strWV);
	}
};

