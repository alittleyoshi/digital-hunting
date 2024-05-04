#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>
#include "shlwapi.h"
using namespace std;
int level;
int speed;
int debug = 0;
int invincible = 0;
int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, 1, -1};
int blessnum[4];
const int inf = 1e9 + 5;
int mp[200][200];
int block[200][200];
int selfwall[200][200];
char trans[5000];
int x, y, ex, ey, px, py;
int skilltime1;
int skilltime2;
int eneskilltime;
int siz = 10;
int checkvis[200][200];
int checkcnt = 0;
int distnow;
int levelnum;
string recpath = ".\\DigitalHunting\\record.txt";
string prcpath = ".\\DigitalHunting\\process.txt";
struct node
{
	int x;
	int y;
	int dis;
};
void cls()
{
	system("cls");
}
void gotoxy(int mx, int my)
{
	COORD pos = {mx, my};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
	return;
}
bool outof(int cx, int cy)
{
	return (cx > 0 && cy > 0 && cx <= siz && cy <= siz && block[cx][cy] == 0 && selfwall[cx][cy] == 0);
}
bool outof2(int cx, int cy)
{
	return (cx >= x - 1 && cy >= y - 1 && cx <= x + 1 && cy <= y + 1);
}
bool outof3(int cx, int cy, int dir)
{
	if (dir == 0)
		return (cx - 1 >= 1 && cx + 1 <= siz && cy >= 1 && cy <= siz);
	else
		return (cx >= 1 && cx <= siz && cy - 1 >= 1 && cy + 1 <= siz);
}

void checkroad(int nx, int ny)
{
	if (checkvis[nx][ny] == 1 || outof(nx, ny) == 0)
		return;
	if (nx == px && ny == py)
	{
		checkcnt++;
		return;
	}
	checkvis[nx][ny] = 1;
	checkroad(nx + 1, ny);
	checkroad(nx, ny + 1);
	checkroad(nx - 1, ny);
	checkroad(nx, ny + 1);
}
int roadcheck2(int nx, int ny)
{
	queue<node> q;
	q.push((node){nx, ny, 0});
	while (!q.empty())
	{
		node now = q.front();
		q.pop();
		if (now.x == x && now.y == y)
		{
			return now.dis;
		}
		for (int i = 0; i < 4; i++)
		{
			int vx = now.x + dx[i], vy = now.y + dy[i];
			if (checkvis[vx][vy] != 1 && outof(vx, vy) == 1)
			{
				checkvis[vx][vy] = 1;
				q.push((node){vx, vy, now.dis + 1});
			}
		}
	}
	return inf;
}
int roadcheck3(int nx, int ny)
{
	queue<node> q;
	q.push((node){nx, ny, 0});
	while (!q.empty())
	{
		node now = q.front();
		q.pop();
		if (now.x == px && now.y == py)
		{
			return now.dis;
		}
		for (int i = 0; i < 4; i++)
		{
			int vx = now.x + dx[i], vy = now.y + dy[i];
			if (checkvis[vx][vy] != 1 && outof(vx, vy) == 1)
			{
				checkvis[vx][vy] = 1;
				q.push((node){vx, vy, now.dis + 1});
			}
		}
	}
	return inf;
}
int enemycheck(int nex, int ney)
{
	memset(checkvis, 0, sizeof(checkvis));
	return roadcheck2(nex, ney);
}
int enemycheck2(int nex, int ney)
{
	memset(checkvis, 0, sizeof(checkvis));
	return roadcheck3(nex, ney);
}
char getkey()
{
	char key;
	while (1)
	{
		key = 0;
		if (_kbhit())
		{
			key = getch();
			break;
		}
	}
	if ('A' <= key && key <= 'Z')
		key = key - ('A' - 'a');
	return key;
}
void debuger()
{
	if (debug == 0)
		return;
	string str[5] = {"skill1", "skill2", "enemy_skill", "Invincible/Not Invincible", "quit"};
	int nowchoice = 0;
	cls();
	while (1)
	{
		gotoxy(0, 0);
		cout << "Debug Menu" << '\n';
		for (int i = 0; i < 5; i++)
		{
			if (i == nowchoice)
				cout << "-> ";
			else
				cout << "   ";
			cout << str[i] << '\n';
		}
		cout << "(Press S,W to choose, press E to confirm)" << '\n';
		char nchr = getkey();
		if (nchr == 's' && nowchoice < 4)
			nowchoice++;
		if (nchr == 'w' && nowchoice > 0)
			nowchoice--;
		if (nchr == 'e')
		{
			cls();
			if (nowchoice == 0)
			{
				cout << "input the new number:";
				cin >> skilltime1;
				cls();
				continue;
			}
			else if (nowchoice == 1)
			{
				cout << "input the new number:";
				cin >> skilltime2;
				cls();
				continue;
			}
			else if (nowchoice == 2)
			{
				cout << "input the new number:";
				cin >> eneskilltime;
				cls();
				continue;
			}
			else if (nowchoice == 3)
			{
				invincible = 1 - invincible;
				if (invincible == 1)
					cout << "You are now invincible.";
				else
					cout << "You are now  not invincible.";
				Sleep(1000);
				cls();
				continue;
			}
			else
				return;
		}
	}
}
void bless()
{
	string str[4] = {"Speed up(move more step at a time)", "Time blessing(get more Time Freeze count)", "Building blessing(get more wall building count)", "Give up the chance."};
	int nowchoice = 0;
	cls();
	while (1)
	{
		gotoxy(0, 0);
		cout << "You've reach LEVEL" << levelnum << ",so you can choose a blessing." << '\n';
		for (int i = 0; i < 4; i++)
		{
			if (i == nowchoice)
				cout << "-> ";
			else
				cout << "   ";
			cout << str[i] << '\n';
		}
		cout << "(Press S,W to choose, press E to confirm)" << '\n';
		char nchr = getkey();
		if (nchr == 's' && nowchoice < 3)
			nowchoice++;
		if (nchr == 'w' && nowchoice > 0)
			nowchoice--;
		if (nchr == 'e')
		{
			cls();
			if (nowchoice == 0)
				blessnum[0]++;
			else if (nowchoice == 1)
				blessnum[1] += levelnum / 5;
			else if (nowchoice == 2)
				blessnum[2] += levelnum / 5;
			cls();
			return;
		}
	}
}

