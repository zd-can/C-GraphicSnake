#include "stdafx.h"
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define U 1
#define D 2
#define L 3 
#define R 4 
#define ESC 27
#define ENTER 13
#define LEFT 75
#define UP 72
#define RIGHT 77
#define DOWN 80
#define FLOOR 0
#define WALL 1
#define SNAKE 2
#define FOOD 3
#define MINE 4
#define POISON 5

int poison_x, poison_y;
int rand_num_x[10] = { 0 }, rand_num_y[10] = { 0 };
int end = 0, length = 0, score = 0, count = 0;
IMAGE wall,food,mine,poison;
IMAGE mapnow,map1, map2, map3,body,body1,body2,body3;
int map[20][20];
int direction = R;
int sleeptime = 450;
FILE * mem;
FILE *fp;

void Pos(int x, int y);
void HideCursor();
void Choose();
void ChooseLevel();
void Level_1();
void Level_2();
void Level_3();
void MarkMap();
void AddSnake(int a, int b);
void DeleteTail();
void CreateSnake();
void CreateWall();
void CreateFood();
void CreateMine();
void CreatePoison();
void Shine();
void SnakeMove();
void pause();
void RunGame();
void EndGame();
void Save();
void Load();
void ScoreSave();
void ScoreLoad();

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct snake {
	int x;
	int y;
	struct snake *previous;
	struct snake *next;
}*head = NULL, *tail = NULL;

struct savescore {
	char name[20];
	int grade;
}person[3], temp, t;

