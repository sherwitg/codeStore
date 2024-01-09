#pragma once
#include"headFile.h"
#include "Status.h"
#include "Line.h"
#include "Node.h"
#include"Knight.h"
#include"Button.h"
#include"Map.h"
#include"TextBox.h"
#include"draw.h"
#include<vector>
#include <graphics.h>


using namespace std;

//界面索引(0-1)说明：
// 0-主菜单 1-开始游戏界面

// 定义Widget类表示图形用户界面

class Widget
{
private:

    IMAGE* page1;
    IMAGE* page2;
    Button* button1_1;
    Button* button1_2;
    Button* button2_1;
    Button* button2_2;
    Button* button2_3;
    Button* button2_4;
    TextBox* tbox;

    int width, height;
    int currentIndex;       // 当前页面索引
    wstring propos = L"A";  //初始化此前所在位置propos为A
    Map* map_ptr;
    Knight* knight_ptr;
    Map m1;
    Knight k1;
    vector<IMAGE*> pages; // 存储所有页面的图片指针
    vector<vector<Button*>> buttons; // 存储每个页面上的按钮
    vector<vector<TextBox*> >textBoxs;//存储每个页面上的文本框
    // 添加一个页面
    void addPage(IMAGE* page)
    {
        pages.push_back(page);
        buttons.push_back({});
        textBoxs.push_back({});
    }

    // 在指定页面上添加一个按钮
    void addButton(int index, Button* button)
    {
        if (index >= 0 && index < buttons.size())
        {
            buttons[index].push_back(button);
        }
    }

    //添加文本框
    void addTextBox(int index, TextBox* textBox)
    {
        if (index >= 0 && index < textBoxs.size())
        {
            textBoxs[index].push_back(textBox);
        }
    }

    // 设置当前显示的页面索引
    void setCurrentIndex(int index)
    {
        if (index >= 0 && index < pages.size())
        {
            currentIndex = index;
        }
    }

