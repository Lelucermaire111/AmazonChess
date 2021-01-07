// ��������ѷ������
#include<graphics.h>
#include <conio.h>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<time.h>
#include"Star.h"
#define MAX_VAL  100000
#define MIN_VAL  -100000
using namespace std;
int map[8][8];//�������״̬
int n;//������Ϸ����
int AI;//���Ƶ����Ѷ�
int turnID = 1;//�����������
int step = 50;//������ӿ��
int my_x[4], my_y[4], enemy_x[4], enemy_y[4];//����˫������λ��
int dx[8] = { 1,1,1,-1,-1,-1,0,0 };//direction����
int dy[8] = { 1,0,-1,1,0,-1,1,-1 };
int answer_x1, answer_x2, answer_x3, answer_y1, answer_y2, answer_y3;//��������� �ֱ��Ӧ���ӡ����ӡ��ϰ�
int enddeep;//������������
int RegretTimes = 3;//����������
int KeyTimes = 3;//������ʾ����
void Key();//��ʾ����
void save1();//���˴����
void load1();
void save2();//�˻������
void load2();
void RegretSave1();//���˻���
void RegretSave2();
void RegretLoad1();//�˻�����
void RegretLoad2();
void LoadBoard();
void instructions();//��Ϸ˵������
void Amazons_Chess();//����������
double Evaluate();//��ֵ����
double DFS_alphabeta(int depth, double lasrmaxmin, int my_color, int ennmy_color);//����+��֦����
bool OutOfMap(int x, int y)//�ж��Ƿ�Խ�磬���Խ�緵��1
{
	if (x < 0 || x>7 || y < 0 || y>7)
		return true;
	else
		return false;
}
bool illegal(int i, int j, int x, int y)//�ж��߷��Ƿ�Ϸ�
{
	if (i == x)//����
	{
		for (int k = min(j, y) + 1; k < max(j, y); k++)
		{
			if (map[i][k] != 0)return true;
		}
		return false;
	}
	if (j == y)//����
	{
		for (int k = min(i, x) + 1; k < max(i, x); k++)
		{
			if (map[k][j] != 0)return true;
		}
		return false;
	}
	if (i - x == y - j)//б��
	{
		int p = max(j, y) - 1;
		for (int k = min(i, x) + 1; k < max(i, x); k++)
		{
			if (map[k][p] != 0)return true;
			p--;
			if (p >= min(j, y))break;
		}
		return false;
	}
	if (i - x == j - y)//б��
	{
		int p = min(j, y) + 1;
		for (int k = min(i, x) + 1; k < max(i, x); k++)
		{
			if (map[k][p] != 0)return true;
			p++;
			if (p <= max(j, y))break;
		}
		return false;
	}
	else
		return true;
}
bool Blacklose()//�жϺڷ���
{
	int FlagWin = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j] == 2)
			{
				for (int p = 0; p < 8; p++)
				{
					if (map[dx[p] + i][dy[p] + j] == 0)
					{
						if (OutOfMap(dx[p] + i, dy[p] + j))
							continue;
						FlagWin = 0;
						return false;
					}
					else
					{
						FlagWin = 1;
					}
				}
			}
		}
	}
	return true;
}
bool Whitelose()//�жϰ׷���
{
	int FlagWin = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j] == 1)
			{
				for (int p = 0; p < 8; p++)
				{
					if (map[dx[p] + i][dy[p] + j] == 0)
					{
						if (OutOfMap(dx[p] + i, dy[p] + j))
							continue;
						FlagWin = 0;
						return false;
					}
					else
					{
						FlagWin = 1;
					}
				}
			}
		}
	}
	return true;
}
int board()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			map[i][j] = 0;
	map[2][0] = map[5][0] = map[0][2] = map[7][2] = 1;
	map[0][5] = map[7][5] = map[2][7] = map[5][7] = 2;
	return 0;
}
int Draw()
{
	int i, j;
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 8; j++)
		{
			if ((i + j) % 2 == 1)
			{
				setfillcolor(BLUE);
				solidrectangle(i * step, j * step, (i + 1) * step, (j + 1) * step); 	// ������ɫ����
			}
			else
			{
				setfillcolor(GREEN);
				solidrectangle(i * step, j * step, (i + 1) * step, (j + 1) * step); 	// ������ɫ����
			}
		}
	}
	return 0;
}
int myking[8][8], diking[8][8], myqueen[8][8], diqueen[8][8];//�ֱ����king�߷���queen�߷��ܵ���ո����С����
int visited[8][8];//��������Ƿ񱻷��ʹ�
int queenx[65], queeny[65];
int qstep[65];//�ֱ����ǰ�����Լ����沽��
int mobility;
double Evaluate()
{
	int end, top = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			myking[i][j] = diking[i][j] = myqueen[i][j] = diqueen[i][j] = MAX_VAL;
		}
	double t1 = 0, t2 = 0, p1 = 0, p2 = 0, m = 0;//�ֱ��ֵ�Ĳ���
	for (int z = 0; z < 4; z++)
	{
		memset(visited, 0, sizeof(visited));
		top = 0;
		end = 1;
		queenx[0] = my_x[z];
		queeny[0] = my_y[z];
		qstep[0] = 0;
		visited[my_x[z]][my_y[z]] = 1;
		while (top != end)
		{
			int x = queenx[top];
			int y = queeny[top];
			int step = qstep[top];
			myking[x][y] = min(myking[x][y], step);
			for (int i = 0; i < 8; i++)
			{
				if (OutOfMap(x + dx[i], y + dy[i]))continue;
				if (visited[x + dx[i]][y + dy[i]])continue;
				if (map[x + dx[i]][y + dy[i]])continue;
				queenx[end] = x + dx[i];
				queeny[end] = y + dy[i];
				qstep[end] = step + 1;//ÿ����Χ̽��������+1
				visited[x + dx[i]][y + dy[i]] = 1;
				end++;
			}
			top++;
		}
		myking[my_x[z]][my_y[z]] = MAX_VAL;//ֻ����µ�ǰ�������ڸ��ӵĲ���������ո�ȡ�ĸ�������С������
	}
	for (int z = 0; z < 4; z++)
	{
		memset(visited, 0, sizeof(visited));
		top = 0;
		end = 1;
		queenx[0] = enemy_x[z];
		queeny[0] = enemy_y[z];
		qstep[0] = 0;
		visited[enemy_x[z]][enemy_y[z]] = 1;
		while (top != end)
		{
			int x = queenx[top];
			int y = queeny[top];
			int step = qstep[top];
			diking[x][y] = min(diking[x][y], step);
			for (int i = 0; i < 8; i++)
			{
				if (OutOfMap(x + dx[i], y + dy[i]))continue;
				if (visited[x + dx[i]][y + dy[i]])continue;
				if (map[x + dx[i]][y + dy[i]])continue;
				queenx[end] = x + dx[i];
				queeny[end] = y + dy[i];
				qstep[end] = step + 1;//ÿ����Χ̽��������+1
				visited[x + dx[i]][y + dy[i]] = 1;
				end++;
			}
			top++;
		}
		diking[enemy_x[z]][enemy_y[z]] = MAX_VAL;//ֻ����µ�ǰ�������ڸ��ӵĲ���������ո�ȡ�ĸ�������С������
	}
	for (int z = 0; z < 4; z++)//��ʼִ��queen�߷�
	{
		memset(visited, 0, sizeof(visited));
		top = 0;
		end = 1;
		queenx[0] = my_x[z];
		queeny[0] = my_y[z];
		qstep[0] = 0;
		visited[my_x[z]][my_y[z]] = 1;
		while (top != end)
		{
			int x = queenx[top];
			int y = queeny[top];
			int step = qstep[top];
			myqueen[x][y] = min(myqueen[x][y], step);
			for (int i = 0; i < 8; i++)
			{
				for (int p = 1;; p++)
				{
					if (OutOfMap(x + p * dx[i], y + p * dy[i]))break;
					if (visited[x + p * dx[i]][y + p * dy[i]])continue;
					if (map[x + p * dx[i]][y + p * dy[i]])break;//ע����king�߷�������
					queenx[end] = x + p * dx[i];
					queeny[end] = y + p * dy[i];
					qstep[end] = step + 1;
					visited[x + p * dx[i]][y + p * dy[i]] = 1;
					end++;
				}

			}
			top++;
		}
		myqueen[my_x[z]][my_y[z]] = MAX_VAL;
	}
	for (int z = 0; z < 4; z++)//��ʼִ��queen�߷�
	{
		memset(visited, 0, sizeof(visited));
		top = 0;
		end = 1;
		queenx[0] = enemy_x[z];
		queeny[0] = enemy_y[z];
		qstep[0] = 0;
		visited[enemy_x[z]][enemy_y[z]] = 1;
		while (top != end)
		{
			int x = queenx[top];
			int y = queeny[top];
			int step = qstep[top];
			diqueen[x][y] = min(diqueen[x][y], step);
			for (int i = 0; i < 8; i++)
			{
				for (int p = 1;; p++)
				{
					if (OutOfMap(x + p * dx[i], y + p * dy[i]))break;
					if (visited[x + p * dx[i]][y + p * dy[i]])continue;
					if (map[x + p * dx[i]][y + p * dy[i]])break;//ע����king�߷�������
					queenx[end] = x + p * dx[i];
					queeny[end] = y + p * dy[i];
					qstep[end] = step + 1;
					visited[x + p * dx[i]][y + p * dy[i]] = 1;
					end++;
				}

			}
			top++;
		}
		diqueen[enemy_x[z]][enemy_y[z]] = MAX_VAL;
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (myqueen[i][j] > 1)continue;//��֤һ��֮�ڵ���
			mobility = 0;
			for (int p = 0; p < 8; p++)
			{
				if (OutOfMap(i + dx[p], j + dy[p]))continue;
				if (map[i + dx[p]][j + dx[p]])continue;
				mobility++;
			}
			m += mobility / myking[i][j];
		}
	//mobility����m�������
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (myking[i][j] > diking[i][j])t1 += -1;
			else if (myking[i][j] < diking[i][j])t1 += 1;
			else if (myking[i][j] == MAX_VAL)t1 += 0;
			if (myqueen[i][j] > diqueen[i][j])t2 += -1;
			else if (myqueen[i][j] < diqueen[i][j])t2 += 1;
			else if (myqueen[i][j] == MAX_VAL)t2 += 0;
			p1 += 2.0 * (pow(2.0, -1 * myqueen[i][j]) - pow(2.0, -1 * diqueen[i][j]));
			p2 += min(1.0, max(-1.0, (diking[i][j] - myking[i][j]) / 6.0));
		}
	double value;
	if (n <= 7) value = 0.14 * t1 + 0.40 * t2 + 0.13 * p1 + 0.13 * p2 + 0.20 * m;
	else if (n <= 16) value = 0.30 * t1 + 0.25 * t2 + 0.20 * p1 + 0.20 * p2 + 0.05 * m;
	else value = 0.8 * t1 + 0.1 * t2 + 0.05 * p1 + 0.05 * p2;
	return value;
}
double DFS_alphabeta(int depth, double lastmaxmin,int my_color,int enemy_color)//�����������+���¼�֦ ���ϸ���Ѱ�����ֵ
{
	if (depth == enddeep)return Evaluate();
	double maxmin;
	//����ҷ�����alpha��֦
	if (depth % 2 == 0)
	{
		maxmin = MIN_VAL;
		int x1, x2, x3, y1, y2, y3;
		for (int z = 0; z < 4; z++)
		{
			x1 = my_x[z];
			y1 = my_y[z];
			for (int i = 0; i < 8; i++)
			{
				for (int p = 1;; p++)
				{
					if (OutOfMap(x1 + p * dx[i], y1 + p * dy[i]))break;
					if (map[x1 + p * dx[i]][y1 + p * dy[i]])break;
					x2 = x1 + p * dx[i];
					y2 = y1 + p * dy[i];
					my_x[z] = x2;
					my_y[z] = y2;
					map[x1][y1] = 0;
					map[x2][y2] = my_color;
					//�ͷ��ϰ�
					for (int i = 0; i < 8; i++)
					{
						for (int p = 1;; p++)
						{
							if (OutOfMap(x2 + p * dx[i], y2 + p * dy[i]))break;
							if (map[x2 + p * dx[i]][y2 + p * dy[i]])break;
							x3 = x2 + p * dx[i];
							y3 = y2 + p * dy[i];
							map[x3][y3] = -1;
							double newval = DFS_alphabeta(depth + 1, maxmin,my_color,enemy_color);
							map[x3][y3] = 0;//�ָ�
							if (depth != 0)maxmin = max(newval, maxmin);//���������ȡ����������ֵ
							else if (newval > maxmin)
							{
								maxmin = newval;
								answer_x1 = x1;
								answer_y1 = y1;
								answer_x2 = x2;
								answer_y2 = y2;
								answer_x3 = x3;
								answer_y3 = y3;
							}
							if (maxmin >= lastmaxmin)//��֦
							{
								map[x1][y1] = my_color;
								map[x2][y2] = 0;
								my_x[z] = x1;
								my_y[z] = y1;
								return maxmin;
							}
						}
					}
					//�ͷ��ϰ�����
					map[x1][y1] = my_color;
					map[x2][y2] = 0;
					my_x[z] = x1;
					my_y[z] = y1;
				}
			}
		}
		if (maxmin == MIN_VAL)return Evaluate();
	}
	//����з�����¼�֦
	else
	{
		maxmin = MAX_VAL;
		int x1, x2, x3, y1, y2, y3;
		for (int z = 0; z < 4; z++)
		{
			x1 = enemy_x[z];
			y1 = enemy_y[z];
			for (int i = 0; i < 8; i++)
			{
				for (int p = 1;; p++)
				{
					if (OutOfMap(x1 + p * dx[i], y1 + p * dy[i]))break;
					if (map[x1 + p * dx[i]][y1 + p * dy[i]])break;
					x2 = x1 + p * dx[i];
					y2 = y1 + p * dy[i];
					enemy_x[z] = x2;
					enemy_y[z] = y2;
					map[x1][y1] = 0;
					map[x2][y2] = enemy_color;
					//�ͷ��ϰ�
					for (int i = 0; i < 8; i++)
					{
						for (int p = 1;; p++)
						{
							if (OutOfMap(x2 + p * dx[i], y2 + p * dy[i]))break;
							if (map[x2 + p * dx[i]][y2 + p * dy[i]])break;
							x3 = x2 + p * dx[i];
							y3 = y2 + p * dy[i];
							map[x3][y3] = -1;
							double newval = DFS_alphabeta(depth + 1, maxmin,my_color,enemy_color);
							map[x3][y3] = 0;//�ָ�
							maxmin = min(newval, maxmin);
							if (maxmin <= lastmaxmin)
							{
								map[x1][y1] = enemy_color;
								map[x2][y2] = 0;
								enemy_x[z] = x1;
								enemy_y[z] = y1;
								return maxmin;
							}
						}
					}
					//�ͷ��ϰ�����
					map[x1][y1] = enemy_color;
					map[x2][y2] = 0;
					enemy_x[z] = x1;
					enemy_y[z] = y1;
				}
			}
		}
		if (maxmin == MAX_VAL)return Evaluate();
	}
	return maxmin;
}
void MODE1()
{
b:int step = 50;
	// ��ʼ����ͼ����
	initgraph(800, 600);
	// ���ñ���ɫΪ��ɫ
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	loadimage(NULL, _T("1.jpg"), 800, 600);
	Draw();
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	settextcolor(BLACK);
	for (int i = 0; i <= 7; i++)
	{
		setbkmode(TRANSPARENT);
		outtextxy(75 + 50 * i, 25, int('0' + i));

	}
	for (int i = 0; i <= 7; i++)
	{
		setbkmode(TRANSPARENT);
		outtextxy(25, 75 + 50 * i, int('0' + i));
	}
	f.lfHeight = 30;					// ��������߶�Ϊ 30
	settextstyle(&f);
	outtextxy(500, 250, "��ӭ��������ѷ��!");
	outtextxy(600, 300, "�浵");
	outtextxy(700, 300, "����");
	outtextxy(700, 550, "����");
	outtextxy(0, 550, "�˻ز˵�");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j] == 1)
			{
				setfillcolor(WHITE);
				solidcircle(25 + (i + 1) * step, 25 + (j + 1) * step, 20);
			}
			if (map[i][j] == 2)
			{
				setfillcolor(BLACK);
				solidcircle(25 + (i + 1) * step, 25 + (j + 1) * step, 20);
			}
		}
	MOUSEMSG mouse_1, mouse_2, mouse_3;
	int x, y, x_1, y_1, x_2, y_2, x_3, y_3;
	while (1)
	{
	a:mouse_1 = GetMouseMsg();
		if (Whitelose())
		{
			settextcolor(BLACK);
			outtextxy(600, 350, "�ڷ�ʤ");
			outtextxy(550, 400, "�Ƿ�����һ�֣�");
			outtextxy(600, 450, "��");
			outtextxy(680, 450, "��");
			mouse_1 = GetMouseMsg();
			if (mouse_1.uMsg == WM_LBUTTONDOWN)
			{
				if (mouse_1.x >= 600 && mouse_1.x <= 630 && mouse_1.y >= 450 && mouse_1.y <= 490)
				{
					turnID = 1;
					board();
					Draw();
					goto b;
				}
				if (mouse_1.x >= 680 && mouse_1.x <= 710 && mouse_1.y >= 450 && mouse_1.y <= 490)
				{
					closegraph();
					Amazons_Chess();
				}
			}
		}
		if (Blacklose())
		{
			settextcolor(BLACK);
			outtextxy(600, 350, "�׷�ʤ");
			outtextxy(550, 400, "�Ƿ�����һ�֣�");
			outtextxy(600, 450, "��");
			outtextxy(680, 450, "��");
			mouse_1 = GetMouseMsg();
			if (mouse_1.uMsg == WM_LBUTTONDOWN)
			{
				if (mouse_1.x >= 600 && mouse_1.x <= 630 && mouse_1.y >= 450 && mouse_1.y <= 490)
				{
					turnID = 1;
					board();
					Draw();
					goto b;
				}
				if (mouse_1.x >= 680 && mouse_1.x <= 710 && mouse_1.y >= 450 && mouse_1.y <= 490)
				{
					closegraph();
					Amazons_Chess();
				}
			}
		}
		if (mouse_1.uMsg == WM_LBUTTONDOWN)
		{
			if (mouse_1.x >= 600 && mouse_1.x <= 660 && mouse_1.y >= 300 && mouse_1.y <= 340)
			{
				save1();
				outtextxy(610, 340, "�浵�ɹ�!");
				Sleep(1000);
				setbkcolor(WHITE);
				clearrectangle(610, 340, 750, 370);
			}
			if (mouse_1.x >= 700 && mouse_1.x <= 760 && mouse_1.y >= 300 && mouse_1.y <= 340)
			{
				load1();
				Draw();
				LoadBoard();
				goto a;
			}
			if (mouse_1.x >= 700 && mouse_1.x <= 730 && mouse_1.y >= 550 && mouse_1.y <= 580)
			{
				RegretLoad1();
				Draw();
				LoadBoard();
				goto a;
			}
			if (mouse_1.x >= 0 && mouse_1.x <= 160 && mouse_1.y >= 550 && mouse_1.y <= 600)
			{
				closegraph();
				Amazons_Chess();
			}
			RegretSave1();
			x = 50 * int(mouse_1.x / 50) + 25;
			y = 50 * int(mouse_1.y / 50) + 25;
			x_1 = (mouse_1.x / 50) - 1; y_1 = (mouse_1.y / 50) - 1;
			if (turnID == 1)
			{
				if (map[x_1][y_1] == 2)
				{
					while (true)
					{
						mouse_2 = GetMouseMsg();
						if (mouse_2.uMsg == WM_LBUTTONDOWN)
						{
							x = 50 * int(mouse_2.x / 50) + 25;
							y = 50 * int(mouse_2.y / 50) + 25;
							x_2 = (mouse_2.x / 50) - 1; y_2 = (mouse_2.y / 50) - 1;
							if (map[x_2][y_2] || OutOfMap(x_2, y_2) || illegal(x_1, y_1, x_2, y_2))goto a;
							else
							{
								setfillcolor(BLACK);
								fillcircle(x, y, 20);
								if ((x_1 + y_1) % 2 == 1)
								{
									setfillcolor(BLUE);
									solidrectangle((x_1 + 1) * step, (y_1 + 1) * step, (x_1 + 2) * step, (y_1 + 2) * step); 	// ������ɫ����
								}
								else
								{
									setfillcolor(GREEN);
									solidrectangle((x_1 + 1) * step, (y_1 + 1) * step, (x_1 + 2) * step, (y_1 + 2) * step); 		// ������ɫ����
								}
								map[x_2][y_2] = 2;
								map[x_1][y_1] = 0;
							}
							while (true)
							{
								mouse_3 = GetMouseMsg();
								if (mouse_3.uMsg == WM_LBUTTONDOWN)
								{
									x = 50 * int(mouse_3.x / 50) + 25;
									y = 50 * int(mouse_3.y / 50) + 25;
									x_3 = (mouse_3.x / 50) - 1; y_3 = (mouse_3.y / 50) - 1;
									if (map[x_3][y_3] != 0 || OutOfMap(x_3, y_3) || illegal(x_2, y_2, x_3, y_3))continue;
									else
									{
										line(x - 25, y - 25, x + 25, y + 25);
										line(x - 25, y + 25, x + 25, y - 25);
										map[x_3][y_3] = -1;
										break;
									}

								}
							}
							turnID = 2;
							break;
						}
					}
				}
			}
			if (turnID == 2)
			{
				if (map[x_1][y_1] == 1)
				{
					while (true)
					{
						mouse_2 = GetMouseMsg();
						if (mouse_2.uMsg == WM_LBUTTONDOWN)
						{
							x = 50 * int(mouse_2.x / 50) + 25;
							y = 50 * int(mouse_2.y / 50) + 25;
							x_2 = (mouse_2.x / 50) - 1; y_2 = (mouse_2.y / 50) - 1;
							if (map[x_2][y_2] || OutOfMap(x_2, y_2) || illegal(x_1, y_1, x_2, y_2))goto a;
							else
							{
								setfillcolor(WHITE);
								fillcircle(x, y, 20);
								if ((x_1 + y_1) % 2 == 1)
								{
									setfillcolor(BLUE);
									solidrectangle((x_1 + 1) * step, (y_1 + 1) * step, (x_1 + 2) * step, (y_1 + 2) * step); 	// ������ɫ����
								}
								else
								{
									setfillcolor(GREEN);
									solidrectangle((x_1 + 1) * step, (y_1 + 1) * step, (x_1 + 2) * step, (y_1 + 2) * step); 		// ������ɫ����
								}
								map[x_2][y_2] = 1;
								map[x_1][y_1] = 0;
							}
							while (true)
							{
								mouse_3 = GetMouseMsg();
								if (mouse_3.uMsg == WM_LBUTTONDOWN)
								{
									x = 50 * int(mouse_3.x / 50) + 25;
									y = 50 * int(mouse_3.y / 50) + 25;
									x_3 = (mouse_3.x / 50) - 1; y_3 = (mouse_3.y / 50) - 1;
									if (map[x_3][y_3] != 0 || OutOfMap(x_3, y_3) || illegal(x_2, y_2, x_3, y_3))continue;
									else
									{
										line(x - 25, y - 25, x + 25, y + 25);
										line(x - 25, y + 25, x + 25, y - 25);
										map[x_3][y_3] = -1;
										break;
									}

								}
							}
							turnID = 1;
							break;
						}
					}
				}
			}
		}
	}
}
void MODE2()
{
b:int step = 50;
	// ��ʼ����ͼ����
	initgraph(800, 600);
	// ���ñ���ɫΪ��ɫ
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	loadimage(NULL, _T("1.jpg"), 800, 600);
	Draw();
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	settextcolor(BLACK);
	setlinestyle(PS_SOLID);
	for (int i = 0; i <= 7; i++)
	{
		setbkmode(TRANSPARENT);
		outtextxy(75 + 50 * i, 25, int('0' + i));

	}
	for (int i = 0; i <= 7; i++)
	{
		setbkmode(TRANSPARENT);
		outtextxy(25, 75 + 50 * i, int('0' + i));
	}
	f.lfHeight = 30;					// ��������߶�Ϊ 30
	settextstyle(&f);
	outtextxy(500, 250, "��ӭ��������ѷ��!");
	outtextxy(600, 300, "�浵");
	outtextxy(700, 300, "����");
	outtextxy(700, 550, "����");
	outtextxy(700, 500, "��ʾ");
	outtextxy(200, 550, "������  �λ������");
	outtextxy(200, 500, "������  ����ʾ����");
	outtextxy(0, 550, "�˻ز˵�");
	outtextxy(300, 550, RegretTimes + '0');
	outtextxy(300, 500, KeyTimes + '0');
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j] == 1)
			{
				setfillcolor(WHITE);
				solidcircle(25 + (i + 1) * step, 25 + (j + 1) * step, 20);
			}
			if (map[i][j] == 2)
			{
				setfillcolor(BLACK);
				solidcircle(25 + (i + 1) * step, 25 + (j + 1) * step, 20);
			}
		}
	MOUSEMSG mouse_1, mouse_2, mouse_3;
	int x,y,x_1, y_1, x_2, y_2, x_3, y_3;
	while (1)
	{
		turnID = 1;
		if (Whitelose())
		{
			settextcolor(BLACK);
			outtextxy(600, 350, "�ڷ�ʤ");
			outtextxy(550, 400, "�Ƿ�����һ�֣�");
			outtextxy(600, 450, "��");
			outtextxy(680, 450, "��");
			mouse_1 = GetMouseMsg();
			if (mouse_1.uMsg == WM_LBUTTONDOWN)
			{
				if (mouse_1.x >= 600 && mouse_1.x <= 630 && mouse_1.y >= 450 && mouse_1.y <= 490)//�������
				{
					n = 1;
					turnID = 1;
					board();
					Draw();
					goto b;
				}
				if (mouse_1.x >= 680 && mouse_1.x <= 710 && mouse_1.y >= 450 && mouse_1.y <= 490)//����˳�
				{
					n = 1;
					turnID = 1;
					KeyTimes = 3;
					RegretTimes = 3;
					closegraph();
					Amazons_Chess();
				}
			}
		}
		if (Blacklose())
		{
			settextcolor(BLACK);
			outtextxy(600, 350, "�׷�ʤ");
			outtextxy(550, 400, "�Ƿ�����һ�֣�");
			outtextxy(600, 450, "��");
			outtextxy(680, 450, "��");
			mouse_1 = GetMouseMsg();
			if (mouse_1.uMsg == WM_LBUTTONDOWN)
			{
				if (mouse_1.x >= 600 && mouse_1.x <= 630 && mouse_1.y >= 450 && mouse_1.y <= 490)
				{
					n = 1;
					turnID = 1;
					board();
					Draw();
					RegretTimes = 3;
					KeyTimes = 3;
					goto b;
				}
				if (mouse_1.x >= 680 && mouse_1.x <= 710 && mouse_1.y >= 450 && mouse_1.y <= 490)
				{
					n = 1;
					turnID = 1;
					closegraph();
					Amazons_Chess();
				}
			}
		}
		if (turnID == 1)
		{
		a:mouse_1 = GetMouseMsg();
			if (mouse_1.uMsg == WM_LBUTTONDOWN)
			{
				if (mouse_1.x >= 600 && mouse_1.x <= 660 && mouse_1.y >= 300 && mouse_1.y <= 340)
				{
					save2();
					outtextxy(610, 340, "�浵�ɹ�!");
					Sleep(1000);
					setbkcolor(WHITE);
					clearrectangle(610, 340, 750, 370);
				}
				if (mouse_1.x >= 700 && mouse_1.x <= 760 && mouse_1.y >= 300 && mouse_1.y <= 340)
				{
					load2();
					Draw();
					LoadBoard();
					clearrectangle(300, 500, 600, 550);
					outtextxy(200, 500, "������  ����ʾ����");
					outtextxy(300, 500, KeyTimes + '0');
					clearrectangle(300, 550, 600, 600);
					outtextxy(200, 550, "������  �λ������");
					outtextxy(300, 550, RegretTimes + '0');
					goto a;
				}
				if (mouse_1.x >= 700 && mouse_1.x <= 730 && mouse_1.y >= 550&& mouse_1.y <= 580)
				{
					if (RegretTimes)
					{
						RegretLoad2();
						Draw();
						LoadBoard();
						RegretTimes--;
						setbkcolor(WHITE);
						clearrectangle(300, 550, 600, 600);
						outtextxy(200, 550, "������  �λ������");
						outtextxy(300, 550, RegretTimes + '0');
						clearrectangle(300, 500, 600, 550);
						outtextxy(200, 500, "������  ����ʾ����");
						outtextxy(300, 500, KeyTimes + '0');
					}
					goto a;
				}
				if (mouse_1.x >= 700 && mouse_1.x <= 760 && mouse_1.y >= 500 && mouse_1.y <= 530)
				{
					if (KeyTimes)
					{
						KeyTimes--;
						setbkcolor(WHITE);
						clearrectangle(300, 500, 600, 550);
						outtextxy(200, 500, "������  ����ʾ����");
						outtextxy(300, 500, KeyTimes + '0');
						Key();
					}
					goto a;
				}
				if (mouse_1.x >= 0 && mouse_1.x <= 160 && mouse_1.y >= 550 && mouse_1.y <= 600)
				{
					closegraph();
					KeyTimes = 3;
					RegretTimes = 3;
					Amazons_Chess();
				}
				if (turnID == 1)
					RegretSave2();
				x = 50 * int(mouse_1.x / 50) + 25;
				y = 50 * int(mouse_1.y / 50) + 25;
				x_1 = (mouse_1.x / 50) - 1; y_1 = (mouse_1.y / 50) - 1;
				if (map[x_1][y_1] == 2)
				{
					while (true)
					{
						mouse_2 = GetMouseMsg();
						if (mouse_2.uMsg == WM_LBUTTONDOWN)
						{
							x = 50 * int(mouse_2.x / 50) + 25;
							y = 50 * int(mouse_2.y / 50) + 25;
							x_2 = (mouse_2.x / 50) - 1; y_2 = (mouse_2.y / 50) - 1;
							if (map[x_2][y_2] || OutOfMap(x_2, y_2) || illegal(x_1, y_1, x_2, y_2))goto a;
							else
							{
								setfillcolor(BLACK);
								fillcircle(x, y, 20);
								if ((x_1 + y_1) % 2 == 1)
								{
									setfillcolor(BLUE);
									solidrectangle((x_1 + 1) * step, (y_1 + 1) * step, (x_1 + 2) * step, (y_1 + 2) * step); 	// ������ɫ����
								}
								else
								{
									setfillcolor(GREEN);
									solidrectangle((x_1 + 1)* step, (y_1 + 1)* step, (x_1 + 2)* step, (y_1 + 2)* step); 	// ������ɫ����
								}
								map[x_2][y_2] = 2;
								map[x_1][y_1] = 0;
							}
							while (true)
							{
								mouse_3 = GetMouseMsg();
								if (mouse_3.uMsg == WM_LBUTTONDOWN)
								{
									x = 50 * int(mouse_3.x / 50) + 25;
									y = 50 * int(mouse_3.y / 50) + 25;
									x_3 = (mouse_3.x / 50) - 1; y_3 = (mouse_3.y / 50) - 1;
									if (map[x_3][y_3] != 0 || OutOfMap(x_3, y_3) || illegal(x_2, y_2, x_3, y_3))continue;
									else
									{
										line(x - 25, y - 25, x + 25, y + 25);
										line(x - 25, y + 25, x + 25, y - 25);
										map[x_3][y_3] = -1;
										break;
									}

								}
							}
							n++;
							turnID = 2;
							break;
						}
					}
				}
			}
		}
		if (turnID == 2)
		{
			int my_num = 0, enemy_num = 0;
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
				{
					if (map[i][j] == 1)
					{
						my_x[my_num] = i;
						my_y[my_num] = j;
						my_num++;
					}
					if (map[i][j] == 2)
					{
						enemy_x[enemy_num] = i;
						enemy_y[enemy_num] = j;
						enemy_num++;
					}
				}
			if (AI == 2)
			{
				if (n < 12)enddeep = 1;
				else if (n < 18)enddeep = 2;
				else enddeep = 3;//���ž������Ӷ�̬������������
			}
			else if (AI == 1)
			{
				enddeep = 1;//��ͨAIֻ��һ��
			}
			DFS_alphabeta(0, MAX_VAL,1,2);
			if ((answer_x1 + answer_y1) % 2 == 1)
			{
				setfillcolor(BLUE);
				solidrectangle((answer_x1 + 1) * step, (answer_y1 + 1) * step, (answer_x1 + 2) * step, (answer_y1 + 2) * step); 	// ������ɫ����
			}
			else
			{
				setfillcolor(GREEN);
				solidrectangle((answer_x1 + 1) * step, (answer_y1 + 1) * step, (answer_x1 + 2) * step, (answer_y1 + 2) * step); 	// ������ɫ����
			}
			Sleep(100);
			setfillcolor(WHITE);
			fillcircle((answer_x2 + 1) * 50 + 25, (answer_y2 + 1) * 50 + 25, 20);
			line((answer_x3 + 1) * step, (answer_y3 + 1) * step, (answer_x3 + 2) * step, (answer_y3 + 2) * step);
			line((answer_x3 + 1) * step, (answer_y3 + 2) * step, (answer_x3 + 2) * step, (answer_y3 + 1) * step);
			turnID = 1;
			map[answer_x1][answer_y1] = 0;
			map[answer_x2][answer_y2] = 1;
			map[answer_x3][answer_y3] = -1;
		}
	}
}
void save1()
{
	ofstream fout("BoardData1.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fout << map[i][j] << " ";
		}
	fout << turnID;
	fout.close();
	return;
}
void save2()
{
	ofstream fout("BoardData2.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fout << map[i][j] << " ";
		}
	fout << turnID << " " << n << " " << KeyTimes << " " << RegretTimes;
	fout.close();
	return;
}
void RegretSave1()
{
	ofstream fout("Data1.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fout << map[i][j] << " ";
		}
	fout << turnID;
	fout.close();
	return;
}
void RegretSave2()
{
	ofstream fout("Data2.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fout << map[i][j] << " ";
		}
	fout << turnID << " " << n<<" "<<KeyTimes<<" "<<RegretTimes;
	fout.close();
	return;
}
void load1()
{
	ifstream fin;
	fin.open("BoardData1.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fin >> map[i][j];
		}
	fin >> turnID;
	fin.close();
	return;
}
void load2()
{
	ifstream fin;
	fin.open("BoardData2.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fin >> map[i][j];
		}
	fin >> turnID >> n >> KeyTimes >> RegretTimes;
	fin.close();
	return;
}
void RegretLoad1()
{
	ifstream fin;
	fin.open("Data1.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fin >> map[i][j];
		}
	fin >> turnID;
	fin.close();
	return;
}
void RegretLoad2()
{
	ifstream fin;
	fin.open("Data2.txt");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			fin >> map[i][j];
		}
	fin >> turnID >> n >> KeyTimes >> RegretTimes;
	fin.close();
	return;
}
void LoadBoard()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j] == 1)
			{
				setfillcolor(WHITE);
				fillcircle((i + 1) * 50 + 25, (j + 1) * 50 + 25, 20);
			}
			if (map[i][j] == 2)
			{
				setfillcolor(BLACK);
				fillcircle((i + 1) * 50 + 25, (j + 1) * 50 + 25, 20);
			}
			if (map[i][j] == -1)
			{
				line((i + 1) * step, (j + 1) * step, (i + 2) * step, (j + 2) * step);
				line((i + 1) * step, (j + 2) * step, (i + 2) * step, (j + 1) * step);
			}
		}
	}
}
void Key()
{
	int mmynum = 0, eennum = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j] == 2)
			{
				my_x[mmynum] = i;
				my_y[mmynum] = j;
				mmynum++;
			}
			if (map[i][j] == 1)
			{
				enemy_x[eennum] = i;
				enemy_y[eennum] = j;
				eennum++;
			}
		}
	enddeep = 1;
	DFS_alphabeta(0, MAX_VAL,2,1);
	map[answer_x1][answer_y1] = 2;
	map[answer_x2][answer_y2] = 0;
	if (answer_x1 == answer_x3 && answer_y1 == answer_y3)
		map[answer_x3][answer_y3] = 2;
	else
	{
		map[answer_x3][answer_y3] = 0;
	}
	setfillcolor(RED);//��Ȧ��ʾ��ʾ���߷�
	circle((answer_x1 + 1) * 50 + 25, (answer_y1 + 1) * 50 + 25, 5);
	circle((answer_x2 + 1) * 50 + 25, (answer_y2 + 1) * 50 + 25, 5);
	setlinestyle(PS_DASH | PS_ENDCAP_FLAT, 3);
	line((answer_x3 + 1) * step, (answer_y3 + 1) * step, (answer_x3 + 2) * step, (answer_y3 + 2) * step);
	line((answer_x3 + 1) * step, (answer_y3 + 2) * step, (answer_x3 + 2) * step, (answer_y3 + 1) * step);
	setlinestyle(PS_SOLID);
	Sleep(1500);
	if ((answer_x2 + answer_y2) % 2 == 1)
	{
		setfillcolor(BLUE);
		solidrectangle((answer_x2 + 1) * step, (answer_y2 + 1) * step, (answer_x2 + 2) * step, (answer_y2 + 2) * step); 	// ������ɫ����
	}
	else
	{
		setfillcolor(GREEN);
		solidrectangle((answer_x2 + 1) * step, (answer_y2 + 1) * step, (answer_x2 + 2) * step, (answer_y2 + 2) * step); 	// ������ɫ����
	}
		if ((answer_x3 + answer_y3) % 2 == 1)
		{
			setfillcolor(BLUE);
			solidrectangle((answer_x3 + 1) * step, (answer_y3 + 1) * step, (answer_x3 + 2) * step, (answer_y3 + 2) * step); 	// ������ɫ����
		}
		else
		{
			setfillcolor(GREEN);
			solidrectangle((answer_x3 + 1) * step, (answer_y3 + 1) * step, (answer_x3 + 2) * step, (answer_y3 + 2) * step); 	// ������ɫ����
		}
		setfillcolor(BLACK);
		fillcircle((answer_x1 + 1) * 50 + 25, (answer_y1 + 1) * 50 + 25, 20);

}
void instructions()
{
	initgraph(800, 500);
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	LOGFONT f;
	loadimage(NULL, _T("1.jpg"), 800, 500);
	gettextstyle(&f);
	f.lfHeight = 100;
	f.lfWeight = 100;
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(150, 50, "��Ϸ˵��");
	f.lfHeight = 30;
	f.lfWeight = 0;
	settextstyle(&f);
	outtextxy(0, 220, "1.��ʾ���ܣ���Ȧ�������������ӣ����ߴ������λ��");
	outtextxy(0, 260, "2.����ģʽ��������ʾ����");
	outtextxy(0, 300, "3.�˻�ģʽ��ÿ�������λ��塢������ʾ");
	outtextxy(0, 340, "4.��Ϸ���򣨽����������˻ر����ڣ�");
	outtextxy(0, 400, "ף����Ϸ���");
	outtextxy(600, 450, "�˻ز˵�");
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 0 && m.x <= 500 && m.y >= 340 && m.y <= 400)
			{
				closegraph();
				initgraph(1000, 800);
				loadimage(NULL, _T("rule.png"), 1000, 800);
				_getch();
				closegraph();
				instructions();
			}
			if (m.x >= 600 && m.x <= 720 && m.y >= 450 && m.y <= 480)
			{
				closegraph();
				Amazons_Chess();
			}
		}
	}

}
void Amazons_Chess()
{
	initgraph(800, 600);
	// ���ñ���ɫΪ��ɫ
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	loadimage(NULL, _T("1.jpg"), 800, 600);
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 100;
	f.lfWeight = 100;
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(200, 100, "����ѷ��");
	f.lfHeight = 40;
	f.lfWeight = 0;
	settextstyle(&f);
	outtextxy(300, 300, "����ģʽ");
	outtextxy(300, 400, "�˻���ս");
	outtextxy(300, 530, "��Ϸ˵��");
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 300 && m.x <= 460 && m.y >= 300 && m.y <= 340)
			{
				closegraph();
				board();
				MODE1();
			}
			if (m.x >= 300 && m.x <= 460 && m.y >= 400 && m.y <= 440)
			{
				outtextxy(300, 480, "��ͨ");
				outtextxy(400, 480, "����");
				while (1)
				{
					m = GetMouseMsg();
					if (m.uMsg == WM_LBUTTONDOWN)
					{
						if (m.x >= 300 && m.x <= 380 && m.y >= 480 && m.y <= 520)
							AI = 1;
						if (m.x >= 400 && m.x <= 480 && m.y >= 480 && m.y <= 520)
							AI = 2;
						closegraph();
						board();
						MODE2();
					}
				}
			}
			if (m.x >= 300 && m.x <= 460 && m.y >= 530 && m.y <= 570)
			{
				closegraph();
				instructions();
			}

		}
	}
	return;
}
int main()
{
	Start();
	Amazons_Chess();
	return 0;
}