void Pos(int x, int y) {     //设置光标位置
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void HideCursor() {    //隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void Choose() {
	IMAGE choose1,choose2,choose3,choose4;
	char d;
	int c=200,m;
	loadimage(&choose1, _T("C:\\Users\\ASUS\\图形界面\\images\\选择新游戏.jpg"));
	loadimage(&choose2, _T("C:\\Users\\ASUS\\图形界面\\images\\选择继续游戏.jpg"));
	loadimage(&choose3, _T("C:\\Users\\ASUS\\图形界面\\images\\选择排行榜.jpg"));
	loadimage(&choose4, _T("C:\\Users\\ASUS\\图形界面\\images\\选择退出游戏.jpg"));
	putimage(0, 0, &choose1);
	do{
	d = _getch();
	switch(d) {
	case LEFT:c = c - 1; break;
	case RIGHT:c = c + 1; break;
	default:break;
	}
	m =c % 4;
	switch (m) {
	case 0:putimage(0, 0, &choose1); break;
	case 1:putimage(0, 0, &choose2); break;
	case 2:putimage(0, 0, &choose3); break;
	case 3:putimage(0, 0, &choose4); break;
	default:break;
	}
	} while (d != ENTER);
	switch (m) {
	case 0:ChooseLevel(); break;
	case 1:Load(); ScoreLoad(); CreateWall();MarkMap();RunGame();system("pause"); break;
	case 2:ScoreLoad(); break;
	case 3:exit(0); break;
	default:break;
	}
	 getchar();
}

void ChooseLevel() {
	IMAGE choose1, choose2, choose3, describe;
	char d;
	int c = 210, m;
	loadimage(&choose1, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡选择简单.jpg"));
	loadimage(&choose2, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡选择一般.jpg"));
	loadimage(&choose3, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡选择困难.jpg"));
	loadimage(&describe, _T("C:\\Users\\ASUS\\图形界面\\images\\游戏说明.jpg"));
	putimage(0, 0, &choose1);
	do {
		d = _getch();
		switch (d) {
		case LEFT:c = c - 1; break;
		case RIGHT:c = c + 1; break;
		default:break;
		}
		m = c % 3;
		switch (m) {
		case 0:putimage(0, 0, &choose1); break;
		case 1:putimage(0, 0, &choose2); break;
		case 2:putimage(0, 0, &choose3); break;
		default:break;
		}
	} while (d != ENTER);
	putimage(0, 0, &describe);
	_getch();
	switch (m) {
	case 0:Level_1(); break;
	case 1:Level_2(); break;
	case 2:Level_3(); break;
	default:break;
	}

	getchar();

}

void MarkMap(){
	int i;
	for (i = 0; i < 20; i++) { 
		map[i][0] = WALL; 
		map[0][i] = WALL;
		map[i][19] = WALL;
		map[19][i] = WALL;
	}
}

void AddSnake(int a, int b) {    //增加一节蛇头 
	snake *temp = (snake *)malloc(sizeof(snake));
	temp->x = a;
	temp->y = b;
	temp->next = NULL;
	if (head == NULL) {
		tail = head = temp;
		head->next = NULL;
	}
	else {
		head->next = temp;
		temp->previous = head;
		head = head->next;
	}
	map[b][a] = SNAKE;
	putimage((10 +a ) * 16, (5+b) * 16, 16, 16, &body, (10+a) * 16, (5+b) * 16);
	length++;
}

void DeleteTail() {    //删除尾节点 
	struct snake *temp = NULL;
	putimage((10 + tail->x) * 16, (5 + tail->y) * 16, 16, 16, &mapnow, (10 + tail->x) * 16, (5 + tail->y) * 16);
	map[tail->y][tail->x] = FLOOR;
	temp = tail;
	tail = tail->next;
	tail->previous = NULL;
	free((void *)temp);
	length--;
}

void CreateSnake() {     //初始化蛇身 
	AddSnake(4, 4);
	AddSnake(5, 4);
	AddSnake(6, 4);
	AddSnake(7, 4);
	length = 4;
}

void CreateWall() {
	int i;
	for (i = 0; i < 20; i++) {
		putimage((10 + i) * 16, 5 * 16, 16, 16, &wall, (10 + i) * 16, 5 * 16);
		putimage((10 + i) * 16, 24 * 16, 16, 16, &wall, (10 + i) * 16, 24 * 16);
		putimage(10 * 16, (5 + i) * 16, 16, 16, &wall, 10 * 16, (5 + i) * 16);
		putimage(29 * 16, (5 + i) * 16, 16, 16, &wall, 29 * 16, (5 + i) * 16);
	}
}

void CreateFood() {
	int food_x, food_y;
	srand((unsigned int)time(0));
	food_x = rand() % 18 + 1;
	food_y = rand() % 18 + 1;
	while (map[food_y][food_x] != FLOOR) {
		food_x = rand() % 18 + 1;
		food_y = rand() % 18 + 1;
	}
	map[food_y][food_x] = FOOD;
	putimage((10 + food_x) * 16, (5 + food_y) * 16, 16, 16, &food, (10 + food_x) * 16, (5 + food_y) * 16);
}

void CreateMine() {
	int mine_x, mine_y;
	srand((unsigned int)time(0));
	mine_x = rand() % 18 + 1;
	mine_y = rand() % 18 + 1;
	while (map[mine_y][mine_x] != FLOOR) {
		mine_x = rand() % 18 + 1;
		mine_y = rand() % 18 + 1;
	}
	map[mine_y][mine_x] = MINE;
	putimage((10 + mine_x) * 16, (5 + mine_y) * 16, 16, 16, &mine, (10 + mine_x) * 16, (5 + mine_y) * 16);
}

void CreatePoison() {
	int count_x = 0, count_y = 0;
	int j = 0, flag = 1, i;
	srand((unsigned)time(NULL));
	while (count_x < 10) {
		poison_x = rand() % 18 + 1;
		for (j = 0; j < 10; j++) {
			if (poison_x == rand_num_x[j]) {
				flag = 0;
				break;
			}
			else flag = 1;
		}
		if (flag) {
			rand_num_x[count_x] = poison_x;
			count_x++;
		}
	}
	while (count_y < 10) {
		poison_y = rand() % 18 + 1;
		for (j = 0; j < 10; j++) {
			if (poison_y == rand_num_y[j]) {
				flag = 0;
				break;
			}
			else flag = 1;
		}
		if (flag) {
			rand_num_y[count_y] = poison_y;
			count_y++;
		}
	}
	if (count % 2 == 0) {
		for (j = 0; j < 10; j++) {
			if (map[rand_num_y[j]][rand_num_x[j]] == FLOOR) {
				map[rand_num_y[j]][rand_num_x[j]] = POISON;
				putimage((10 + rand_num_x[j]) * 16, (5 + rand_num_y[j]) * 16, 16, 16, &poison, (10 + rand_num_x[j]) * 16, (5 + rand_num_y[j]) * 16);
			}
		}
	}
	else if (count != 2) {
		for (i = 1; i < 19; i ++)
			for (j = 1; j < 19; j++) {
				if (map[j][i] == POISON) {
					putimage((10 + i) * 16, (5 + j) * 16, 16, 16, &mapnow, (10 + i) * 16, (5 + j) * 16);
					map[j][i] = FLOOR;
				}
			}
	}
}

void Shine() {   //毒草闪烁函数
	int i, j;
	if (count <= 100) {
		if (count % 2 == 0) {
			for (j = 0; j < 10; j++) {
				if (map[rand_num_y[j]][rand_num_x[j]] == FLOOR) {
					map[rand_num_y[j]][rand_num_x[j]] = POISON;
					putimage((10 + rand_num_x[j]) * 16, (5 + rand_num_y[j]) * 16, 16, 16, &poison, (10 + rand_num_x[j]) * 16, (5 + rand_num_y[j]) * 16);
				}
			}
		}
		else if (count != 2) {
			for (i = 1; i < 19; i++)
				for (j = 1; j < 19; j++) {
					if (map[j][i] == POISON) {
						putimage((10 + i) * 16, (5 + j) * 16, 16, 16, &mapnow, (10 + i) * 16, (5 + j) * 16);
						map[j][i] = FLOOR;
					}
				}
		}
	}
	else if (count > 100) {
		for (i = 1; i < 19; i ++)
			for (j = 1; j < 19; j++) {
				if (map[j][i] == POISON) {
					putimage((10 + i) * 16, (5 +j) * 16, 16, 16, &mapnow, (10 + i) * 16, (5 + j) * 16);
					map[j][i] = FLOOR;
				}
			}
	}
}

void SnakeMove() {
	int a, b, i, j;
	a = head->x;
	b = head->y;
	switch (direction) {
	case(U):b--; break;
	case(D):b++; break;
	case(L):a--; break;
	case(R):a ++; break;
	}
	int t = map[b][a];
	if (t == FLOOR) {
		AddSnake(a, b);
		DeleteTail();
	}
	else if (t == WALL) {
		end = 2;
		EndGame();
	}
	else if (t == FOOD) {
		AddSnake(a, b);
		CreateFood();
		score = score + 20;
		map[b][a] = SNAKE;
	}
	else if (t == MINE) {
		if (length == 1) {
			end = 3;
			score = score - 50;
			EndGame();
		}
		else {
			AddSnake(a, b);
			map[b][a] = SNAKE;
			if (length % 2 == 0) i = length / 2;
			else i = (length - 1) / 2;
			j = length;
			while (j > i) {
				DeleteTail();
				j--;
			}
			score = score - 50;
			CreateMine();
		}
	}
	else if (t == POISON) {
		if (length == 1) {
			end = 4;
			score = score - 5;
			EndGame();
		}
		else {
			AddSnake(a, b);
			map[b][a] = SNAKE;
			DeleteTail();
			DeleteTail();
			score = score - 10;
		}
	}
	else if (t == SNAKE) {
		end = 1;
		EndGame();
	}

}

void pause() {
	while (1) {
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE)) {
			break;
		}
	}
}

void RunGame() {
	if (sleeptime == 450) Level_1();
	if (sleeptime == 250) Level_2();
	if (sleeptime == 100) Level_3();
}

void Level_1() {
	body = body1;
	IMAGE pass11,pass12;
	sleeptime = 450;
	MarkMap();
	mapnow = map1;
	putimage(0, 0, &map1);
	CreateWall();
	CreateSnake();
	CreateFood();CreateFood();CreateFood();
	CreateMine();CreateMine();CreateMine();
	CreatePoison();
	while (length<10) {
		count++;
		Shine();
		SnakeMove();
		Sleep(sleeptime);
		if (GetAsyncKeyState(VK_UP) && direction != D)direction = U;	
		else if (GetAsyncKeyState(VK_DOWN) && direction != U)direction = D;	
		else if (GetAsyncKeyState(VK_LEFT) && direction != R)direction = L;	
		else if (GetAsyncKeyState(VK_RIGHT) && direction != L)direction = R;	
		else  if(GetAsyncKeyState(VK_SPACE))pause();	
		if (_kbhit()) {
			if (_getch() == ESC) {//按ESC存档并退出 
				Save();    //存档
				free(head);
				exit(0);
			}
		}
	}
	if (length >= 10) {
		sleeptime = 200;
		loadimage(&pass11, _T("C:\\Users\\ASUS\\图形界面\\images\\通过第一关1.jpg"));
		loadimage(&pass12, _T("C:\\Users\\ASUS\\图形界面\\images\\通过第一关2.jpg"));
		putimage(0, 0, &pass11);
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &pass11); break;
			case 1:putimage(0, 0, &pass12); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0) {
			int i, j;
			for (i = 1; i < 19; i++)
				for (j = 1; j < 19; j++)
					map[j][i] = FLOOR;
			direction = R;
			snake *p, *t;
			p = tail;
			p->previous = NULL;
			for (i = 1; i <= length; i++) {
				t = p;
				p = p->next;
				free((void *)t);
			}
			head = NULL;
			Level_2();
		}
		else if (m == 1) {
			Save();    //存档
			free(head);
			exit(0);
		}
	}
}

