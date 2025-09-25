#include<iostream>
#include<graphics.h>
#include<easyx.h>
#include<conio.h>
#include<random>
#include<string>
#include<Windows.h>
#include<tchar.h>
#include<thread>
#include<chrono>
#include<atomic>
#include"TNT.h"

using namespace std;

void initpick();//函数initgame中需要调用在其之后的函数initpick 必须在其之前先声明
atomic<bool> timereset(true);//保证不同局游戏内时间计算不互相影响
bool mistake = false;//扫到雷判定

void sweep(TNT* sq, int rownum, int linenum, int &judgenum)
{
	if (!sq->ifsweep())return;
	//若不需要扫则跳过
	if (sq->ifbomb()) {
		sq->putbomb();
		mistake = true;
		return;
	}
	//若为雷则显示雷 返回
	else { 
		sq->putnum(judgenum);
	}
	//显示周围雷个数 标记为已打开
	if (sq->bnum() == 0) {

		if (sq->getxnum() == 0) {
			sweep(sq + linenum, rownum, linenum, judgenum);//扫右方雷
			if (sq->getynum() == 0) {
				sweep(sq + 1, rownum, linenum, judgenum);//扫下方
				sweep(sq + linenum + 1, rownum, linenum, judgenum);//扫右下
			}//y=0,同时在第一行
			else if (sq->getynum() == linenum-1) {
				sweep(sq - 1, rownum, linenum, judgenum);//扫上方
				sweep(sq + linenum-1, rownum, linenum, judgenum);//扫右上
			}//y=0,同时在末行
			else {
				sweep(sq - 1, rownum, linenum, judgenum);
				sweep(sq + linenum-1, rownum, linenum, judgenum);
				sweep(sq + 1, rownum, linenum, judgenum);
				sweep(sq + linenum+1, rownum, linenum, judgenum);
			}//在中间行全扫
		}
		//x=0,即空格在第一列时

		else if (sq->getxnum() == rownum-1) {
			sweep(sq - linenum, rownum, linenum, judgenum);//扫左方雷
			if (sq->getynum() == 0) {
				sweep(sq - linenum+1, rownum, linenum, judgenum);//扫左下
				sweep(sq + 1, rownum, linenum, judgenum);//扫下方
			}//y=0,同时在第一行
			else if (sq->getynum() == linenum-1) {
				sweep(sq - linenum-1, rownum, linenum, judgenum);//扫左上
				sweep(sq - 1, rownum, linenum, judgenum);//扫上方
			}//y=19,同时在末行
			else {
				sweep(sq - linenum-1, rownum, linenum, judgenum);
				sweep(sq - linenum+1, rownum, linenum, judgenum);
				sweep(sq - 1, rownum, linenum, judgenum);
				sweep(sq + 1, rownum, linenum, judgenum);
			}//在中间行全扫
		}
		//x=29,即空格在末列时

		else {
			if (sq->getynum() == 0) {
				sweep(sq - linenum, rownum, linenum, judgenum);//扫左方
				sweep(sq - linenum+1, rownum, linenum, judgenum);//扫左下
				sweep(sq + 1, rownum, linenum, judgenum);//扫下方
				sweep(sq + linenum, rownum, linenum, judgenum);//扫右方
				sweep(sq + linenum+1, rownum, linenum, judgenum);//扫右下
			}//y=0,同时在第一行时
			else if (sq->getynum() == linenum-1) {
				sweep(sq - linenum-1, rownum, linenum, judgenum);//扫左上
				sweep(sq - linenum, rownum, linenum, judgenum);//扫左方
				sweep(sq - 1, rownum, linenum, judgenum);//扫上方
				sweep(sq + linenum-1, rownum, linenum, judgenum);//扫右上
				sweep(sq + linenum, rownum, linenum, judgenum);//扫右方
			}//y=19,同时在末行时
			else {
				sweep(sq - linenum - 1, rownum, linenum, judgenum);//扫左上
				sweep(sq - linenum, rownum, linenum, judgenum);//扫左方
				sweep(sq - linenum + 1, rownum, linenum, judgenum);//扫左下
				sweep(sq - 1, rownum, linenum, judgenum);//扫上方
				sweep(sq + 1, rownum, linenum, judgenum);//扫下方
				sweep(sq + linenum - 1, rownum, linenum, judgenum);//扫右上
				sweep(sq + linenum, rownum, linenum, judgenum);//扫右方
				sweep(sq + linenum + 1, rownum, linenum, judgenum);//扫右下
			}//中间行中间列
		}
		//0<x<29,在中间列时

	}
	//若雷个数为0则递归扫雷
};
//左键单击扫单一雷  && 扫到空格递归扫周围九宫格

