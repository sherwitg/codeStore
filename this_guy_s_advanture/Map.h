#pragma once
#include <random>
#include"draw.h"
#include"Node.h"
#include"Line.h"
using namespace std;
const int DEFAULT_INFINITY = 0x3f3f3f3f;

//极坐标布局算法实现无向连接图 -> 图像拟合度过高

double distance(Node a, Node b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//斜率判断
//bool slope(Node& n1,Node& n2, Node& n3)
//{
//    return ((n3.y - n2.y) * (n2.x - n1.x) - (n2.y - n2.y) * (n3.x - n2.x) < 15);
//}

void check(int tag, Node* nodes, int i)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> delta(0, 50); // X的网格范围
    int n = i-1;
    while (n--)
    {
        if(tag == 2)
        while (distance(nodes[i],nodes[n]) < 50)
            nodes[i].y += 80;
        else if(tag == 1)
            while (distance(nodes[i], nodes[n]) < 50)
                nodes[i].y -= 80;
    }
    //for (int j = 0; j < i; j++)
    //{
    //    for (int k = j; k < i; k++)
    //        //检查斜率
    //        if (slope(nodes[j], nodes[k], nodes[i]))
    //            nodes[i].y = delta(gen) + 150;
    //}
}

class Map
{
protected:
	int nodeNum = 0, nodeMaxSize, lineNum = 0;
	Node* nodes;
	Line* lines;
	int infinity = DEFAULT_INFINITY;
	Status* visited;
	int* safestDist;
	int* safestPath;
	int* richestDist;
	int* richestPath;
    int resWealth = 0;								//记录当前最高财富值
    
public:
    IMAGE* before;                                              //此前的图像信息
    Map() { before = new IMAGE(widgetLen, widgetWid); }
    void loadMap();                                             //网格算法处理点坐标分布
    bool CreateMap(wstring str);                                //初始化世界地图
    void ShowCurMap();                                          //展示地图
    void draw_nodes();                                          //绘制点
    void draw_edges();                                          //绘制线
    void closeMap() { closegraph(); }
    int GetNodeNum() const { return this->nodeNum; }		 //获取当前顶点数目
    int GetNodeMaxSize() const { return this->nodeMaxSize; } //获取最大顶点数目
    int GetLineNum() const { return this->lineNum; };	     //获取当前弧的数目
    Status GetVisitedTag(int v) const;
    Status SetVisitedTag(int v, Status tag) const;
    int GetInfinity() const { return DEFAULT_INFINITY; }	//获取表示无穷大的值
    int GetNodeIndex(const wstring& ch)const;
    wstring GetNodeName(int i)const { return nodes[i].data; }
    void GetMapPos(int i, int& x, int& y) { x = nodes[i].x; y = nodes[i].y; }
    int GetWealth(int i) { return nodes[i].wealth; }
    int GetRisk(int v1, int v2)const;
    int GetWeight(int v1, int v2) const;
    void redraw(int id);
    void redraw2(int id);
    int GetFirstAdjvex(int v) const;				        //获取顶点v的第一个邻接点
    int GetNextAdjvex(int v1, int v2)const;			        //获取顶点v1相对于v2的后继邻接点
    void FindSafestLoad();			                        //小心为上
    void OutputSafestLoad();					            //输出生命值最高的路线
    void FindRichestLoad();			                        //富贵险中求
    void DFS(int in, int v, int& hp, int& maxWealth, int* tmpPath); //DFS+剪枝求财富值最高的路线
    void OutputRichestLoad(int curHP)const;	                //输出财富值最高的路线
    Status InsertVex(const wstring& e, int VexWeight);	    //插入数据元素值为e,权值为VexWeight的顶点
    Status InsertArc(int v1, int v2, int w);		        //插入顶点v1<->v2，权为w的弧
    virtual ~Map();
    void updateMap()const;
};

Status Map::InsertVex(const wstring& e, int VexWeight) {	//插入数据元素值为e的顶点
    if (nodeNum == nodeMaxSize)  return OVER_FLOW;		//顶点数组已满
    nodes[nodeNum].data = e;								//将顶点数据存储到顶点数组尾部
    nodes[nodeNum].wealth = VexWeight;					//城堡财富值
    nodes[nodeNum].firstLine = NULL;
    visited[nodeNum] = UNVISITED;
    nodeNum++;
    return SUCCESS;
}

