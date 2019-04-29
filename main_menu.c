#include <ncurses.h>

#define ENTER 10
#define ESC 27

void show_main_menu()
{
	static int selected = 1;
	int input;
	int quit = false;

	print_main_menu(selected);

	while ((input = getch()) != ESC)
	{
		if (input == KEY_DOWN && selected < 2)
			++selected;
		else if (input == KEY_UP && selected > 1)
			--selected;
		else if (input == ENTER)
		{
			if (selected == 2)
			{
				quit = show_color_menu();
			}
			else
			{
				start_arena();
			}
		}

		if (quit)
			return;

		print_main_menu(selected);
	}
}

int show_color_menu()
{
	int input;
	int selected = PLAYER_COLOR;

	print_color_menu(selected);

	while ((input = getch()) != ESC)
	{
		if (input == KEY_RIGHT && selected < 7)
			++selected;
		else if (input == KEY_LEFT && selected > 1)
			--selected;
		else if (input == KEY_UP)
			selected = 1;
		else if (input == KEY_DOWN)
			selected = 8;
		else if (input == ENTER)
		{
			if (selected == 8)
			{
				show_main_menu();
				return(false);
			}
			else
				PLAYER_COLOR = selected;
		}

		print_color_menu(selected);
	}

	return(true);
}

void print_color_menu(selected)
int selected;
{
	clear();

	attron(A_BOLD | COLOR_PAIR(COLOR_GREEN));
	mvprintw(WIN_H/6, (WIN_W/2)-10, "The Healthy Gladiator");
	attroff(COLOR_PAIR(COLOR_GREEN));

	attron(COLOR_PAIR(PLAYER_COLOR));
	mvprintw(WIN_H/3, (WIN_W/2)-7, "Character Color");
	attroff(COLOR_PAIR(PLAYER_COLOR));

	for (int i = 1; i < 8; ++i)
	{
		int color = i;

		if (selected == i)
			color += 8;
			
		attron(COLOR_PAIR(color));
		mvprintw(WIN_H/3+2, (WIN_W/2)-(8-(i*2)), "@");
		attroff(COLOR_PAIR(color));
	}

	if (selected == 8)
		attron(COLOR_PAIR(WHITE_BLUE));
	else
		attron(COLOR_PAIR(COLOR_CYAN));

	mvprintw((WIN_H/3)+5, (WIN_W/2)-3, "[Back]");

	if (selected == 8)
		attroff(A_BOLD | COLOR_PAIR(WHITE_BLUE));
	else
		attroff(A_BOLD | COLOR_PAIR(COLOR_CYAN));

	mvprintw(WIN_H-1, 0, "Press ESC to exit. Use the arrow keys to navigate and ENTER to submit");

	refresh();
}

void print_main_menu(selected)
int selected;
{
	clear();

	attron(A_BOLD | COLOR_PAIR(COLOR_GREEN));
	mvprintw(WIN_H/6, (WIN_W/2)-10, "The Healthy Gladiator");
	attroff(COLOR_PAIR(COLOR_GREEN));
	
	if (selected == 1)
		attron(COLOR_PAIR(WHITE_BLUE));
	else
		attron(COLOR_PAIR(COLOR_CYAN));

	mvprintw(WIN_H/3, (WIN_W/2)-3, "[Play]");

	if (selected == 1)
	{
		attroff(COLOR_PAIR(WHITE_BLUE));
		attron(COLOR_PAIR(COLOR_CYAN));
	}
	else
		attron(COLOR_PAIR(WHITE_BLUE));

	mvprintw((WIN_H/3)+2, (WIN_W/2)-7, "[Player Color]");

	if (selected == 2)
		attroff(A_BOLD | COLOR_PAIR(WHITE_BLUE));
	else
		attroff(A_BOLD | COLOR_PAIR(COLOR_CYAN));

	mvprintw(WIN_H-1, 0, "Press [ESC] to exit. Use the arrow keys to navigate and [ENTER] to submit");

	wrefresh(stdscr);
}