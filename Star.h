#pragma once
#include <graphics.h>
#include <time.h>
#include <conio.h>
#define MAXSTAR 500	// ��������
struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};

STAR star[MAXSTAR];

// ��ʼ������
void InitStar(int i)
{
	star[i].x = 0;
	star[i].y = rand() % 480;
	star[i].step = (rand() % 5000) / 1000.0 + 1;
	star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5);	// �ٶ�Խ�죬��ɫԽ��
	star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}

// �ƶ�����
void MoveStar(int i)
{
	// ����ԭ��������
	putpixel((int)star[i].x, star[i].y, 0);

	// ������λ��
	star[i].x += star[i].step;
	if (star[i].x > 640)	InitStar(i);

	// ��������
	putpixel((int)star[i].x, star[i].y, star[i].color);
}

// ������
void Start()
{
	int Switch=0;
	srand((unsigned)time(NULL));	// �������
	initgraph(640, 480);			// ������ͼ����
	LOGFONT f;
	gettextstyle(&f);  // ��ȡ��ǰ��������
	f.lfHeight = 50;					// ��������߶�Ϊ 30
	settextstyle(&f);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	outtextxy(120, 160, "����ѷ������С���");
	// ��ʼ����������
	for (int i = 0; i < MAXSTAR; i++)
	{
		InitStar(i);
		star[i].x = rand() % 640;
	}

	// �����ǿգ���������˳�
	while (1)
	{
		for (int i = 0; i < MAXSTAR; i++)
			MoveStar(i);
		Sleep(0.5);
		Switch++;
		if (Switch >= 1500)
		{
			outtextxy(120, 220, "���سɹ���");
			outtextxy(120, 280, "�����������Ϸ");
			_getch();
			break;
			closegraph();
		}
	}
}