Status Map::InsertArc(int v1, int v2, int w) {	//插入顶点v1<->v2权为w的弧
    if (v1 < 0 || v1 >= nodeNum) 						//判断v1是否越界
    {
        cout << "v1取值不合法!" << endl;  return FAILED;
    }
    if (v2 < 0 || v2 >= nodeNum)						//判断v2是否越界
    {
        cout << "v2取值不合法!" << endl;  return FAILED;
    }
    if (v1 == v2) 									//判断v1和v2是否相等
    {
        cout << "v1与v2不能相等!" << endl;  return FAILED;
    }
    if (w == infinity)								//判断w取值是否为无穷
    {
        cout << "权值w不能为无穷大!" << endl;  return FAILED;
    }
    nodes[v1].firstLine = new Line(v2, w, nodes[v1].firstLine);
    nodes[v2].firstLine = new Line(v1, w, nodes[v2].firstLine);
    lineNum++;
    return SUCCESS;
}

void Map::draw_nodes() {
    for (int i = 0; i < nodeNum; i++) {
        solidcircle(nodes[i].x, nodes[i].y, vexRadius);//绘制点
        setbkmode(TRANSPARENT);
        settextcolor(BLACK);//文本颜色设置为深灰色
        wstring ch = GetNodeName(i);
        write(nodes[i].x, nodes[i].y, ch.c_str());
    }
}

void Map::draw_edges() {
    for (int v = 0; v < nodeNum; v++) {
        setlinecolor(DarkGray);
        for (int w = GetFirstAdjvex(v); w != -1; w = GetNextAdjvex(v, w))
            line(nodes[w].x, nodes[w].y, nodes[v].x, nodes[v].y);//绘制线条
    }
}

void Map::loadMap()
{ 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis1(30, 50); // X的网格范围
    std::uniform_int_distribution<> dis2(0, 90); // Y的网格范围  
    nodes[0].x = 180;
    nodes[0].y = 100;//固定起点v0的位置
    nodes[14].x = 800;
    nodes[14].y = 100;//固定终点的位置
    for (int i = 1; i < nodeNum -1; i ++) 
        nodes[i].x = nodes[i - 1].x + dis1(gen);//保证x非递减
    for (int i = 1; i < nodeNum -1; i += 2) 
    {
        nodes[i].y = 150 + dis2(gen);//序号为奇数则下落
        check(1, nodes, i);
    }
    for (int i = 2; i < nodeNum -1; i += 2) 
    {
        nodes[i].y = 150 - dis2(gen);//序号为偶数则上浮
        check(2, nodes, i);
    }
    draw_nodes(); // 在窗口上绘制节点  
    draw_edges(); // 在窗口上绘制连接线
}

bool Map::CreateMap(wstring str)
{
    wifstream infile;
    infile.open(str);					                //打开文件
    if (!infile.is_open())return 0;						//文件打开失败
    //创建世界地图
    int vexNum, arcNum;
    infile >> vexNum >> arcNum;							//获取顶点数目和弧数目
    this->nodeMaxSize = vexNum;
    nodes = new Node[vexNum];
    lines = new Line[arcNum];
    visited = new Status[vexNum];
    //创建城堡
    for (int i = 0; i < vexNum; i++)					
    {
        wstring ch;										//城堡名称
        int wealth;										//城堡财富值
        infile >> ch >> wealth;							//获取数据
        this->InsertVex(ch, wealth);					//插入顶点
    }
    //创建道路
    for (int i = 0; i < arcNum; i++)					
    {
        int v1, v2, w;
        infile >> v1 >> v2 >> w;						
        this->InsertArc(v1, v2, w);						//插入v1<->v2权值为w的边
    }
    infile.close();

    return 1;
}

void Map::ShowCurMap()
{
    setbkcolor(WHITE);
    settextstyle(20, 0, _T("Arial Rounded MT Bold"));
    cleardevice();
    setfillcolor(RGB(240, 240, 240));
    //计算二维点坐标
    loadMap();
}

int Map::GetNodeIndex(const wstring& ch) const {	//获取顶点v的数据元素值
    for (int v = 0; v < nodeNum; v++)					//遍历顶点数组
        if (nodes[v].data == ch)						//判断顶点的数据元素是否等于e
            return v;								//相等则返回顶点下标
    return -1;										//未找到，返回-1
}

int Map::GetWeight(int v1, int v2) const {	//获取从顶点v1到v2的弧的权值
    if (v1 < 0 || v1 >= nodeNum) 						//判断v1是否越界
    {
        cout << "v1取值不合法!" << endl;  return infinity;
    }
    if (v2 < 0 || v2 >= nodeNum)						//判断v2是否越界
    {
        cout << "v2取值不合法!" << endl;  return infinity;
    }
    if (v1 == v2) 									//判断v1和v2是否相等
        return 0;
    Line* p;
    p = nodes[v1].firstLine;							//p指向第一个弧节点
    while (p && p->v1 != v2)						        //寻找邻接点v2
        p = p->nextLine;
    if (!p)  return infinity;						//v2不是v1的邻接点，返回无穷
    return p->risk;								//返回从顶点v1到v2的弧的权值
}