void Level_2() {
	putimage(0, 0, &map2);
	body = body2;
	IMAGE pass21, pass22;
	sleeptime = 250;
	MarkMap();
	mapnow = map2;	
	CreateWall();
	CreateSnake();
	CreateFood(); CreateFood(); CreateFood(); CreateFood();
	CreateMine(); CreateMine(); CreateMine(); CreateMine(); CreateMine();
	CreatePoison();
	while (length<10) {
		count++;
		Shine();
		SnakeMove();
		Sleep(sleeptime);
		if (GetAsyncKeyState(VK_UP) && direction != D)direction = U;
		else if (GetAsyncKeyState(VK_DOWN) && direction != U)direction = D;
		else if (GetAsyncKeyState(VK_LEFT) && direction != R)direction = L;
		else if (GetAsyncKeyState(VK_RIGHT) && direction != L)direction = R;
		else if (GetAsyncKeyState(VK_SPACE))pause();
		if (_kbhit()) {
			if (_getch() == ESC) {//按ESC存档并退出 
				Save();    //存档
				free(head);
				exit(0);
			}
		}
	}
	if (length >= 10) {
		sleeptime = 100;
		loadimage(&pass21, _T("C:\\Users\\ASUS\\图形界面\\images\\通过第二关1.jpg"));
		loadimage(&pass22, _T("C:\\Users\\ASUS\\图形界面\\images\\通过第二关2.jpg"));
		putimage(0, 0, &pass21);
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &pass21); break;
			case 1:putimage(0, 0, &pass22); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0) {
			int i, j;
			for (i = 1; i < 19; i++)
				for (j = 1; j < 19; j++)
					map[j][i] = FLOOR;
			direction = R;
			snake *p, *t;
			p = tail;
			p->previous = NULL;
			for (i = 1; i <= length; i++) {
				t = p;
				p = p->next;
				free((void *)t);
			}
			head = NULL;
			Level_3();
		}
		else if (m == 1) {
			Save();    //存档
			free(head);
			exit(0);
		}
	}
}

