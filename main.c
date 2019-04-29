#include <ncurses.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WHITE_BLACK 8
#define WHITE_RED 9
#define WHITE_GREEN 10
#define WHITE_YELLOW 11
#define WHITE_BLUE 12
#define WHITE_MAGENTA 13
#define WHITE_CYAN 14
#define WHITE_WHITE 15

int getlen(), mrand(), find();

typedef struct vector {
	int x;
	int y;
} vector;

#define vect_new(x, y) (vector){x, y}
#define vect_equals(pos1, pos2) (pos1.y == pos2.y && pos1.x == pos2.x)

vector vect_norm();

int WIN_H, WIN_W;
int PLAYER_COLOR = COLOR_RED;

#include "weapons.h"

typedef struct player {
	int life;
	int maxlife;
	WEAPON weapon;
	int potions;
	int level;
	vector pos;
} PLAYER;

#include "enemies.h"
#include "shop.h"
#include "arena.h"
#include "main_menu.h"

int main()
{
	time_t t;
	srand((unsigned) time(&t));

	initscr();
	noecho();
	start_color();
	curs_set(0);
	keypad(stdscr, TRUE);
	cbreak();

	for (int i = 0; i < 8; ++i)
		init_pair(i, i, COLOR_BLACK);
	
	for (int i = 8; i < 16; ++i)
		init_pair(i, i, COLOR_WHITE);

	getmaxyx(stdscr, WIN_H, WIN_W);

	int window_too_small = false;

	if (WIN_W >= 80 && WIN_H >= 35)
		show_main_menu();
	else
		window_too_small = true;

	clear();
	endwin();

	if (window_too_small)
		printf("\nYour terminal window is too small! (Needs to be at least 80x35)\n\n");

	return(0);
}

vector vect_norm(pos)
vector pos;
{
	if (pos.x >= 1)
		pos.x = 1;
	else if (pos.x <= -1)
		pos.x = -1;
	else
		pos.x = 0;

	if (pos.y >= 1)
		pos.y = 1;
	else if (pos.y <= -1)
		pos.y = -1;
	else
		pos.y = 0;

	return(pos);
}

int mrand(n1, n2)
int n1, n2;
{
	int out = -1;

	++n1;
	++n2;

	while (out < n1)
		out = rand() % n2+1;

	return(out-1);
}

int find(s1, s2)
char *s1, *s2;
{
	int i;

	for (i=0; i <= getlen(s1); ++i)
	{
		if (s1[i] == s2[0])
		{
			for (int a = 1; a < getlen(s2); ++a)
			{
				if (s1[i+a] != s2[i+a])
					break;
			}
			return(i);
		}
	}

	return(-1);
}

int getlen(string)
char string[];
{
	int i;

	for (i = 0; string[i] != '\0'; ++i);

	return(i);
}