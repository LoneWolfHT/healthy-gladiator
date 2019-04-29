#include <ncurses.h>

#define HP_PRICE 3
#define ESC 27
#define ENTER 10

WINDOW *shop_win;
int min_selected;

void show_shop(player)
PLAYER *player;
{
	min_selected = 0;

	WEAPON weapons[] = {WEAPON_DAGGER, WEAPON_SHORTSWORD, WEAPON_SPEAR, WEAPON_SWORD, WEAPON_AXE, WEAPON_MACE, WEAPON_BLADE};
	while (player->weapon.name != weapons[min_selected++].name);

	int selected = min_selected;
	int input;

	shop_win = newwin(15, WIN_W, 0, 0);
	keypad(shop_win, true);

	clear();
	mvprintw(WIN_H-1, 0, "Press [ESC] to leave shop. Use the arrow keys to navigate and [ENTER] to submit");
	refresh();

	print_shop(player, weapons, selected, "Welcome to the shop!");
	char info[100] = "Welcome to the shop!";

	while ((input = getch()) != ESC)
	{

		if (input == KEY_UP && selected > min_selected)
			--selected;
		else if (input == KEY_DOWN && selected <= 6)
			++selected;
		else if (input == ENTER)
		{
			if (selected >= 0 && selected <= 6)
			{
				if ((player->life - weapons[selected].price) > 0)
				{
					player->life -= weapons[selected].price;
					player->maxlife -= weapons[selected].price;
					player->weapon = weapons[selected];
					sprintf(info, "You bought the %s", weapons[selected].name);
				}
				else
					sprintf(info, "You will die if you buy that item!");
			}
			else if (selected == 7)
			{
				if (player->life > 3)
				{
					player->life -= 3;
					player->maxlife -= 3;
					player->potions += 1;
					sprintf(info, "You bought 1 Life Potion");
				}
				else
					sprintf(info, "You will die if you buy that item!");
			}
		}

		print_shop(player, weapons, selected, info);
	}

	wclear(shop_win);
	wrefresh(shop_win);
	delwin(shop_win);
}

void print_shop(player, weapons, selected, info)
PLAYER player;
WEAPON weapons[];
char *info;
int selected;
{
	int y;

	wclear(shop_win);

	wborder(shop_win, '|', '|', '-', '-', '+', '+', '+', '+');

	for (y = 1; y <= 6; ++y)
	{
		if (y >= min_selected && player.life - weapons[y].price > 0)
			wattron(shop_win, A_BOLD | COLOR_PAIR(selected == y ? WHITE_BLUE : COLOR_CYAN));
		else
			wattron(shop_win, A_DIM | COLOR_PAIR(selected == y ? COLOR_BLUE : COLOR_CYAN));

		mvwprintw(shop_win, y, 1, "[$%d] %s - DMG: %d", weapons[y].price, weapons[y].name, weapons[y].dmg);

		if (y >= min_selected && player.life - weapons[y].price > 0)
			wattroff(shop_win, A_BOLD | COLOR_PAIR(selected == y ? WHITE_BLUE : COLOR_CYAN));
		else
			wattroff(shop_win, A_DIM | COLOR_PAIR(selected == y ? COLOR_BLUE : COLOR_CYAN));
	}

	mvwhline(shop_win, y++, 1, '-', WIN_W-2);

	wattron(shop_win, (player.life <= 3 ? A_DIM : A_BOLD) | COLOR_PAIR(selected == y-1 ? WHITE_BLUE : COLOR_CYAN));
	mvwprintw(shop_win, y++, 1, "[$3] Life Potion - (Refills life to max when in arena)");
	wattroff(shop_win, (player.life <= 3 ? A_DIM : A_BOLD) | COLOR_PAIR(selected == y-1 ? WHITE_BLUE : COLOR_CYAN));

	mvwhline(shop_win, y++, 1, '-', WIN_W-2);
	mvwhline(shop_win, y++, 1, '-', WIN_W-2);
	
	wattron(shop_win, COLOR_PAIR(PLAYER_COLOR));
	mvwprintw(shop_win, y++, 1, "[You] Life: %d/%d | Weapon: %s (DMG: %d) | Potions: %d",
		player.life,
		player.maxlife,
		player.weapon.name,
		player.weapon.dmg,
		player.potions
	);
	wattroff(shop_win, COLOR_PAIR(PLAYER_COLOR));

	mvwhline(shop_win, y++, 1, '-', WIN_W-2);

	mvwprintw(shop_win, y, 1, info);

	wrefresh(shop_win);
}