    // 处理鼠标点击事件
    void mouseClick(int mouseX, int mouseY)
    {
        //按钮
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                if (button->checkClick(mouseX, mouseY))
                {
                    break;
                }
            }
        }
        //文本框
        if (currentIndex >= 0 && currentIndex < textBoxs.size())
        {
            for (auto textbox : textBoxs[currentIndex])
            {
                textbox->checkClick(mouseX, mouseY);
            }
        }
    }

    // 处理鼠标移动事件
    void mouseMove(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                button->checkMouseOver(mouseX, mouseY);
            }
        }
    }

    // 处理键盘输入事件
    void keyInput(wchar_t ch)
    {      
        if (currentIndex >= 0 && currentIndex < textBoxs.size())
            //遍历textBoxs[currentIndex]中的每一个元素，并将每个元素赋值给变量textbox
            for (auto textbox : textBoxs[currentIndex])
            {
                if (textbox->getSelect())
                {
                    //判断是否按下回车键
                    if (!textbox->keyInput(ch))continue;
                    //获取当前文字内容
                    wstring nextpos = textbox->getText();
                    //清除文本框内容
                    textbox->clear();

                    //判断输入正确与否
                    if (nextpos == propos)      //判断是否是同一个点
                    {
                        MessageBoxEx(GetHWnd(), L"你已抵达该城堡", L"提示:"
                            , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
                        continue;
                    }
                    else if (m1.GetRisk(m1.GetNodeIndex(propos), m1.GetNodeIndex(nextpos)) == 0x3f3f3f3f)
                    {
                        MessageBoxEx(GetHWnd(), L"不存在该路径,请重新输入", L"提示:"
                            , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
                        continue;
                    }

                    int e;
                    e = m1.GetNodeIndex(nextpos);
                    int ans = k1.goNext(m1.GetRisk(k1.GetCurPos(), e), m1.GetWealth(e), e);
                    if (ans == -1)//游戏失败
                    {
                        k1.Display(BLACK);  //展示当前角色状况
                        putimage(0, 0, m1.before);
                        k1.UpdateGame();    //刷新角色信息
                        create_Map();
                        propos = L"A";
                        k1.Display(BLACK);
                        getimage(pages[1], 0, 0, 800, 600);
                    }
                    else if (ans == 1)//游戏成功
                    {
                        m1.redraw2(m1.GetNodeIndex(nextpos));
                        putimage(0, 0, m1.before);
                        k1.Display(BLACK);//展示当前角色状况
                        getimage(pages[1], 0, 0, 800, 600);
                        propos = nextpos;
                        draw();
                        FlushBatchDraw();
                       
                        MessageBoxEx(GetHWnd(), L"           游戏胜利!\n", L"游戏结果:"
                            , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

                        //刷新游戏
                        putimage(0, 0, m1.before);
                        k1.UpdateGame();    //刷新角色信息
                        create_Map();       //刷新地图信息
                        propos = L"A";
                        k1.Display(BLACK);
                        getimage(pages[1], 0, 0, 800, 600);
                    }
                    else//游戏继续
                    {
                        m1.redraw2(m1.GetNodeIndex(nextpos));
                        putimage(0, 0, m1.before);
                        k1.Display(BLACK);//展示当前角色状况
                        getimage(pages[1], 0, 0, 800, 600);
                        propos = nextpos;
                    }

                }
            }
    }

    // 绘制当前页面的内容(文本框和按钮)
    void draw()
    {
        if (currentIndex >= 0 && currentIndex < pages.size())//页面索引取值范围判断
        {
            putimage(0, 0, pages[currentIndex]); // 在窗口中绘制当前页面的图片

            if (currentIndex >= 0 && currentIndex < buttons.size())
            {
                for (Button* button : buttons[currentIndex])
                {
                    button->draw(); // 绘制当前页面上的所有按钮
                }
            }
            if (currentIndex >= 0 && currentIndex < textBoxs.size())
            {
                for (auto textbox : textBoxs[currentIndex])
                {
                    if (textbox->getSelect()) textbox->updateCursor();
                    textbox->draw();// 绘制当前页面上的所有文本框
                }
            }
        }

    }

    void create_Map()
    {
        IMAGE img;
        loadimage(&img, _T("background.jpg"));  // 请确保 "background.jpg" 在您的项目目录中  

        // 设置窗口背景  
        //setbkimage(&img);

        //创建世界地图
        map_ptr = new Map;
        m1 = *map_ptr;
        if (!m1.CreateMap(L"map.txt"))
        {
            initgraph(widgetLen, widgetWid);
            TCHAR ss[] = _T("游戏地图加载失败");
            setbkmode(TRANSPARENT);                 //预设置透明背景模式
            settextcolor(WHITE);                    //设置文本颜色
            write(widgetLen / 2, widgetWid / 2, ss);//居中显示文本
            // 暂停并等待用户输入任意键退出  
            char ch = _getch();                     // 获取一个字符输入  
            closegraph();                           // 关闭图形模式  
            exit(-1);
        }
        //绘制游戏地图
        m1.ShowCurMap();
        //给起点重新染色
        m1.redraw(0);
        wchar_t str[] = L"选择将前往的下一个城堡: ";
        settextstyle(15, 0, _T("Arial Rounded MT Bold"));
        settextcolor(BLACK);
        write(widgetLen / 2, widgetWid / 2 + 100, str);
        getimage(m1.before, 0, 0, widgetLen, widgetWid);
    }
   
public:
    Widget(int width, int height)
        :width(width), height(height), currentIndex(-1)
    {}
    ~Widget() {}

    
    void init()// 初始化控件
    {
        //初始化图形环境
        initgraph(widgetLen, widgetWid);

        //设置窗口信息
        HWND hwnd = GetHWnd();
        SetWindowPos(hwnd, NULL, 365, 200, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        SetWindowText(hwnd, L"勇士闯关");
        setbkmode(TRANSPARENT);                             //预设置透明背景模式
        BeginBatchDraw();

        // 创建页面1(勇士闯关主界面)
        page1 = new IMAGE(widgetLen, widgetWid);            //也可以用loadimage()函数加载图片
        setbkcolor(WHITE);                                  // 设置背景颜色
        cleardevice();
        wchar_t str[] = L"勇士闯关小游戏";
        settextstyle(30, 0, _T("Arial Rounded MT Bold"));   //字体高度为50  宽度自适应
        settextcolor(BLACK);
        write(widgetLen / 2, widgetWid / 2 - 150, str);
        getimage(page1, 0, 0, widgetLen, widgetWid);        // 将页面1的内容保存到图片中
        addPage(page1);                                     // 添加页面1
        button1_1 = new Button(400, 250, 100, 50, L"开始游戏", [&]() {
            setCurrentIndex(1);                             // 点击按钮1时，切换到页面2
            putimage(0, 0, m1.before);
            k1.Display(BLACK);                              //显示角色信息   
            getimage(pages[1], 0, 0, widgetLen, widgetWid); //获取图像数据
            FlushBatchDraw();});
        button1_2 = new Button(400, 350, 100, 50, L"退出游戏", [&]() {exit(0); });
        addButton(0, button1_1);                            // 将按钮1添加到页面1
        addButton(0, button1_2);                            // 将按钮2添加到页面1

        page2 = new IMAGE(widgetLen, widgetWid);
        getimage(page2, 0, 0, widgetLen, widgetWid);        // 将页面2的内容保存到图片中
        addPage(page2);                                     // 添加页面2
        button2_1 = new Button(45, 150, 100, 50, L"小心为上", [&]() {m1.FindSafestLoad();});
        button2_2 = new Button(45, 250, 100, 50, L"富贵险中求", [&]() {m1.FindRichestLoad();});
        button2_3 = new Button(45, 350, 100, 50, L"刷新地图", [&]() {
            putimage(0, 0, m1.before);
            k1.UpdateGame();    //刷新角色信息
            create_Map();
            propos = L"A";
            k1.Display(BLACK);
            getimage(pages[1], 0, 0, widgetLen, widgetWid);
            draw();
            FlushBatchDraw(); 
            });
        button2_4 = new Button(45, 450, 100, 50, L"返回主菜单", [&]() {
            putimage(0, 0, m1.before);
            k1.UpdateGame();    //刷新角色信息
            create_Map();
            propos = L"A";
            k1.Display(BLACK);
            getimage(pages[1], 0, 0, widgetLen, widgetWid);
            draw();
            FlushBatchDraw();
            setCurrentIndex(0);});
        TextBox* tbox = new TextBox(450, 430, 80, 25, 20);  //在游戏界面创建文本框
        addTextBox(1, tbox);                                //向表单添加一个文本框控件
        addButton(1, button2_1);                            // 将按钮3添加到页面3
        addButton(1, button2_2);                            // 将按钮1添加到页面2
        addButton(1, button2_3);                            // 将按钮2添加到页面3
        addButton(1, button2_4);                            // 将按钮2添加到页面3
        setCurrentIndex(0);                                 // 设置初始显示页面为页面1

        //创建地图
        create_Map();
    }

    // 运行进入消息循环
    void run()
    {
        ExMessage msg;                          //定义消息变量

        BeginBatchDraw();                       // 开始批量绘制

        while (true)
        {
            if (peekmessage(&msg))              // 检查是否有消息
            {
                int mouseX = msg.x;             // 获取鼠标x坐标
                int mouseY = msg.y;             // 获取鼠标y坐标

                switch (msg.message)
                {
                case WM_LBUTTONDOWN:            // 鼠标左键按下事件
                    mouseClick(mouseX, mouseY); // 处理鼠标点击事件
                    break;
                case WM_MOUSEMOVE:              // 鼠标移动事件
                    mouseMove(mouseX, mouseY);  // 处理鼠标移动事件
                    break;
                case WM_CHAR:                   //键盘输入事件
                    keyInput(msg.ch);           //处理键盘输入的字符
                    break;
                }
            }
            draw(); // 绘制当前页面内容
            FlushBatchDraw(); // 将缓冲区内容显示在屏幕上
            //Sleep(10);
        }
        EndBatchDraw(); // 结束批量绘制
    }
    // 关闭
    void close()
    {
        closegraph(); // 关闭图形环境
    }
};