char settrans()
{
	trans[0] = ' ';
	trans[1] = 'O';
	trans[2] = 'X';
	trans[3] = 'V';
	trans[4] = static_cast<char>(219);
	trans[5] = '*';
}
void wallreduce()
{
	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
		{
			if (selfwall[i][j] > 0)
				selfwall[i][j]--;
		}
	}
}
void renderMap(int i, int j)
{
	//	printf("%c ",trans[mp[i][j]]);
	switch (mp[i][j])
	{
	case 5:
		if (selfwall[i][j] >= 4)
		{
			printf("%c ", static_cast<char>(178));
		}
		else if (selfwall[i][j] >= 2)
		{
			printf("%c ", static_cast<char>(177));
		}
		else if (selfwall[i][j] >= 1)
		{
			printf("%c ", static_cast<char>(176));
		}
		break;
	default:
		printf("%c ", trans[mp[i][j]]);
	}
}
void ptmap()
{
	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
			renderMap(i, j);
		printf("\n");
	}
	cout << '\n';
	if (debug == 1)
		cout << "[DEBUG MODE]:Press B to open the DEBUG MENU." << endl;
	cout << "You are O, your enemy is X, you need to get V" << '\n';
	cout << "Press WASD to move" << '\n';
	cout << "Press F to give up moving." << '\n';
	cout << "Here are some skills you can use:" << '\n';
	cout << "1.Press 1: Time Freeze. (" << skilltime1 << ")" << '\n';
	cout << "2.Press 2: build a wall. (" << skilltime2 << ")" << '\n';
}
void ptmap1()
{
	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
			renderMap(i, j);
		printf("\n");
	}
	cout << '\n';
	if (debug == 1)
		cout << "[DEBUG MODE]:Press B to open the DEBUG MENU." << endl;
	cout << "Now is player1's turn.'" << '\n';
	cout << "Press WASD to move" << '\n';

	cout << "Here are some skills you can use:" << '\n';
	cout << "1.Press 1: Time Freeze. (" << skilltime1 << ")" << '\n';
	cout << "2.Press 2: build a wall. (" << skilltime2 << ")" << '\n';
}
void ptmap2(int lststep)
{
	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
			renderMap(i, j);
		printf("\n");
	}
	cout << '\n';
	if (debug == 1)
		cout << "[DEBUG MODE]:Press B to open the DEBUG MENU." << endl;
	cout << "Now is player2's turn.'" << '\n';
	cout << "Press IJKL to move" << '\n';
	cout << "You can move " << lststep << " more step." << '\n';
	cout << "If you don't want to move,press O." << '\n';
	cout << "Here are skills you can use:" << '\n';
	cout << "1.Press P: Instantly moved to player1's' side . (" << eneskilltime << ")" << '\n';
}
void setmap()
{
	memset(mp, 0, sizeof(mp));
	selfwall[x][y] = 0;
	selfwall[ex][ey] = 0;
	selfwall[px][py] = 0;
	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
		{
			if (block[i][j] == 1)
			{
				mp[i][j] = 4;
				continue;
			}
			if (block[i][j] == 1)
			{
				mp[i][j] = 4;
				continue;
			}
			if (selfwall[i][j] > 0)
			{
				mp[i][j] = 5;
				continue;
			}

			if (x == i && y == j)
				mp[i][j] = 1;
			if (ex == i && ey == j)
				mp[i][j] = 2;
			if (px == i && py == j)
				mp[i][j] = 3;
		}
	}
}

