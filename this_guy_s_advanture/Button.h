#pragma once
#include <graphics.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include "Button.h"
using namespace std;

//按钮设置
class Button
{
private:
    int x; // 按钮左上角x坐标
    int y; // 按钮左上角y坐标
    int width; // 按钮宽度
    int height; // 按钮高度
    int radius = 20;
    float scale; // 缩放比例，用于实现鼠标悬停效果
    bool isMouseOver; // 表示鼠标是否在按钮上方
    wstring text; // 按钮文本
    function<void()> onClick; // 点击按钮触发的函数

public:

    Button(int x, int y, int width, int height, const wstring& text, const function<void()>& onClick)
        : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
    {
    }
  

    // 检查鼠标是否在按钮上方
    void checkMouseOver(int mouseX, int mouseY)
    {
        isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

        if (isMouseOver) {
            scale = 0.95f; // 鼠标悬停时缩放按钮
        }
        else {
            scale = 1.0f; // 恢复按钮原始大小
        }
    }

    // 检查鼠标点击是否在按钮内，并执行函数
    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {
            onClick(); // 执行按钮点击时的函数
            isMouseOver = false;
            scale = 1.0f;
            return true;
        }
        return false;
    }

    // 绘制按钮
    void draw()
    {
        int scaledWidth = width * scale; // 缩放后的按钮宽度
        int scaledHeight = height * scale; // 缩放后的按钮高度
        int scaledX = x + (width - scaledWidth) / 2; // 缩放后的按钮x坐标
        int scaledY = y + (height - scaledHeight) / 2; // 缩放后的按钮y坐标

        if (isMouseOver)
        {
            setlinecolor(RGB(0, 120, 215)); // 鼠标悬停时按钮边框颜色
            setfillcolor(RGB(229, 241, 251)); // 鼠标悬停时按钮填充颜色

        }
        else
        {
            setlinecolor(RGB(173, 173, 173)); // 按钮边框颜色
            setfillcolor(RGB(225, 225, 225)); // 按钮填充颜色
        }
        solidroundrect(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight,10,10); // 绘制按钮
        settextcolor(BLACK); // 设置文本颜色为黑色
        setbkmode(TRANSPARENT); // 设置文本背景透明
        settextstyle(20 * scale, 0, _T("微软雅黑")); // 设置文本大小和字体
        //居中显示按钮文本
        int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2; // 计算文本在按钮中央的x坐标
        int textY = scaledY + (scaledHeight - textheight(_T("微软雅黑"))) / 2; // 计算文本在按钮中央的y坐标
        outtextxy(textX, textY, text.c_str()); // 在按钮上绘制文本
    }
};