int Map::GetRisk(int v1, int v2) const {	//获取顶点v1与v2之间的弧的权值
    return min(GetWeight(v1, v2), GetWeight(v2, v1));
}

void Map::redraw(int id)//访问的节点
{
    setfillcolor(DarkGray);
    solidcircle(nodes[id].x, nodes[id].y, 10);
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(20, 0, _T("Arial Rounded MT Bold"));
    write(nodes[id].x, nodes[id].y, this->GetNodeName(id).c_str());
}

void Map::redraw2(int id)//已访问的节点为深绿色
{
    putimage(0, 0, before);
    setfillcolor(DarkGreen);
    solidcircle(nodes[id].x, nodes[id].y, 10);
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(20, 0, _T("Arial Rounded MT Bold"));
    write(nodes[id].x, nodes[id].y, this->GetNodeName(id).c_str());
    getimage(before, 0, 0, widgetLen, widgetWid);
}

Status Map::GetVisitedTag(int v) const {	//获取顶点v的访问标志
    if (v < 0 || v >= nodeNum)						//判断v是否越界
    {
        cout << "v取值不合法!" << endl;  return FAILED;
    }
    return visited[v];
}

Status Map::SetVisitedTag(int v, Status tag) const {	//修改顶点v的访问标志为tag
    if (v < 0 || v >= nodeNum)							//判断v是否越界
    {
        cout << "v取值不合法!" << endl;  return FAILED;
    }
    visited[v] = tag;
    return SUCCESS;
}

int Map::GetFirstAdjvex(int v) const {	//获取顶点v的第一个邻接点
    if (v < 0 || v >= nodeNum)						//判断v是否越界
    {
        cout << "v取值不合法!" << endl;  return -1;
    }
    if (nodes[v].firstLine == NULL)  return -1;		//不存在邻接点
    else  return nodes[v].firstLine->v1;			//返回第一个弧节点记录的弧头顶点序号
}

int Map::GetNextAdjvex(int v1, int v2) const {	//获取顶点v1相对于v2的后继邻接点
    Line* p;
    if (v1 < 0 || v1 >= nodeNum) 						//判断v1是否越界
    {
        cout << "v1取值不合法!" << endl;  return -1;
    }
    if (v2 < 0 || v2 >= nodeNum)						//判断v2是否越界
    {
        cout << "v2取值不合法!" << endl;  return -1;
    }
    if (v1 == v2) 									//判断v1和v2是否相等
    {
        cout << "v1与v2不能相等!" << endl;  return -1;
    }
    p = nodes[v1].firstLine;							//p指向弧链表的第一个弧节点
    while (p != NULL && p->v1 != v2)				//寻找v2所在的弧节点
        p = p->nextLine;
    if (p == NULL || p->nextLine == NULL)  return -1;	//不存在后继邻接点
    else  return p->nextLine->v1;				//返回后继弧节点记录的弧头顶点序号
}

//找到生命值最高的路径
void Map::FindSafestLoad()
{
    safestPath = new int[nodeMaxSize];
    safestDist = new int[nodeMaxSize];
    int minharm, infinity = GetInfinity();
    int v, u;
    safestPath[0] = -1;										//起点没有前驱
    safestDist[0] = 0;										//初始状态伤害值为0
    for (v = 1; v < GetNodeNum(); v++) {				//初始化数组dist、path及顶点标志
        safestDist[v] = GetRisk(0, v);					//获取v0->v的权值
        if (safestDist[v] >= 100)  safestPath[v] = -1;		//勇士走上死路，表明不存在可行的v0->v路线
        else  safestPath[v] = 0;							//存在弧<v0，v>
        SetVisitedTag(v, UNVISITED);        				//置顶点访问标志为未访问，表示未确定最短路径
    }
    SetVisitedTag(0, VISITED);								//无需寻找源点v0的最短路径
    for (int i = 1; i < GetNodeNum(); i++) {			//寻找从源点v0到其他顶点的最短路径
        u = 0;  			  								//u记录当前距离值最小的顶点，初始化为源点
        minharm = infinity;									//minharm记录距离值的最小值，初始化为无穷
        //在尚未确定最短路径的顶点中寻找距离值最小的顶点
        for (v = 0; v < GetNodeNum(); v++)
            if (GetVisitedTag(v) == UNVISITED && safestDist[v] < minharm)
            {
                u = v;	    minharm = safestDist[v];
            }
        SetVisitedTag(u, VISITED);							//确定了顶点u的最短路径
        //检查u的尚未确定最短路径的邻接点
        for (v = GetFirstAdjvex(u); v != -1; v = GetNextAdjvex(u, v))
            if (GetVisitedTag(v) == UNVISITED && minharm + GetRisk(u, v) < safestDist[v]) {
                //尚未确定v的最短路径并且从v0沿着u的最短路径到达u再经由<u, v>到达v更短
                safestDist[v] = minharm + GetRisk(u, v);	//修改顶点v的距离值
                safestPath[v] = u;							//修改顶点v的前驱
            }
    }
    OutputSafestLoad();
    updateMap();
}