void Level_3() {
	putimage(0, 0, &map3);
	body = body3;
	IMAGE pass31, pass32;
	sleeptime = 100;
	MarkMap();
	mapnow = map3;
	CreateWall();
	CreateSnake();
	CreateFood(); CreateFood(); CreateFood(); CreateFood(); CreateFood();
	CreateMine(); CreateMine(); CreateMine(); CreateMine(); CreateMine(); CreateMine(); CreateMine();
	CreatePoison();
	while (length<10) {
		count++;
		Shine();
		SnakeMove();
		Sleep(sleeptime);
		if (GetAsyncKeyState(VK_UP) && direction != D)direction = U;
		else if (GetAsyncKeyState(VK_DOWN) && direction != U)direction = D;
		else if (GetAsyncKeyState(VK_LEFT) && direction != R)direction = L;
		else if (GetAsyncKeyState(VK_RIGHT) && direction != L)direction = R;
		else if (GetAsyncKeyState(VK_SPACE))pause();
		if (_kbhit()) {
			if (_getch() == ESC) {//按ESC存档并退出 
				Save();    //存档
				free(head);
				exit(0);
			}
		}
	}
	if (length >= 10) {
		sleeptime = 100;
		loadimage(&pass31, _T("C:\\Users\\ASUS\\图形界面\\images\\通过第三关1.jpg"));
		loadimage(&pass32, _T("C:\\Users\\ASUS\\图形界面\\images\\通过第三关2.jpg"));
		putimage(0, 0, &pass31);
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &pass31); break;
			case 1:putimage(0, 0, &pass32); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0) {
			int i, j;
			for (i = 1; i < 19; i++)
				for (j = 1; j < 19; j++)
					map[j][i] = FLOOR;
			direction = R;
			snake *p, *t;
			p = tail;
			p->previous = NULL;
			for (i = 1; i <= length; i++) {
				t = p;
				p = p->next;
				free((void *)t);
			}
			head = NULL;
			ScoreLoad();
		}
		else  {
			Save();    //存档
			free(head);
			exit(0);
		}
	}
}

