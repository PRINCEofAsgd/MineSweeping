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

void initpick();//����initgame����Ҫ��������֮��ĺ���initpick ��������֮ǰ������
atomic<bool> timereset(true);//��֤��ͬ����Ϸ��ʱ����㲻����Ӱ��
bool mistake = false;//ɨ�����ж�

void sweep(TNT* sq, int rownum, int linenum, int &judgenum)
{
	if (!sq->ifsweep())return;
	//������Ҫɨ������
	if (sq->ifbomb()) {
		sq->putbomb();
		mistake = true;
		return;
	}
	//��Ϊ������ʾ�� ����
	else { 
		sq->putnum(judgenum);
	}
	//��ʾ��Χ�׸��� ���Ϊ�Ѵ�
	if (sq->bnum() == 0) {

		if (sq->getxnum() == 0) {
			sweep(sq + linenum, rownum, linenum, judgenum);//ɨ�ҷ���
			if (sq->getynum() == 0) {
				sweep(sq + 1, rownum, linenum, judgenum);//ɨ�·�
				sweep(sq + linenum + 1, rownum, linenum, judgenum);//ɨ����
			}//y=0,ͬʱ�ڵ�һ��
			else if (sq->getynum() == linenum-1) {
				sweep(sq - 1, rownum, linenum, judgenum);//ɨ�Ϸ�
				sweep(sq + linenum-1, rownum, linenum, judgenum);//ɨ����
			}//y=0,ͬʱ��ĩ��
			else {
				sweep(sq - 1, rownum, linenum, judgenum);
				sweep(sq + linenum-1, rownum, linenum, judgenum);
				sweep(sq + 1, rownum, linenum, judgenum);
				sweep(sq + linenum+1, rownum, linenum, judgenum);
			}//���м���ȫɨ
		}
		//x=0,���ո��ڵ�һ��ʱ

		else if (sq->getxnum() == rownum-1) {
			sweep(sq - linenum, rownum, linenum, judgenum);//ɨ����
			if (sq->getynum() == 0) {
				sweep(sq - linenum+1, rownum, linenum, judgenum);//ɨ����
				sweep(sq + 1, rownum, linenum, judgenum);//ɨ�·�
			}//y=0,ͬʱ�ڵ�һ��
			else if (sq->getynum() == linenum-1) {
				sweep(sq - linenum-1, rownum, linenum, judgenum);//ɨ����
				sweep(sq - 1, rownum, linenum, judgenum);//ɨ�Ϸ�
			}//y=19,ͬʱ��ĩ��
			else {
				sweep(sq - linenum-1, rownum, linenum, judgenum);
				sweep(sq - linenum+1, rownum, linenum, judgenum);
				sweep(sq - 1, rownum, linenum, judgenum);
				sweep(sq + 1, rownum, linenum, judgenum);
			}//���м���ȫɨ
		}
		//x=29,���ո���ĩ��ʱ

		else {
			if (sq->getynum() == 0) {
				sweep(sq - linenum, rownum, linenum, judgenum);//ɨ��
				sweep(sq - linenum+1, rownum, linenum, judgenum);//ɨ����
				sweep(sq + 1, rownum, linenum, judgenum);//ɨ�·�
				sweep(sq + linenum, rownum, linenum, judgenum);//ɨ�ҷ�
				sweep(sq + linenum+1, rownum, linenum, judgenum);//ɨ����
			}//y=0,ͬʱ�ڵ�һ��ʱ
			else if (sq->getynum() == linenum-1) {
				sweep(sq - linenum-1, rownum, linenum, judgenum);//ɨ����
				sweep(sq - linenum, rownum, linenum, judgenum);//ɨ��
				sweep(sq - 1, rownum, linenum, judgenum);//ɨ�Ϸ�
				sweep(sq + linenum-1, rownum, linenum, judgenum);//ɨ����
				sweep(sq + linenum, rownum, linenum, judgenum);//ɨ�ҷ�
			}//y=19,ͬʱ��ĩ��ʱ
			else {
				sweep(sq - linenum - 1, rownum, linenum, judgenum);//ɨ����
				sweep(sq - linenum, rownum, linenum, judgenum);//ɨ��
				sweep(sq - linenum + 1, rownum, linenum, judgenum);//ɨ����
				sweep(sq - 1, rownum, linenum, judgenum);//ɨ�Ϸ�
				sweep(sq + 1, rownum, linenum, judgenum);//ɨ�·�
				sweep(sq + linenum - 1, rownum, linenum, judgenum);//ɨ����
				sweep(sq + linenum, rownum, linenum, judgenum);//ɨ�ҷ�
				sweep(sq + linenum + 1, rownum, linenum, judgenum);//ɨ����
			}//�м����м���
		}
		//0<x<29,���м���ʱ

	}
	//���׸���Ϊ0��ݹ�ɨ��
};
//�������ɨ��һ��  && ɨ���ո�ݹ�ɨ��Χ�Ź���

void KAI(TNT* sq,int rownum, int linenum,int &judgenum) {
	int n = 0;//������0 ͳ�ƾŹ������ѱ���׸���
	if (sq->getxnum() == 0) {

		if ((sq + linenum)->ifflag())n++;//�ж��ҷ��Ƿ���

		if (sq->getynum() == 0) {

			if ((sq + 1)->ifflag())n++;//�ж��·�
			if ((sq + linenum+1)->ifflag())n++;//�ж�����

			if (n == sq->bnum()) {
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
				if ((sq + linenum+1)->ifsweep())sweep(sq + linenum+1, rownum, linenum, judgenum);
			}//����������Ŀ��ȷ��ɨ �ҷ����·�������

			else {
				for (int i = 2; i > 0; i--) {

					setlinecolor(red);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					//��ɫ��ʾ

					this_thread::sleep_for(chrono::milliseconds(400));
					//ɫ��ͣ�� �ﵽ��˸Ч��

					setlinecolor(white);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq->getxloc() + 1, sq->getyloc() + 1, sq->getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					//�����

					this_thread::sleep_for(chrono::milliseconds(200));

				}
			}//������ɨ������ �ܱߺ�ɫ��ʾ

		}//ͬʱ�ڵ�һ��ʱ

		else if (sq->getynum() == linenum-1) {
			if ((sq - 1)->ifflag())n++;//�ж��Ϸ�
			if ((sq + linenum-1)->ifflag())n++;//�ж�����
			if (n == sq->bnum()) {
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq + linenum-1)->ifsweep())sweep(sq + linenum-1, rownum, linenum, judgenum);
			}//����������Ŀ��ȷ��ɨ �ҷ����Ϸ�������
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
		}//ͬʱ��ĩ��

		else {
			if ((sq - 1)->ifflag())n++;//�ж��Ϸ�
			if ((sq + linenum-1)->ifflag())n++;//�ж�����
			if ((sq + 1)->ifflag())n++;//�ж��·�
			if ((sq + linenum+1)->ifflag())n++;//�ж�����
			if (n == sq->bnum()) {
				if ((sq + linenum)->ifsweep())sweep(sq + linenum, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq + linenum-1)->ifsweep())sweep(sq + linenum-1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
				if ((sq + linenum+1)->ifsweep())sweep(sq + linenum+1, rownum, linenum, judgenum);
			}//����������Ŀ��ȷ��ɨ �ҷ����Ϸ������ϡ��·�������
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
		}//���м���

	}
	//���Ŀ��ڵ�һ��ʱ
	else if (sq->getxnum() == rownum-1) {
		if ((sq - linenum)->ifflag())n++;//�ж���

		if (sq->getynum() == 0) {
			if ((sq - linenum+1)->ifflag())n++;//�ж�����
			if ((sq + 1)->ifflag())n++;//�ж��·�
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum+1)->ifsweep())sweep(sq - linenum+1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
			}//�����ȷ ɨ�󷽡����¡��·�
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
		}//ͬʱ�ڵ�һ��

		else if (sq->getynum() == linenum-1) {
			if ((sq - linenum-1)->ifflag())n++;//�ж�����
			if ((sq - 1)->ifflag())n++;//�ж��Ϸ�
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum-1)->ifsweep())sweep(sq - linenum-1, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
			}//ɨ �󷽡����ϡ��Ϸ�
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
		}//ͬʱ��ĩ��

		else {
			if ((sq - linenum-1)->ifflag())n++;//�ж�����
			if ((sq - 1)->ifflag())n++;//�ж��Ϸ�
			if ((sq - linenum+1)->ifflag())n++;//�ж�����
			if ((sq + 1)->ifflag())n++;//�ж��·�
			if (n == sq->bnum()) {
				if ((sq - linenum)->ifsweep())sweep(sq - linenum, rownum, linenum, judgenum);
				if ((sq - linenum-1)->ifsweep())sweep(sq - linenum-1, rownum, linenum, judgenum);
				if ((sq - 1)->ifsweep())sweep(sq - 1, rownum, linenum, judgenum);
				if ((sq - linenum+1)->ifsweep())sweep(sq - linenum+1, rownum, linenum, judgenum);
				if ((sq + 1)->ifsweep())sweep(sq + 1, rownum, linenum, judgenum);
			}//���м��� ɨ�󷽡����ϡ��Ϸ������¡��·�
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
		}//���м���

	}
	//���Ŀ���ĩ��ʱ
	else {
		if (sq->getynum() == 0) {
			if ((sq - linenum)->ifflag())n++;//��
			if ((sq - linenum+1)->ifflag())n++;//����
			if ((sq + 1)->ifflag())n++;//�·�
			if ((sq + linenum)->ifflag())n++;//�ҷ�
			if ((sq + linenum+1)->ifflag())n++;//����
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
		}//ͬʱ������

		else if (sq->getynum() == linenum-1) {
			if ((sq - linenum-1)->ifflag())n++;//����
			if ((sq - linenum)->ifflag())n++;//��
			if ((sq - 1)->ifflag())n++;//�Ϸ�
			if ((sq + linenum-1)->ifflag())n++;//����
			if ((sq + linenum)->ifflag())n++;//�ҷ�
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
		}//ͬʱ��ĩ��

		else {
			if ((sq - linenum-1)->ifflag())n++;//����
			if ((sq - linenum)->ifflag())n++;//��
			if ((sq - linenum+1)->ifflag())n++;//����
			if ((sq - 1)->ifflag())n++;//�Ϸ�
			if ((sq + 1)->ifflag())n++;//�·�
			if ((sq + linenum-1)->ifflag())n++;//����
			if ((sq + linenum)->ifflag())n++;//�ҷ�
			if ((sq + linenum+1)->ifflag())n++;//����
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
		}//���м���

	}
	//���м���ʱ
}
//���˫��ɨ�Ź���

void puttime(chrono::high_resolution_clock::time_point st,int bklen,int origin) {
	//�������������ʼʱʱ���
	while (timereset) {
		chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
		//��õ�ʱʱ��� ����Ϊchrono�����ռ��еġ�����������
		chrono::seconds duration = chrono::duration_cast<chrono::seconds>(current - st);
		//������ʱ�����ֵת��Ϊ�� ����Ϊchrono�����ռ��е�seconds
		int sec = duration.count();
		//��seconds����ת��Ϊint
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
		//�����߳�����ʱ����
	}
	timereset = true;
}
//��ʱ��

void initgame(int choice) {
	
	int bklen = 270;
	int bkwid = 270;
	int origin = 75;
	int rownum = 9;//�˴�ָ��(�����)
	int linenum = 9;//��
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
	//��ģѡ��

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
	//��������

	/*����ÿ��ص�����
	x:�����꣬��ʾ������0~rownum��������һάi
	y:�����꣬��ʾ������0~linenum�������ڶ�άj*/
	TNT* sq = new TNT[rownum * linenum];//����������ά���� ���ö�̬�������� �ó˻���ʽһά����ģ���ά����
	for (int j = 0; j < linenum; j++) {//y��Χ
		for (int i = 0; i < rownum; i++) {//x��Χ
			sq[i * linenum + j].setloc(i, j);
		}
	}

	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<int>kai1(0, rownum-1);
	uniform_int_distribution<int>kai2(0, linenum-1);
	//�����������

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
	//��������ײ����

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
				}//��������
				else if (j == linenum-1) {
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
				}//����ĩ��
				else {
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//�����м���
			}//����
			else if (i == rownum-1) {
				if (j == 0) {
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//ĩ������
				else if (j == linenum-1) {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
				}//ĩ��ĩ��
				else {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//ĩ���м���
			}//ĩ��
			else {
				if (j == 0) {
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//�м�������
				else if (j == linenum-1) {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
				}//�м���ĩ��
				else {
					if (sq[(i - 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i - 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[i * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j - 1].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j].ifbomb())sq[i * linenum + j].bplus();
					if (sq[(i + 1) * linenum + j + 1].ifbomb())sq[i * linenum + j].bplus();
				}//�м����м���
			}//�м���
		}
	}
	//��Χ�׸���

	while (int first = 1) {
		int i = kai1(generator);
		int j = kai2(generator);
		if (sq[i * linenum + j].bnum() == 0) {
			sweep(&sq[i * linenum + j], rownum, linenum, judgenum);
			first = 0;
			break;
		}
	}
	//�����ȡ���׷��� ��֤�״δ򿪷���Ź���������

	COLORREF bkclr = getpixel(5, 5);//��ȡ����ɫ
	setbkmode(OPAQUE);
	setbkcolor(black);
	settextcolor(red);
	setfont(45, 27, "����");
	TCHAR numtotal[10];
	if (bombnum > 99)sprintf_s(numtotal, _T("%d"), bombnum);
	else if (bombnum > 9)sprintf_s(numtotal, _T("0%d"), bombnum);
	else if (bombnum >= 0)sprintf_s(numtotal, _T("00%d"), bombnum);
	outtextxy(20, 15-origin, numtotal);
	//�׸�����ʼ��

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	thread thdtime(puttime, start, bklen, origin);
	thdtime.detach();//�������еķ����߳�

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
				if (sq[i * linenum + j].ifstat())KAI(&sq[i * linenum + j], rownum, linenum, judgenum);//δɨ ɨ����
				else if (!sq[i * linenum + j].ifstat())sweep(&sq[i * linenum + j], rownum, linenum, judgenum);//��ɨ ����ɨ�Ź���
			}
			//�������ɨ��

			else if (mes.message == WM_RBUTTONUP && !sq[i * linenum + j].ifstat()) {

				if (!sq[i * linenum + j].ifflag() && !sq[i * linenum + j].ifstat()) {
					sq[i * linenum + j].putflag(judgenum);
					currentnumber--;
				}//û���Ҳû�� ����
				else if (sq[i * linenum + j].ifflag() && !sq[i * linenum + j].ifstat()) {
					sq[i * linenum + j].putcover(judgenum);
					currentnumber++;
				}//�������û�� ��ԭ

				setbkcolor(black);
				settextcolor(red);
				setfont(45, 27, "����");
				TCHAR numlft[10];
				if (currentnumber > 99)sprintf_s(numlft, _T("%d"),currentnumber);
				else if (currentnumber > 9)sprintf_s(numlft, _T("0%d"), currentnumber);
				else if (currentnumber >= 0)sprintf_s(numlft, _T("00%d"), currentnumber);
				outtextxy(20, 15 - origin, numlft);
				//ʵʱ��¼�׸���

			}
			//�����Ҽ������

			if (mistake) {

				timereset = false;//ֹͣ��ʱ
				for (int k = 2; k > 0; k--) {

					setlinecolor(red);
					arc(sq[i * linenum + j].getxloc() + 1, sq[i * linenum + j].getyloc() + 1, sq[i * linenum + j].getxloc() + 29, sq[i * linenum + j].getyloc() + 29, 0, 2 * pi);
					//��ɫ��ʾ

					this_thread::sleep_for(chrono::milliseconds(400));
					//ɫ��ͣ�� �ﵽ��˸Ч��

					setlinecolor(white);
					arc(sq[i * linenum + j].getxloc() + 1, sq->getyloc() + 1, sq[i * linenum + j].getxloc() + 29, sq->getyloc() + 29, 0, 2 * pi);
					setlinecolor(gray4);
					arc(sq[i * linenum + j].getxloc() + 1, sq->getyloc() + 1, sq[i * linenum + j].getxloc() + 29, sq->getyloc() + 29, -0.75 * pi, 0.25 * pi);
					//�����

					this_thread::sleep_for(chrono::milliseconds(200));

				}
				break;

			}
			//��Ϸʧ��
			else if (judgenum == 0 && currentnumber == 0) {
				timereset = false;//ֹͣ��ʱ
				this_thread::sleep_for(chrono::milliseconds(2000));
				break;
			}
			//��Ϸ�ɹ�

		}
	}

	delete[] sq;
	initpick();

}
//һ����Ϸ����

void initpick() {

	initgraph(600, 250);
	setfillcolor(gray2);
	fillrectangle(0, 0, 600, 250);
	setbkcolor(gray2);
	settextcolor(blue);
	setfont(40, 20, "����");
	outtextxy(50, 55, "����");
	outtextxy(250, 55, "�м�");
	outtextxy(450, 55, "ר��");
	settextcolor(red);
	outtextxy(50, 160, "����");
	outtextxy(250, 160, "�ɼ�");
	outtextxy(450, 160, "�˳�");
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
			}//����
			else if (pick.message == WM_LBUTTONUP && pick.x >= 230 && pick.x <= 350 && pick.y >= 45 && pick.y <= 105) {
				closegraph();
				initgame(2);
			}//�м�
			else if (pick.message == WM_LBUTTONUP && pick.x >= 430 && pick.x <= 550 && pick.y >= 45 && pick.y <= 105) {
				closegraph();
				initgame(3);
			}//ר��
			else if (pick.message == WM_LBUTTONUP && pick.x >= 30 && pick.x <= 150 && pick.y >= 150 && pick.y <= 210) {

			}//����
			else if (pick.message == WM_LBUTTONUP && pick.x >= 230 && pick.x <= 350 && pick.y >= 150 && pick.y <= 210) {

			}//�ɼ�
			else if (pick.message == WM_LBUTTONUP && pick.x >= 430 && pick.x <= 550 && pick.y >= 150 && pick.y <= 210) {
				closegraph();
				break;
			}//�˳�
		}
	}

}
//һ��ѡ�����

int main()
{

	initpick();

	return 0;
};

/*�������
* ��ʱֹͣɨ��
* ��ͬ���ⱳ����ͼ��
* ��Ч
* �Զ�ģʽ
* �����淨
* 
* �����ַŴ���С
*/