void Map::OutputSafestLoad()//输出生命值最高的路径
{
    wstring str = L"";
    if (safestDist[this->nodeMaxSize - 1] >= 100)
    {
        str += L"不存在安全抵达终点的道路";
    }
    else
    {
        wstring* route = new wstring[GetNodeNum()];
        int len = 0;
        wstring sdata, edata, tdata;
        sdata = this->GetNodeName(0);
        edata = this->GetNodeName(this->nodeMaxSize - 1);
        route[len++] = edata;
        str += L"当前位置开始的生命值最高的路径是:";
        for (int j = this->nodeMaxSize - 1; safestPath[j] != -1; j = safestPath[j])
        {
            tdata = this->GetNodeName(safestPath[j]);
            route[len++] = tdata;
        }
        for (int i = len - 1; i >= 0; i--)
        {
            str += route[i];
            if (i > 0) str += L"->";
        }
        str += L"，最高生命值是";
        wostringstream oss;
        oss << 100 - safestDist[this->nodeMaxSize - 1];
        std::wstring wstr = oss.str();
        str += wstr;
        delete[]route;
    }
    settextcolor(BLACK);
    MessageBoxEx(GetHWnd(), str.c_str(), L"小心为上:"
        , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
    
}

//从源点v0开始找到财富最大且生命值不为零的路线
void Map::FindRichestLoad()
{
    resWealth = 0;//!!慎重，不初始化最大值将无法记录下一次结果
    richestPath = new int[nodeMaxSize];
    richestDist = new int[nodeMaxSize];
    int limit = 100;
    richestPath[0] = 0;
    int curHP = 100;
    int maxWealth = 0;
    int* tmpPath = new int[nodeMaxSize];
    tmpPath[0] = 0;
    DFS(1, 1, curHP, maxWealth, tmpPath);
    OutputRichestLoad(curHP);
    updateMap();
}

void Map::DFS(int in, int v, int& hp, int& maxWealth, int* tmpPath)
{
    //DFS+剪枝(回溯法)
    if (v == this->GetNodeNum() - 1)//到达终点，找到一个可行解
    {
        maxWealth += GetWealth(v);
        tmpPath[in] = v;
        if (maxWealth > resWealth)//判断是否为更优解
        {
            resWealth = maxWealth;
            for (int i = 0; i <= this->GetNodeNum() - 1; i++)
            {
                richestPath[i] = tmpPath[i];
            }
        }
        maxWealth -= GetWealth(v);
        return;
    }
    if (hp - GetRisk(tmpPath[in - 1], v) > 0)//确保前往下一座城堡时角色未死亡
    {
        SetVisitedTag(v, VISITED);
        maxWealth += GetWealth(v);
        tmpPath[in] = v;
        hp -= GetRisk(tmpPath[in - 1], v);
        for (int w = GetFirstAdjvex(v); w != -1; w = GetNextAdjvex(v, w))
            if (GetVisitedTag(w) == UNVISITED)
            {
                DFS(in + 1, w, hp, maxWealth, tmpPath);
                SetVisitedTag(w, UNVISITED);
            }
    }
    else
        return;
}

void Map::OutputRichestLoad(int curHP)const
{
    wstring str = L"";
    if (curHP <= 0)
        str += L"不存在安全抵达终点的道路";
    else
    {
        wstring* route = new wstring[nodeMaxSize];
        int len = 0;
        for (int j = 0; richestPath[j] >= 0 && richestPath[j] <= GetNodeNum() - 1; j++)
            route[len++] = this->GetNodeName(richestPath[j]);
        str += L"当前位置开始的财富值最高的路径是：\n";
        for (int i = 0; i <= len - 1; i++)
        {
            if (i > 0)
                str += L" -> ";
            str += route[i]; 
        }
        str += '\n';
        wostringstream oss1, oss2;
        oss1 << resWealth;
        oss2 << curHP;
        std::wstring wstrWV = oss1.str();
        std::wstring wstrHP = oss2.str();
        str += L"最高财富值是";
        str += wstrWV;
        str += L"，当前生命值是";
        str += wstrHP;
        delete[]route;
    }
    MessageBoxEx(GetHWnd(), str.c_str(), L"富贵险中求:"
        , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
}

Map::~Map()
{
    closegraph();
    delete[]nodes;
    delete[]lines;
}

void Map::updateMap()const
{
    for (int i = 0; i <= nodeMaxSize - 1; i++)
        SetVisitedTag(i, UNVISITED);
}