void EndGame() {
	int option = 1;
	IMAGE end11,end21, end31, end41,end12,end22,end32,end42;
	loadimage(&end11, _T("C:\\Users\\ASUS\\图形界面\\images\\咬1.jpg"));
	loadimage(&end21, _T("C:\\Users\\ASUS\\图形界面\\images\\撞墙1.jpg"));
	loadimage(&end31, _T("C:\\Users\\ASUS\\图形界面\\images\\炸死1.jpg"));
	loadimage(&end41, _T("C:\\Users\\ASUS\\图形界面\\images\\毒死1.jpg"));
	loadimage(&end12, _T("C:\\Users\\ASUS\\图形界面\\images\\咬2.jpg"));
	loadimage(&end22, _T("C:\\Users\\ASUS\\图形界面\\images\\撞墙2.jpg"));
	loadimage(&end32, _T("C:\\Users\\ASUS\\图形界面\\images\\炸死2.jpg"));
	loadimage(&end42, _T("C:\\Users\\ASUS\\图形界面\\images\\毒死2.jpg"));
	ScoreSave();
	if (end == 1) { 
		putimage(0, 0, &end11); 
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &end11); break;
			case 1:putimage(0, 0, &end12); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0) ScoreLoad();
		else exit(0);
	}
	else if (end == 2) {
		putimage(0, 0, &end21);
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &end21); break;
			case 1:putimage(0, 0, &end22); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0)  ScoreLoad();
		else exit(0);
	}
	else if (end == 3) {
		putimage(0, 0, &end31);
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &end31); break;
			case 1:putimage(0, 0, &end32); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0)  ScoreLoad();
		else exit(0);
	}
	else if (end == 4) {
		putimage(0, 0, &end41);
		int d, m, c = 200;
		do {
			d = _getch();
			switch (d) {
			case LEFT:c = c - 1; break;
			case RIGHT:c = c + 1; break;
			default:break;
			}
			m = c % 2;
			switch (m) {
			case 0:putimage(0, 0, &end41); break;
			case 1:putimage(0, 0, &end42); break;
			default:break;
			}
		} while (d != ENTER);
		if (m == 0)  ScoreLoad();
		else exit(0);
	}
}

void Save() {
	if ((mem = fopen("memory.txt", "wb")) == NULL) {
		printf("无法打开memory.txt!\n");
		fclose(mem);
	}
	else {
		fwrite(&sleeptime, sizeof(sleeptime), 1, mem);
		fwrite(&score, sizeof(score), 1, mem);
		fclose(mem);
	}
}

void Load() {
	if ((mem = fopen("memory.txt", "rb")) == NULL) {
		Pos(12, 8);
		printf("无存档");
		Pos(12, 9);
		fclose(mem);
	}
	else {
		fread(&sleeptime, sizeof(sleeptime), 1, mem);
		fread(&score, sizeof(score), 1, mem);
		fclose(mem);
	}
}

