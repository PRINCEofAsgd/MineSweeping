#pragma once
#include<iostream>
#include<graphics.h>
#include<easyx.h>
#include<conio.h>
#include<random>
#include<string>
#include<Windows.h>
#include<tchar.h>

float pi = 3.1415926;
const COLORREF gray1 = RGB(240, 240, 240);
const COLORREF gray2 = RGB(200, 200, 200);
const COLORREF gray3 = RGB(160, 160, 160);
const COLORREF gray4 = RGB(128, 128, 128);
const COLORREF gray5 = RGB(90, 90, 90);
const COLORREF black = RGB(0, 0, 0);
const COLORREF white = RGB(255, 255, 255);
const COLORREF blue = RGB(28, 28, 255);
const COLORREF green = RGB(0, 187, 0);
const COLORREF red = RGB(255, 0, 0);
const COLORREF purple = RGB(159, 53, 255);
const COLORREF brown = RGB(132, 43, 0);
const COLORREF indigo = RGB(0, 202, 202);
//颜色常量初始化

class TNT
{

	int x_num = 0;
	int y_num = 0;//标号(1为单位)
	int x_loc = 0;
	int y_loc = 0;//坐标(图片大小为单位)
	bool bomb = false;//是否有雷
	bool flag = false;//是否插旗标记
	bool stat = false;//是否已打开
	int num = 0;//周围雷个数
	IMAGE bpicture;
	IMAGE fpicture;
	IMAGE cover;

public:

	TNT() {

	}

	int getxnum() { return x_num; }
	int getynum() { return y_num; }
	int getxloc() { return x_loc; }
	int getyloc() { return y_loc; }
	int bnum() { return num; }

	bool ifbomb() {
		if (bomb == true)return true;
		else return false;
	}//判断是否为雷
	bool ifflag() {
		if (flag == true)return true;
		else return false;
	}//判断是否插旗标记
	bool ifstat() {
		if (stat == true)return true;
		else return false;
	}//判断是否已打开
	bool ifsweep() {
		if (stat == false && flag == false)return true;
		else return false;
	}//没打开 也没标记 返回true 表示需要扫
	bool ifmistake() {
		if (stat == true && bomb == true)return true;
	}//打开雷 返回true 表示错误

	void bplus() { num++; }
	void bset() { num = 9; }
	void bchange() {
		if (bomb == true)bomb = false;
		else bomb = true;
	}//改变埋雷状态
	void fchange() {
		if (flag == false)flag = true;
		else flag = false;
	}//改变插旗状态
	void schange() {
		if (stat == false)stat = true;
		else stat = false;
	}//改变打开状态
	void setloc(int x, int y) {
		x_num = x;
		y_num = y;
		x_loc = x * 30;
		y_loc = y * 30;
	}//设置坐标

	void putnum(int &judgenum) {

		stat = true;//标记为已打开
		judgenum--;

		setfillcolor(gray1);
		solidcircle(x_loc + 15, y_loc + 15, 13);

		switch (bnum()) {
		case 0:
			settextcolor(gray1);
			break;
		case 1:
			settextcolor(RGB(28, 28, 255));
			break;
		case 2:
			settextcolor(RGB(0, 187, 0));
			break;
		case 3:
			settextcolor(RGB(255, 0, 0));
			break;
		case 4:
			settextcolor(RGB(159, 53, 255));
			break;
		case 5:
			settextcolor(RGB(132, 43, 0));
			break;
		case 6:
			settextcolor(RGB(0, 202, 202));
			break;
		case 9:
			settextcolor(RGB(0, 0, 0));
			break;
		default:
			settextcolor(RGB(0, 0, 0));
			break;
		}
		//根据周围雷个数设置显示数字的颜色

		setbkmode(TRANSPARENT);//背景透明
		setfont(25, 15, "宋体");//大小和字体

		try {
			TCHAR buffer[10];//可兼容字节长度的字符类型
			int result = sprintf_s(buffer, _T("%d"), bnum());
			//_T("%d")表示编码格式，将整数bnum()转换为TCHAR字符串储存在buffer中，返回正确写入的字符数存入result
			if (result < 0) {
				// 如果返回值小于 0，表示发生错误，抛出异常
				throw std::runtime_error("sprintf_s failed");
			}
			LPCTSTR temp = buffer;//声明指向TCHAR的指针
			outtextxy(x_loc + 8, y_loc + 3, temp);//在指定位置输出内容
		}
		catch (const std::exception& e) {
			// 捕获并处理异常，输出错误信息
			std::cerr << "Exception caught: " << e.what() << std::endl;
		}

	}
	//显示周围雷个数 设置状态为已打开 判定数-1

	void putbomb() {

		stat = true;
		flag = true;

		loadimage(&bpicture, "bomb1.jpg", 30, 30);
		putimage(x_loc , y_loc , &bpicture);

	}
	//显示炸弹 设置状态为已打开 游戏状态置1

	void putflag(int &judgenum) {

		flag = true;
		judgenum--;

		setfillcolor(gray2);
		solidcircle(x_loc + 15, y_loc + 15, 13);
		//旗帜背景色
		loadimage(&fpicture, "flag3.jpg", 16, 16);
		putimage(x_loc + 7, y_loc + 7, &fpicture);
		//显示旗帜
		setlinecolor(white);
		arc(x_loc + 1, y_loc + 1, x_loc + 29, y_loc + 29, 0, 2 * pi);
		setlinecolor(gray4);
		arc(x_loc + 1, y_loc + 1, x_loc + 29, y_loc + 29, -0.75 * pi, 0.25 * pi);
		//抗锯齿色差
	}
	//显示旗帜 设置状态为已标记 判定数-1

	void putcover(int &judgenum) {

		flag = false;
		stat = false;
		judgenum++;

		setfillcolor(gray2);
		solidcircle(x_loc + 15, y_loc + 15, 13);
		//cover颜色
		setlinecolor(white);
		arc(x_loc + 1, y_loc + 1, x_loc + 29, y_loc + 29, 0, 2 * pi);
		setlinecolor(gray4);
		arc(x_loc + 1, y_loc + 1, x_loc + 29, y_loc + 29, -0.75 * pi, 0.25 * pi);
		//抗锯齿

	}
	//取消旗帜 设置状态为未标记 判定数+1

};