void KAI(TNT* sq,int rownum, int linenum,int &judgenum) {
	int n = 0;//个数置0 统计九宫格内已标记雷个数
	if (sq->getxnum() == 0) {

		if ((sq + linenum)->ifflag())n++;//判断右方是否标记

		if (sq->getynum() == 0) {

			if ((sq + 1)->ifflag())n++;//判断下方
			if ((sq + linenum+1)->ifflag())n++;//判断右下

			if (n == sq->bnum()) {
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
				if ((sq + linenum+1)->ifsweep())sweep(sq + linenum+1, rownum, linenum, judgenum);
			}//如果标记雷数目正确则扫 右方、下方、右下

			else {
				for (int i = 2; i > 0; i--) {

					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					//红色警示

					this_thread::sleep_for(chrono::milliseconds(400));
					//色彩停留 达到闪烁效果

					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					//抗锯齿

					this_thread::sleep_for(chrono::milliseconds(200));

				}
			}//不满足扫雷条件 周边红色警示

		}//同时在第一行时

		else if (sq->getynum() == linenum-1) {
			if ((sq - 1)->ifflag())n++;//判断上方
			if ((sq + linenum-1)->ifflag())n++;//判断右上
			if (n == sq->bnum()) {
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq + linenum-1)->ifsweep())sweep(sq + linenum-1, rownum, linenum, judgenum);
			}//如果标记雷数目正确则扫 右方、上方、右上
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//同时在末行

		else {
			if ((sq - 1)->ifflag())n++;//判断上方
			if ((sq + linenum-1)->ifflag())n++;//判断右上
			if ((sq + 1)->ifflag())n++;//判断下方
			if ((sq + linenum+1)->ifflag())n++;//判断右下
			if (n == sq->bnum()) {
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq + linenum-1)->ifsweep())sweep(sq + linenum-1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
				if ((sq + linenum+1)->ifsweep())sweep(sq + linenum+1, rownum, linenum, judgenum);
			}//如果标记雷数目正确则扫 右方、上方、右上、下方、右下
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//在中间行

	}
	//中心块在第一列时
	else if (sq->getxnum() == rownum-1) {
		if ((sq - linenum)->ifflag())n++;//判断左方

		if (sq->getynum() == 0) {
			if ((sq - linenum+1)->ifflag())n++;//判断左下
			if ((sq + 1)->ifflag())n++;//判断下方
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum+1)->ifsweep())sweep(sq - linenum+1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
			}//如果正确 扫左方、左下、下方
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//同时在第一行

		else if (sq->getynum() == linenum-1) {
			if ((sq - linenum-1)->ifflag())n++;//判断左上
			if ((sq - 1)->ifflag())n++;//判断上方
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum-1)->ifsweep())sweep(sq - linenum-1, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
			}//扫 左方、左上、上方
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//同时在末行

		else {
			if ((sq - linenum-1)->ifflag())n++;//判断左上
			if ((sq - 1)->ifflag())n++;//判断上方
			if ((sq - linenum+1)->ifflag())n++;//判断左下
			if ((sq + 1)->ifflag())n++;//判断下方
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum-1)->ifsweep())sweep(sq - linenum-1, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq - linenum+1)->ifsweep())sweep(sq - linenum+1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
			}//在中间行 扫左方、左上、上方、左下、下方
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//在中间行

	}
	//中心块在末列时
	else {
		if (sq->getynum() == 0) {
			if ((sq - linenum)->ifflag())n++;//左方
			if ((sq - linenum+1)->ifflag())n++;//左下
			if ((sq + 1)->ifflag())n++;//下方
			if ((sq + linenum)->ifflag())n++;//右方
			if ((sq + linenum+1)->ifflag())n++;//右下
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum+1)->ifsweep())sweep(sq - linenum+1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq + linenum+1)->ifsweep())sweep(sq + linenum+1, rownum, linenum, judgenum);
			}
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//同时在首行

		else if (sq->getynum() == linenum-1) {
			if ((sq - linenum-1)->ifflag())n++;//左上
			if ((sq - linenum)->ifflag())n++;//左方
			if ((sq - 1)->ifflag())n++;//上方
			if ((sq + linenum-1)->ifflag())n++;//右上
			if ((sq + linenum)->ifflag())n++;//右方
			if (n == sq->bnum()) {
				if ((sq - linenum-1)->ifsweep())sweep(sq - linenum-1, rownum, linenum, judgenum);
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq + linenum-1)->ifsweep())sweep(sq + linenum-1, rownum, linenum, judgenum);
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
			}
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//同时在末行

		else {
			if ((sq - linenum-1)->ifflag())n++;//左上
			if ((sq - linenum)->ifflag())n++;//左方
			if ((sq - linenum+1)->ifflag())n++;//左下
			if ((sq - 1)->ifflag())n++;//上方
			if ((sq + 1)->ifflag())n++;//下方
			if ((sq + linenum-1)->ifflag())n++;//右上
			if ((sq + linenum)->ifflag())n++;//右方
			if ((sq + linenum+1)->ifflag())n++;//右下
			if (n == sq->bnum()) {
				if ((sq - linenum-1)->ifsweep())sweep(sq - linenum-1, rownum, linenum, judgenum);
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum+1)->ifsweep())sweep(sq - linenum+1, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
				if ((sq + linenum-1)->ifsweep())sweep(sq + linenum-1, rownum, linenum, judgenum);
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq + linenum+1)->ifsweep())sweep(sq + linenum+1, rownum, linenum, judgenum);
			}
			else {
				for (int i = 2; i > 0; i--) {
					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					this_thread::sleep_for(chrono::milliseconds(400));
					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					this_thread::sleep_for(chrono::milliseconds(200));
				}
			}
		}//在中间行

	}
	//在中间列时
}
//左键双击扫九宫格