void ScoreSave() {
	int i;
	if ((fp = fopen("排行榜.txt", "ab+")) == NULL) {
		Pos(12, 5);
		printf("无法打开排行榜.txt!\n");
		fclose(fp);
	}
	else {
		fp = fopen("排行榜.txt", "ab+");
		fread(&person[0], sizeof(person[0]), 1, fp);
		fread(&person[1], sizeof(person[1]), 1, fp);
		fread(&person[2], sizeof(person[2]), 1, fp);
		fclose(fp);
		if (person[0].grade == 0){
			person[0].grade = score;
			person[1].grade = 0;
			person[2].grade = 0;
			strcpy(person[0].name, temp.name);
		}
		else if (person[1].grade == 0) {
			person[1].grade = score;
			person[2].grade = 0;
			strcpy(person[1].name, temp.name);
		}
		else if (person[2].grade == 0) {
			person[2].grade = score;
			strcpy(person[2].name, temp.name);
		}
		else if (score > person[2].grade) {
			person[2].grade = score;
			strcpy(person[2].name, temp.name);
		}
		if (person[2].grade > person[1].grade) {
			t.grade = person[2].grade;
			person[2].grade = person[1].grade;
			person[1].grade = t.grade;
			strcpy(t.name, person[2].name);
			strcpy(person[2].name, person[1].name);
			strcpy(person[1].name, t.name);
		}
		if (person[1].grade > person[0].grade) {
			t.grade = person[1].grade;
			person[1].grade = person[0].grade;
			person[0].grade = t.grade;
			strcpy(t.name, person[1].name);
			strcpy(person[1].name, person[0].name);
			strcpy(person[0].name, t.name);
		}
		if ((fp = fopen("排行榜.txt", "wb")) == NULL) {
			Pos(12, 5);
			printf("无法打开排行榜.txt!");
			fclose(fp);
		}
		else {
			for (i = 0; i < 3; i++)
				fwrite(&person[i], sizeof(savescore), 1, fp);
			fclose(fp);
		}
	}
	Pos(12, 7);
	system("cls");
}

void ScoreLoad() {
	system("cls");
	int i, flag = 0;
	if ((fp = fopen("排行榜.txt", "rb+")) == NULL) {
		printf("\n\n\n\t\t\t\t  暂无游戏记录\n");
		fclose(fp);
	}
	else {
		fp = fopen("排行榜.txt", "rb+");
		for (i = 0; i < 3; i++)
			fread(&person[i], sizeof(savescore), 1, fp);
		Pos(50, 5);
		printf("   排行榜\n");
		for (i = 0; i < 3; i++) {
			Pos(50, 7 + 2 * i);
			printf("第%d名  %-10s：%6d", i + 1, person[i].name, person[i].grade);
		}
		fclose(fp);
	}
	getchar();
}

int main()
{
	Pos(12, 5);
	printf("请输入玩家的名字：");
	Pos(15, 7);
	scanf("%s", temp.name);
	IMAGE start;
	HideCursor();
	MarkMap();
	loadimage(&map1, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡1地图1.jpg"));
	loadimage(&body1, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡1蛇身.jpg"));
	loadimage(&map2, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡2地图1.jpg"));
	loadimage(&body2, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡2蛇身.jpg"));
	loadimage(&map3, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡3地图1.jpg"));
	loadimage(&body3, _T("C:\\Users\\ASUS\\图形界面\\images\\关卡3蛇身.jpg"));
	loadimage(&wall, _T("C:\\Users\\ASUS\\图形界面\\images\\墙.jpg"));
	loadimage(&food, _T("C:\\Users\\ASUS\\图形界面\\images\\食物.jpg"));
	loadimage(&mine, _T("C:\\Users\\ASUS\\图形界面\\images\\地雷.jpg"));
	loadimage(&poison, _T("C:\\Users\\ASUS\\图形界面\\images\\毒草.jpg"));
	initgraph(800, 500);
	loadimage(&start, _T("C:\\Users\\ASUS\\图形界面\\images\\开始界面.jpg"));
	putimage(0, 0, &start);
	_getch();
	Choose();
    return 0;
}