void enemymove()
{
	int dist1 = enemycheck(ex, ey);
	int dist2 = inf;
	node choicemove = (node){ex, ey};

	for (int i = 0; i < 4; i++)
	{
		int vx = ex + dx[i], vy = ey + dy[i];
		if (outof(vx, vy) == 0)
			continue;
		if (choicemove.x == ex && choicemove.y == ey)
			choicemove = (node){vx, vy, 0};
		dist2 = enemycheck(vx, vy);
		if (dist2 < dist1)
			choicemove = (node){vx, vy, 0};
	}

	ex = choicemove.x;
	ey = choicemove.y;
	if (x == ex && y == ey)
	{
		return;
	}
}

void enemyskill()
{
	int dist1 = enemycheck2(x, y);
	int dist2 = inf;
	node choicemove = (node){ex, ey};

	for (int i = 0; i < 4; i++)
	{
		int vx = x + dx[i] * 2, vy = y + dy[i] * 2;
		if (outof(vx, vy) == 0)
			continue;
		if (choicemove.x == ex && choicemove.y == ey)
			choicemove = (node){vx, vy, 0};
		dist2 = enemycheck2(vx, vy);
		if (dist2 < dist1)
			choicemove = (node){vx, vy, 0};
	}
	cls();
	ex = choicemove.x;
	ey = choicemove.y;

	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
			renderMap(i, j);
		printf("\n");
	}
	cout << "ENEMY SKILL";
	Sleep(500);
	cls();
	setmap();
	for (int i = 1; i <= siz; i++)
	{
		for (int j = 1; j <= siz; j++)
			renderMap(i, j);
		printf("\n");
	}
	cout << "ENEMY SKILL";
}
int enemymove2()
{
	bool fl = 1;
	while (fl)
	{ // 移动侦测
		char nk = getkey();
		if (nk == 'i' && (outof(ex - 1, ey)))
		{
			fl = 0;
			ex--;
		}
		if (nk == 'k' && (outof(ex + 1, ey)))
		{
			fl = 0;
			ex++;
		}
		if (nk == 'j' && (outof(ex, ey - 1)))
		{
			fl = 0;
			ey--;
		}
		if (nk == 'l' && (outof(ex, ey + 1)))
		{
			fl = 0;
			ey++;
		}
		if (nk == 'b')
		{
			debuger();
			return 0;
		}
		if (nk == 'p' && eneskilltime > 0)
		{
			fl = 0;
			eneskilltime--;
			enemyskill();
			Sleep(1000);
			return 0;
		}
		if (nk == 'o')
		{
			fl = 0;
			return 0;
		}
	}
	return 1;
}
void puttres()
{
	int anum = 0;
	while (1)
	{
		srand(time(NULL));
		ex = (abs(rand() + anum) % siz) + 1;
		srand(time(NULL));
		ey = (abs(rand() + anum) % siz) + 1;
		if (enemycheck(ex, ey) >= siz)
			return;
		anum++;
	}
}
// TODO 修改地图生成
void putblock()
{
	int ranum = 3;
	while (1)
	{
		memset(checkvis, 0, sizeof(checkvis));
		checkcnt = 0;
		ranum++; // 随机
		for (int i = 1; i <= siz; i++)
		{
			for (int j = 1; j <= siz; j++)
			{
				srand(time(NULL) + rand());
				if ((j / i * rand() + j / ranum - i * j + rand()) % ranum == 0)
					block[i][j] = 1;
				else
					block[i][j] = 0;
			}
		}
		checkroad(1, 1);
		if (checkcnt >= 3)
			return;
	}
	block[px][py] = 0;
	block[1][1] = 0;
	block[ex][ey] = 0;
}
void skill1()
{
	cls();
	int vx = x, vy = y;
	while (1)
	{
		gotoxy(0, 0);
		for (int i = 1; i <= siz; i++)
		{
			for (int j = 1; j <= siz; j++)
			{
				if (vx == i && vy == j)
					printf("\033[1;46mO \033[0m");
				else if (i <= x + 1 && j <= y + 1 && i >= x - 1 && j >= y - 1 && !(i == ex && j == ey) && !(i == px && j == py) && block[i][j] == 0)
					printf("\033[42m  \033[0m");
				if ((vx == i && vy == j) || (i <= x + 1 && j <= y + 1 && i >= x - 1 && j >= y - 1 && !(i == ex && j == ey) && !(i == px && j == py) && block[i][j] == 0))
					continue;
				if (mp[i][j] == 0)
				{
					printf(". ");
					continue;
				}
				renderMap(i, j);
			}
			printf("\n");
		}
		cout << '\n';
		if (debug == 1)
			cout << "[DEBUG MODE]:Press B to open the DEBUG MENU." << endl;
		cout << "Time Freeze! You can move freely within a range of 3 * 3. " << '\n';
		cout << "You won't fail during this period of time." << '\n';
		cout << "Press e to end this state." << '\n';
		int fl = 1;
		while (fl)
		{ // 移动侦测
			char nk = getkey();
			if (nk == 'w' && (outof(vx - 1, vy)) && (outof2(vx - 1, vy)))
			{
				fl = 0;
				vx--;
			}
			if (nk == 's' && (outof(vx + 1, vy)) && (outof2(vx + 1, vy)))
			{
				fl = 0;
				vx++;
			}
			if (nk == 'a' && (outof(vx, vy - 1)) && (outof2(vx, vy - 1)))
			{
				fl = 0;
				vy--;
			}
			if (nk == 'd' && (outof(vx, vy + 1)) && (outof2(vx, vy + 1)))
			{
				fl = 0;
				vy++;
			}
			if (nk == 'e')
			{
				x = vx;
				y = vy;
				cls();
				return;
			}
		}
	}
}
void skill2()
{
	cls();
	int vx = x, vy = y, dir = 0;
	while (1)
	{
		gotoxy(0, 0);
		for (int i = 1; i <= siz; i++)
		{
			for (int j = 1; j <= siz; j++)
			{
				if ((dir == 0 && i >= vx - 1 && i <= vx + 1 && vy == j) || (dir == 1 && j >= vy - 1 && j <= vy + 1 && vx == i))
				{
					printf("\033[31m%c\033[0m", 0xFE);
					continue;
				}
				renderMap(i, j);
			}
			printf("\n");
		}
		cout << '\n';
		if (debug == 1)
			cout << "[DEBUG MODE]:Press B to open the DEBUG MENU." << endl;
		printf("Look at the string \"\033[31m%c%c%c\033[0m\", it is a wall '\n", (char)0xFE, (char)0xFE, (char)0xFE);
		printf("You can press WASD to move it\n");
		printf("Press R to change its direction.\n");
		printf("Press E to enter.\n");
		int fl = 1;
		while (fl)
		{
			char nk = getkey();
			if (nk == 'w' && outof3(vx - 1, vy, dir))
			{

				fl = 0;
				vx--;
			}
			if (nk == 's' && outof3(vx + 1, vy, dir))
			{
				fl = 0;
				vx++;
			}
			if (nk == 'a' && outof3(vx, vy - 1, dir))
			{
				fl = 0;
				vy--;
			}
			if (nk == 'd' && outof3(vx, vy + 1, dir))
			{
				fl = 0;
				vy++;
			}
			if (nk == 'r')
			{
				dir = 1 - dir;
				fl = 0;
			}

			if (nk == 'e')
			{
				if (dir == 0)
					selfwall[vx - 1][vy] = selfwall[vx + 1][vy] = selfwall[vx][vy] = 5;
				else
					selfwall[vx][vy - 1] = selfwall[vx][vy + 1] = selfwall[vx][vy] = 5;
				cls();
				return;
			}
		}
	}
}
void choosesiz()
{
	int nowchoice = 1;
	while (1)
	{
		gotoxy(0, 0);
		cout << "Please choose the size of the game map." << '\n';
		for (int i = 1; i <= 4; i++)
		{
			if (i == nowchoice)
				cout << "-> ";
			else
				cout << "   ";
			cout << i * 10 << " * " << i * 10 << '\n';
		}
		if (5 == nowchoice)
			cout << "-> ";
		else
			cout << "   ";
		cout << "Enter the size yourself." << '\n';
		cout << "(Press S,W to choose, press E to confirm)" << '\n';
		char nchr = getkey();
		if (nchr == 's' && nowchoice < 5)
			nowchoice++;
		if (nchr == 'w' && nowchoice > 1)
			nowchoice--;
		if (nchr == 'e')
		{
			if (nowchoice == 5)
			{
				cls();
				cout << "Plese input the size:";
				cin >> siz;
				cls();
				return;
			}
			else
			{
				siz = nowchoice * 10;
				return;
			}
		}
	}
}
int menu()
{

	string str[4] = {"Game start(1P MODE)", "Game start(2P MODE)", "record", "quit"};
	int nowchoice = 0;
	while (1)
	{
		gotoxy(0, 0);
		cout << "Digital Hunting" << '\n';
		if (debug == 1)
			cout << "[DEBUG MODE]" << endl;
		for (int i = 0; i < 4; i++)
		{
			if (i == nowchoice)
				cout << "-> ";
			else
				cout << "   ";
			cout << str[i] << '\n';
		}
		cout << "(Press S,W to choose, press E to confirm)" << '\n';
		char nchr = getkey();
		if (nchr == 's' && nowchoice < 3)
			nowchoice++;
		if (nchr == 'w' && nowchoice > 0)
			nowchoice--;
		if (nchr == 'e')
		{
			cls();
			if (nowchoice == 0)
				return 0;
			else if (nowchoice == 1)
				return 1;
			else if (nowchoice == 2)
				return 2;
			else
				return 3;
		}
		if (nchr == 'b')
		{
			nchr = getkey();
			if (nchr == 't')
			{
				nchr = getkey();
				if (nchr == 'f')
				{
					debug = 1 - debug;
					cls();
				}
				else
					continue;
			}
			else
				continue;
		}
	}
}