void puttime(chrono::high_resolution_clock::time_point st,int bklen,int origin) {
	//给函数传入程序开始时时间戳
	while (timereset) {
		chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
		//获得当时时间戳 类型为chrono命名空间中的······
		chrono::seconds duration = chrono::duration_cast<chrono::seconds>(current - st);
		//将两个时间戳差值转化为秒 类型为chrono命名空间中的seconds
		int sec = duration.count();
		//将seconds类型转化为int
		settextcolor(red);
		setbkmode(OPAQUE);
		setbkcolor(black);
		setfont(45, 27, "Digital-7");
		TCHAR buffer[80];
		if (sec < 10)
			sprintf_s(buffer, _T("000%d"), sec);
		else if (sec < 100)
			sprintf_s(buffer, _T("00%d"), sec);
		else if (sec < 1000)
			sprintf_s(buffer, _T("0%d"), sec);
		else if (sec < 10000)
			sprintf_s(buffer, _T("%d"), sec);
		outtextxy(bklen-130, 15-origin, buffer);
		this_thread::sleep_for(chrono::milliseconds(500));
		//设置线程休眠时间间隔
	}
	timereset = true;
}
//计时器

void initgame(int choice) {
	
	int bklen = 270;
	int bkwid = 270;
	int origin = 75;
	int rownum = 9;//此处指列(打错了)
	int linenum = 9;//行
	int bombnum = 10;
	int judgenum = 81;
	if (choice == 3) {
		bklen = 900;
		bkwid = 480;
		origin = 75;
		rownum = 30;
		linenum = 16;
		bombnum = 99;
		judgenum = rownum * linenum;
	}
	else if (choice == 2) {
		bklen = 480;
		bkwid = 480;
		origin = 75;
		rownum = 16;
		linenum = 16;
		bombnum = 40;
		judgenum = rownum * linenum;
	}
	else if (choice == 1) {
		bklen = 270;
		bkwid = 270;
		origin = 75;
		rownum = 9;
		linenum = 9;
		bombnum = 10;
		judgenum = rownum * linenum;
	}
	//规模选择

	initgraph(bklen, bkwid+origin, EX_DBLCLKS);
	setorigin(0, origin);
	setfillcolor(gray2);
	solidrectangle(0, -1*origin, bklen, 0);
	setfillcolor(gray2);
	solidrectangle(0, 0, bklen, bkwid);
	setlinestyle(PS_SOLID, 2, PS_JOIN_ROUND);
	setlinecolor(white);
	rectangle(1, 1-origin, bklen-1, -1);
	setlinecolor(gray4);
	line(1, -1, bklen-1, -1);
	line(bklen-1, 1-origin, bklen-1, -1);
	for (int i = 0; i < rownum; i++) {
		for (int j = 0; j < linenum; j++) {
			setfillcolor(gray2);
			solidcircle(i * 30 + 15, j * 30 + 15, 13);
			setlinecolor(white);
			arc(i * 30 + 1, j * 30 + 1, i * 30 + 29, j * 30 + 29, 0, 2 * pi);
			setlinecolor(gray4);
			arc(i * 30 + 1, j * 30 + 1, i * 30 + 29, j * 30 + 29, -0.75 * pi, 0.25 * pi);
		}
	}
	//创建背景

	/*设置每块地的坐标
	x:横坐标，表示列数，0~rownum，储存于一维i
	y:纵坐标，表示行数，0~linenum，储存于二维j*/
	TNT* sq = new TNT[rownum * linenum];//不定常量二维数组 可用动态数组声明 用乘积形式一维数组模拟二维数组
	for (int j = 0; j < linenum; j++) {//y范围
		for (int i = 0; i < rownum; i++) {//x范围
			sq[i * linenum + j].setloc(i, j);
		}
	}

	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<int>kai1(0, rownum-1);
	uniform_int_distribution<int>kai2(0, linenum-1);
	//随机数生成器

	int* temp = new int[bombnum];
	for (int n = 0; n < bombnum; n++) {
		int i = kai1(generator);
		int j = kai2(generator);
		temp[n] = i * 100 + j;
		int flag = 1;
		for (int m = 0; m < n; m++) {
			if (temp[m] == temp[n]) {
				n--;
				flag = 0;
				break;
			}
		}
		if (flag)sq[i * linenum + j].bchange();
	}
	//生成随机雷并埋藏

	for (int i = 0; i < rownum; i++) {
		for (int j = 0; j < linenum; j++) {
			if (sq[i * linenum + j].ifbomb()) {
				sq[i * linenum + j].bset();
				continue;
			}
			if (i == 0) {
				if (j == 0) {
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//首列首行
				else if (j == linenum-1) {
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
				}//首列末行
				else {
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//首列中间行
			}//首列
			else if (i == rownum-1) {
				if (j == 0) {
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//末列首行
				else if (j == linenum-1) {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
				}//末列末行
				else {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//末列中间行
			}//末列
			else {
				if (j == 0) {
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//中间列首行
				else if (j == linenum-1) {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
				}//中间列末行
				else {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//中间列中间行
			}//中间列
		}
	}
	//周围雷个数

	while (int first = 1) {
		int i = kai1(generator);
		int j = kai2(generator);
		if (sq[i * linenum + j].bnum() == 0) {
			sweep(&sq[i * linenum + j], rownum, linenum, judgenum);
			first = 0;
			break;
		}
	}
	//随机抽取无雷方块 保证首次打开方块九宫格内无雷

	COLORREF bkclr = getpixel(5, 5);//获取点颜色
	setbkmode(OPAQUE);
	setbkcolor(black);
	settextcolor(red);
	setfont(45, 27, "宋体");
	TCHAR numtotal[10];
	if (bombnum > 99)sprintf_s(numtotal, _T("%d"), bombnum);
	else if (bombnum > 9)sprintf_s(numtotal, _T("0%d"), bombnum);
	else if (bombnum >= 0)sprintf_s(numtotal, _T("00%d"), bombnum);
	outtextxy(20, 15-origin, numtotal);
	//雷个数初始化

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	thread thdtime(puttime, start, bklen, origin);
	thdtime.detach();//单独运行的分离线程

	int currentnumber = bombnum;
	mistake = false;
	ExMessage mes;

	while (1) {
		if (peekmessage(&mes, EX_MOUSE)) {
			if (!(mes.x >= 0 && mes.x <= bklen && mes.y >= origin && mes.y <= bkwid+origin))continue;
			int x_loc = mes.x - mes.x % 30;
			int y_loc = mes.y - origin - (mes.y - origin) % 30;
			int i = x_loc / 30;
			int j = y_loc / 30;

			if (mes.message == WM_LBUTTONUP) {
				if (sq[i * linenum + j].ifstat())KAI(&sq[i * linenum + j], rownum, linenum, judgenum);//未扫 扫单个
				else if (!sq[i * linenum + j].ifstat())sweep(&sq[i * linenum + j], rownum, linenum, judgenum);//已扫 快速扫九宫格
			}
			//单击左键扫雷

			else if (mes.message == WM_RBUTTONUP && !sq[i * linenum + j].ifstat()) {

				if (!sq[i * linenum + j].ifflag() && !sq[i * linenum + j].ifstat()) {
					sq[i * linenum + j].putflag(judgenum);
					currentnumber--;
				}//没标记也没打开 则标记
				else if (sq[i * linenum + j].ifflag() && !sq[i * linenum + j].ifstat()) {
					sq[i * linenum + j].putcover(judgenum);
					currentnumber++;
				}//标记了且没打开 则复原

				setbkcolor(black);
				settextcolor(red);
				setfont(45, 27, "宋体");
				TCHAR numlft[10];
				if (currentnumber > 99)sprintf_s(numlft, _T("%d"),currentnumber);
				else if (currentnumber > 9)sprintf_s(numlft, _T("0%d"), currentnumber);
				else if (currentnumber >= 0)sprintf_s(numlft, _T("00%d"), currentnumber);
				outtextxy(20, 15 - origin, numlft);
				//实时记录雷个数

			}
			//单击右键标记雷

			if (mistake) {

				timereset = false;//停止计时
				for (int k = 2; k > 0; k--) {

					setlinecolor(red);
					arc(sq[i * linenum + j].getxloc() + 1, sq[i * linenum + j].getyloc() + 1, sq[i * linenum + j].getxloc() + 29, sq[i * linenum + j].getyloc() + 29, 0, 2 * pi);
					//红色警示

					this_thread::sleep_for(chrono::milliseconds(400));
					//色彩停留 达到闪烁效果

					setlinecolor(white);
					arc(sq[i * linenum + j].getxloc() + 1, sq->getyloc() + 1, sq[i * linenum + j].getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq[i * linenum + j].getxloc() + 1, sq->getyloc() + 1, sq[i * linenum + j].getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					//抗锯齿

					this_thread::sleep_for(chrono::milliseconds(200));

				}
				break;

			}
			//游戏失败
			else if (judgenum == 0 && currentnumber == 0) {
				timereset = false;//停止计时
				this_thread::sleep_for(chrono::milliseconds(2000));
				break;
			}
			//游戏成功

		}
	}

	delete[] sq;
	initpick();

}
//一次游戏进程

void initpick() {

	initgraph(600, 250);
	setfillcolor(gray2);
	fillrectangle(0, 0, 600, 250);
	setbkcolor(gray2);
	settextcolor(blue);
	setfont(40, 20, "宋体");
	outtextxy(50, 55, "基础");
	outtextxy(250, 55, "中级");
	outtextxy(450, 55, "专家");
	settextcolor(red);
	outtextxy(50, 160, "设置");
	outtextxy(250, 160, "成绩");
	outtextxy(450, 160, "退出");
	setlinestyle(PS_SOLID, 2, PS_JOIN_ROUND);
	for (int i = 0; i < 2; i++) {
		setlinecolor(white);
		for (int j = 0; j < 3; j++) {
			rectangle(30 + j * 200, 45 + i * 105, 150 + j * 200, 105 + i * 105);
		}
		setlinecolor(black);
		for (int j = 0; j < 3; j++) {
			line(30 + j * 200, 105 + i * 105, 150 + j * 200, 105 + i * 105);
			line(150 + j * 200, 45 + i * 105, 150 + j * 200, 105 + i * 105);
		}
	}

	ExMessage pick;
	while (1) {
		if (peekmessage(&pick, EX_MOUSE)) {
			if (pick.message == WM_LBUTTONUP && pick.x >= 30 && pick.x <= 150 && pick.y >= 45 && pick.y <= 105) {
				closegraph();
				initgame(1);
			}//基础
			else if (pick.message == WM_LBUTTONUP && pick.x >= 230 && pick.x <= 350 && pick.y >= 45 && pick.y <= 105) {
				closegraph();
				initgame(2);
			}//中级
			else if (pick.message == WM_LBUTTONUP && pick.x >= 430 && pick.x <= 550 && pick.y >= 45 && pick.y <= 105) {
				closegraph();
				initgame(3);
			}//专家
			else if (pick.message == WM_LBUTTONUP && pick.x >= 30 && pick.x <= 150 && pick.y >= 150 && pick.y <= 210) {

			}//设置
			else if (pick.message == WM_LBUTTONUP && pick.x >= 230 && pick.x <= 350 && pick.y >= 150 && pick.y <= 210) {

			}//成绩
			else if (pick.message == WM_LBUTTONUP && pick.x >= 430 && pick.x <= 550 && pick.y >= 150 && pick.y <= 210) {
				closegraph();
				break;
			}//退出
		}
	}

}
//一次选择进程

int main()
{

	initpick();

	return 0;
};

/*待解决：
* 何时停止扫雷
* 不同主题背景与图标
* 音效
* 自动模式
* 点雷玩法
* 
* 鼠标滚轮放大缩小
*/