int gamestart()
{
	if (levelnum % 5 == 0)
	{
		bless();
	}
	cls();
	cout << "LEVEL " << levelnum << '\n';
	Sleep(1000);

	skilltime1 = siz / 6 + 2 + blessnum[1];
	skilltime2 = siz / 6 + 1 + blessnum[2];
	eneskilltime = 1 + levelnum / 5;
	speed = 1 + blessnum[0];
	cls();
	cout << "Loading map......" << '\n';
	py = siz, px = siz;
	x = 1, y = 1;
	puttres();
	putblock();
	memset(selfwall, 0, sizeof(selfwall));
	int step = 0;
	bool gclose = 0;
	cls();
	while (1)
	{
		char nk = ' ';
		for (int i = 1; i <= speed; i++)
		{
			gotoxy(0, 0);
			setmap();
			ptmap();
			bool fl = 1;
			while (fl)
			{ // 移动侦测
				nk = getkey();
				if (nk == 'w' && (outof(x - 1, y)))
				{
					fl = 0;
					x--;
				}
				if (nk == 's' && (outof(x + 1, y)))
				{
					fl = 0;
					x++;
				}
				if (nk == 'a' && (outof(x, y - 1)))
				{
					fl = 0;
					y--;
				}
				if (nk == 'd' && (outof(x, y + 1)))
				{
					fl = 0;
					y++;
				}
				if (nk == 'f')
				{
					goto loop2;
				}
				if (nk == 'b')
				{
					debuger();
					goto loop;
				}
				if (nk == '1' && skilltime1 > 0)
				{
					skilltime1--;
					skill1();
					goto loop;
				}
				if (nk == '2' && skilltime2 > 0)
				{
					skilltime2--;
					skill2();
					goto loop;
				}
			}
		loop2:
			if (x == px && y == py)
			{
				char keyn = ' ';
				while (keyn != 'q' && keyn != 's')
				{
					cls();
					cout << "YOU WIN!" << '\n';
					cout << "press \"q\"to go to the next level." << '\n';
					cout << "press \"s\"to save your process." << '\n';
					keyn = getkey();
				}
				cls();
				if (keyn == 's')
				{
					char clist[50];
					sprintf(clist, "%d %d %d %d %d\n", levelnum + 1, siz, blessnum[0], blessnum[1], blessnum[2]);
					const char *cpath = prcpath.c_str();
					FILE *fp = fopen(cpath, "w");
					fprintf(fp, clist);
					fclose(fp);
					cls();
					return 2;
				}
				return 1;
			}
			if (x == ex && y == ey && invincible == 0)
			{
				char keyn = ' ';
				while (keyn != 'q')
				{
					cls();
					cout << "YOU FAIL!" << '\n';
					cout << "Your adventure ends up in LEVEL " << levelnum << '\n';
					cout << "press \"q\"to quit." << '\n';
					keyn = getkey();
				}
				cls();
				return 0;
			}
		}

		wallreduce();
		step++;
		if (eneskilltime > 0 && step % siz == 0)
		{
			eneskilltime--;
			enemyskill();
			Sleep(1000);
			continue;
		}
		srand(time(NULL));
		if (step % (rand() % 2 + 1) == 0)
		{
			srand(time(NULL));
			for (int i = 1; i <= (step + rand()) % 3 + 1 + (rand() % 2 == 0 ? levelnum / 3 : 0); i++)
			{
				enemymove();
				if (x == ex && y == ey && invincible == 0)
				{
					char keyn = ' ';
					while (keyn != 'q')
					{
						cls();
						cout << "YOU FAIL!" << '\n';
						cout << "Your adventure ends up in LEVEL " << levelnum << '\n';
						cout << "press \"q\"to quit." << '\n';
						keyn = getkey();
					}
					cls();
					return 0;
				}
			}
		}
	loop:
		if (x == px && y == py)
		{
			char keyn = ' ';
			while (keyn != 'q' && keyn != 's')
			{
				cls();
				cout << "YOU WIN!" << '\n';
				cout << "press \"q\"to go to the next level." << '\n';
				cout << "press \"s\"to save your process." << '\n';
				keyn = getkey();
			}
			cls();
			if (keyn == 's')
			{
				char clist[50];
				sprintf(clist, "%d %d %d %d %d\n", levelnum + 1, siz, blessnum[0], blessnum[1], blessnum[2]);
				const char *cpath = prcpath.c_str();
				FILE *fp = fopen(cpath, "w");
				fprintf(fp, clist);
				fclose(fp);
				cls();
				return 2;
			}
			return 1;
		}
		if (x == ex && y == ey && invincible == 0)
		{
			char keyn = ' ';
			while (keyn != 'q')
			{
				cls();
				cout << "YOU FAIL!" << '\n';
				cout << "Your adventure ends up in LEVEL " << levelnum << '\n';
				cout << "press \"q\"to quit." << '\n';
				keyn = getkey();
			}
			cls();
			return 0;
		}
	}
}
int gamestart2()
{
	cls();
	Sleep(1000);
	invincible = 0;
	skilltime1 = siz / 4 + 2;
	skilltime2 = siz / 8 + 1;
	eneskilltime = 2;
	cout << "Loading map......" << '\n';
	py = siz, px = siz;
	x = 1, y = 1;
	puttres();
	putblock();
	memset(selfwall, 0, sizeof(selfwall));
	int step = 0;
	bool gclose = 0;
	cls();
	while (1)
	{
		cls();
		gotoxy(0, 0);
		setmap();
		ptmap1();
		bool fl = 1;
		while (fl)
		{ // 移动侦测
			char nk = getkey();
			if (nk == 'w' && (outof(x - 1, y)))
			{
				fl = 0;
				x--;
			}
			if (nk == 's' && (outof(x + 1, y)))
			{
				fl = 0;
				x++;
			}
			if (nk == 'a' && (outof(x, y - 1)))
			{
				fl = 0;
				y--;
			}
			if (nk == 'd' && (outof(x, y + 1)))
			{
				fl = 0;
				y++;
			}
			if (nk == 'b')
			{
				debuger();
				goto loop2;
			}
			if (nk == '1' && skilltime1 > 0)
			{
				skilltime1--;
				skill1();
				goto loop2;
			}
			if (nk == '2' && skilltime2 > 0)
			{
				skilltime2--;
				skill2();
				goto loop2;
			}
		}
		if (x == px && y == py)
		{
			char keyn = ' ';
			while (keyn != 'q')
			{
				cls();
				cout << "Player1 WINS" << '\n';
				cout << "press \"q\"to quit." << '\n';
				keyn = getkey();
			}
			cls();
			return 0;
		}
		if (x == ex && y == ey && invincible == 0)
		{
			char keyn = ' ';
			while (keyn != 'q')
			{
				cls();
				cout << "Player2 WINS" << '\n';
				cout << "press \"q\"to quit." << '\n';
				keyn = getkey();
			}
			cls();
			return 0;
		}
		gotoxy(0, 0);
		wallreduce();
		step++;

		srand(time(NULL));
		if (1)
		{
			srand(time(NULL));
			int enestept = (step + rand()) % 3 + 1;
			for (int i = 1; i <= enestept; i++)
			{
				setmap();
				gotoxy(0, 0);
				ptmap2(enestept + 1 - i);
				if (enemymove2() == 0)
				{
					break;
				}
				if (x == ex && y == ey && invincible == 0)
				{
					char keyn = ' ';
					while (keyn != 'q')
					{
						cls();
						cout << "Player2 WINS" << '\n';
						cout << "press \"q\"to quit." << '\n';
						keyn = getkey();
					}
					cls();
					return 0;
				}
			}
		}
	loop2:
		gotoxy(0, 0);
	}
}
void seerecord()
{
	const char *cpath = recpath.c_str();
	FILE *fp = fopen(cpath, "r");
	if (fp == NULL)
	{
		ofstream file(cpath);
		file.close();
		cout << "No record." << '\n';
		cout << "(Press E to quit)" << '\n';
		while (1)
		{
			char keyn = getkey();
			if (keyn == 'e')
			{
				cls();
				return;
			}
		}
	}
	char recs[1024];
	stack<int> numsta;
	stack<int> sizsta;
	stack<string> timsta;
	while (!feof(fp))
	{
		fscanf(fp, "%s", recs);
		int recnum = 0, recflg = 0, recsiz = 0;
		string rectim = "aaaaaaaaaa";
		for (int i = 0; i <= 9; i++)
			rectim[i] = recs[i];
		for (int i = 10; recs[i] != ')'; i++)
		{
			if (recs[i] == '(')
			{
				recflg = 1;
				continue;
			}
			if (recs[i] == ')')
			{
				recflg = 0;
				continue;
			}

			if (recflg == 1)
			{
				recsiz = recsiz * 10 + (recs[i] - '0');
			}
			else
			{
				recnum = recnum * 10 + (recs[i] - '0');
			}
		}
		numsta.push(recnum);
		timsta.push(rectim);
		sizsta.push(recsiz);
	}
	sizsta.pop();
	numsta.pop();
	timsta.pop();
	if (timsta.empty())
		cout << "No record." << '\n';
	while (!timsta.empty())
	{
		int nowsiz = sizsta.top(), nownum = numsta.top();
		string nowtim = timsta.top();
		sizsta.pop();
		numsta.pop();
		timsta.pop();
		cout << nowtim << ": LEVEL" << nownum << " (map size: " << nowsiz << " * " << nowsiz << " ) " << '\n';
	}
	fclose(fp);
	cout << "(Press E to quit)" << '\n';
	while (1)
	{
		char keyn = getkey();
		if (keyn == 'e')
		{
			cls();
			return;
		}
	}
}
int onClear()
{
	cls();
	char nk = ' ';
	while (nk != 'q' && nk != 'g')
	{
		gotoxy(0, 0);
		cout << "Congratulations, you have cleared this game!" << endl;
		cout << "Thank you for play my game." << endl;
		cout << "This game was made in HSAS OI by 'bai_the_fool' and 'ALittleZhi'." << endl;
		cout << "Now you can choose to quit,or you can move to the ENDLESS MODE." << endl;
		cout << "(Press Q to quit,Press G to move to the ENDLESS MODE!)";
		nk = getkey();
	}
	if (nk == 'q')
		return 0;
	else
		return 1;
}
int main()
{
	system("chcp 437"); // 使用此行以应用OEM字符集，显示完整方块

	CreateDirectory(".\\DigitalHunting", NULL);
	const char *cpath = prcpath.c_str();
	ifstream fp(cpath);
	if (fp)
	{
		if (fp.peek() == std::ifstream::traits_type::eof())
		{
			fp.close();
			FILE *fp = fopen(cpath, "w");
			fprintf(fp, "NULL");
			fclose(fp);
		}
	}
	else
	{
		fp.close();
		std::ofstream file(cpath);
		file.close();
		FILE *fp = fopen(cpath, "w");
		fprintf(fp, "NULL");
		fclose(fp);
	}

	settrans();
	while (1)
	{
		cls();
		int menuch = menu();
		cls();
		if (menuch == 0)
		{
			const char *cpath = prcpath.c_str();
			FILE *fp = fopen(cpath, "r");
			char recs[1024];
			fscanf(fp, "%s", recs);
			string instr0(recs);
			if (instr0 != "NULL")
			{
				cout << "There's already a game process.";
				Sleep(500);
				cls();

				int i = 0, rnum = 0;
				string instr(recs);
				for (int i = 0; i < instr.length(); i++)
					rnum = rnum * 10 + (instr[i] - '0');
				levelnum = rnum;

				fscanf(fp, "%s", recs);
				i = 0, rnum = 0;
				string instr2(recs);
				for (int i = 0; i < instr2.length(); i++)
					rnum = rnum * 10 + (instr2[i] - '0');
				siz = rnum;

				for (int j = 0; j < 3; j++)
				{
					fscanf(fp, "%s", recs);
					i = 0, rnum = 0;
					string instr2(recs);
					for (int i = 0; i < instr2.length(); i++)
						rnum = rnum * 10 + (instr2[i] - '0');
					blessnum[j] = rnum;
				}
			}
			else
			{
				choosesiz();
				for (int j = 0; j < 3; j++)
				{
					blessnum[j] = 0;
				}
				levelnum = 1;
			}
			fclose(fp);
			int result;
			invincible = 0;
			while (1)
			{

				result = gamestart();
				if (result == 0 || result == 1)
				{
					cpath = prcpath.c_str();
					FILE *fp = fopen(cpath, "w");
					fprintf(fp, "NULL");
					fclose(fp);
				}
				if (result == 1 && levelnum == 20)
				{
					int clearchoice = onClear();
					cls();
					cout << "Saving record......" << '\n';
					cpath = recpath.c_str();
					fp = fopen(cpath, "a");
					SYSTEMTIME time;
					GetLocalTime(&time);
					char clist[50];
					sprintf(clist, "%04d/%02d/%02d%d(%d)\n", time.wYear, time.wMonth, time.wDay, levelnum, siz);
					fprintf(fp, clist);
					fclose(fp);
					cls();
					if (clearchoice == 0)
						break;
				}
				if (result == 0 || result == 2)
					break;

				levelnum++;
			}
			if (result == 0 || result == 1)
			{
				cout << "Saving record......" << '\n';
				cpath = recpath.c_str();
				fp = fopen(cpath, "a");
				SYSTEMTIME time;
				GetLocalTime(&time);
				char clist[50];
				sprintf(clist, "%04d/%02d/%02d%d(%d)\n", time.wYear, time.wMonth, time.wDay, levelnum, siz);
				fprintf(fp, clist);
				fclose(fp);
			}
			cls();
		}
		if (menuch == 1)
		{
			choosesiz();
			gamestart2();
		}
		if (menuch == 2)
		{
			seerecord();
		}
		if (menuch == 3)
		{
			return 0;
		}
	}
}