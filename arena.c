#include <ncurses.h>

#define ENTER 10
#define ESC 27

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define PLAYER_DEAD 0
#define ENEMY_DEAD 1
#define NONE_DEAD 2

#define vect_up(v) (vector){v.x, v.y-1}
#define vect_down(v) (vector){v.x, v.y+1}
#define vect_left(v) (vector){v.x-1, v.y}
#define vect_right(v) (vector){v.x+1, v.y}
#define vect_add(v1, v2) (vector){v1.x+v2.x, v1.y+v2.y}
#define vect_sub(v1, v2) (vector){v1.x-v2.x, v1.y-v2.y}

#define ARENA_W 80
#define ARENA_H 24

void init_enemies();
int all_enemies_dead();
char info[100];

void start_arena()
{
	WINDOW *arena_win = newwin(ARENA_H, ARENA_W, 0, 0);
	keypad(arena_win, true);

	ENEMY all_enemies[] = {ENEMY_PEASANT, ENEMY_THIEF, ENEMY_BARBARIAN, ENEMY_BANDIT, ENEMY_DESERTER, ENEMY_GLADIATOR, ENEMY_CHAMP};

	PLAYER player = {
		15,
		15,
		WEAPON_DAGGER,
		0,
		1,
		vect_new(1, ARENA_H/2)
	};


	int input = 0;

	while (input != ESC)
	{
		int enemy_level = ((player.level/3 <= 7) ? player.level/3 : 7);

		if (player.level % 3 == 0)
			--enemy_level;

		if (player.level/3 > 6)
			player.level = enemy_level*3;

		ENEMY enemies[] = {all_enemies[enemy_level], all_enemies[enemy_level], all_enemies[enemy_level]};

		if (player.level % 3 == 1)
		{
			enemies[1].life = 0;
			enemies[2].life = 0;
		}
		else if (player.level % 3 == 2)
			enemies[2].life = 0;

		init_enemies(enemies);

		show_arena(player, enemies, arena_win);

		while ((input = wgetch(arena_win)) != ESC)
		{
			int can_go[4] = {true, true, true, true};
			int k;

			if (input == 'p' && player.potions >= 1 && player.life != player.maxlife)
			{
				player.potions -= 1;
				player.life = player.maxlife;
			}

			if (input == 'G' && getch() == 'M') //Godmode
			{
				player.maxlife = 1000;
				player.life = 1000;
				player.weapon = WEAPON_BLADE;
				player.potions = 1000;
			}

			for (k = 0; k <= 2; ++k)
			{
				int killed_enemy = false;
				vector newpos = vect_add(vect_norm(vect_sub(player.pos, enemies[k].pos)), enemies[k].pos);

				if (enemies[k].life <= 0)
					continue;

				if (newpos.x >= 1 && newpos.x < ARENA_W-1 && newpos.y >= 1 && newpos.y < ARENA_H-1 && !vect_equals(newpos, player.pos))
					enemies[k].pos = newpos;

				if (vect_equals(enemies[k].pos, vect_up(player.pos)))
				{
					killed_enemy = attack(&player, &enemies[k]);
					can_go[UP] = false;
				}
				else if (vect_equals(enemies[k].pos, vect_down(player.pos)))
				{
					killed_enemy = attack(&player, &enemies[k]);
					can_go[DOWN] = false;
				}
				else if (vect_equals(enemies[k].pos, vect_left(player.pos)))
				{
					killed_enemy = attack(&player, &enemies[k]);
					can_go[LEFT] = false;
				}
				else if (vect_equals(enemies[k].pos, vect_right(player.pos)))
				{
					killed_enemy = attack(&player, &enemies[k]);
					can_go[RIGHT] = false;
				}

				if (killed_enemy && all_enemies_dead(enemies))
					goto win;
				else if (killed_enemy)
					player.maxlife += enemies[0].dmg * 2;
			}
			
			if (player.life <= 0)
				goto end;	

			if (can_go[UP] && (input == KEY_UP || input == 'w') && player.pos.y > 1)
				--player.pos.y;
			
			if (can_go[DOWN] && (input == KEY_DOWN || input == 's') && player.pos.y < ARENA_H-2)
				++player.pos.y;

			if (can_go[LEFT] && (input == KEY_LEFT || input == 'a') && player.pos.x > 1)
				--player.pos.x;

			if (can_go[RIGHT] && (input == KEY_RIGHT || input == 'd') && player.pos.x < ARENA_W-2)
				++player.pos.x;

			show_arena(player, enemies, arena_win);
		}

		win:;
		if (input != ESC)
		{
			player.maxlife += (enemies[0].dmg * 2)+player.level;
			player.life = player.maxlife;
			player.level += mrand(0, 1);
			show_shop(&player);
			info[0] = '\0';
		}
	}

	end:;

	wclear(arena_win);
	wrefresh(arena_win);
	delwin(arena_win);
	
	if (player.life <= 0)
		show_death_screen();
}

int all_enemies_dead(enemies)
ENEMY enemies[];
{
	int k;

	for (k = 0; k <= 2; ++k)
		if (enemies[k].life > 0)
			return(false);

	return(true);
}

void init_enemies(enemies)
ENEMY enemies[];
{
	int k;

	for (k = 0; enemies[k].life > 0 && k <= 2; ++k)
		enemies[k].pos = vect_new(mrand(10, ARENA_W-3), mrand(2+((ARENA_H/3)*k), ((ARENA_H/3)*(k+1))-2));
}

void show_death_screen()
{
	int c;

	clear();

	attron(A_BOLD | COLOR_PAIR(COLOR_RED));
	mvprintw((WIN_H-1)/2, (WIN_W/2)-16, "You died. Better luck next time!");
	attroff(A_BOLD | COLOR_PAIR(COLOR_RED));

	mvprintw(WIN_H-1, 0, "Press [ENTER] to continue");

	while ((c = getch()) != ENTER);

	refresh();
}

int attack(player, enemy)
PLAYER *player;
ENEMY *enemy;
{
	int pdmg = mrand(player->weapon.dmg-2, player->weapon.dmg+2);
	int edmg = mrand(enemy->dmg-2, enemy->dmg+2);

	enemy->life -= pdmg;
	sprintf(info, "You hit the %s with your %s, dealing %d damage!\n", enemy->name, player->weapon.name, pdmg);

	if (enemy->life > 0)
	{
		player->life -= edmg;

		if (player->life > 0)
			sprintf(info, "%sThe %s hits you with its %s, dealing %d damage!",
				info,
				enemy->name,
				enemy->weapon,
				edmg
			);
	}
	else
	{
		sprintf(info, "You kill the %s", enemy->name);
		return(true);
	}

	return(false);
}

void show_arena(player, enemies, arena_win)
PLAYER player;
ENEMY enemies[];
WINDOW *arena_win;
{
	int k;
	int offset = find(info, "\n") != -1;

	if (info[0] == '\0')
		offset = -2;

	clear();
	wclear(arena_win);

	wborder(arena_win, '|', '|', '-', '-', '+', '+', '+', '+');

	wattron(arena_win, A_BOLD | COLOR_PAIR(PLAYER_COLOR));
	mvwaddch(arena_win, player.pos.y, player.pos.x, '@');
	wattroff(arena_win, A_BOLD | COLOR_PAIR(PLAYER_COLOR));

	for (k = 0; k <= 2; ++k)
	{
		if (enemies[k].life > 0)
		{
			wattron(arena_win, COLOR_PAIR(enemies[k].color));
			mvwaddch(arena_win, enemies[k].pos.y, enemies[k].pos.x, enemies[k].sym);
			wattroff(arena_win, COLOR_PAIR(enemies[k].color));

			attron(COLOR_PAIR(enemies[k].color));
			mvprintw(ARENA_H+4+(k * 2)+offset, 0, "[%s] Life: %d | Weapon: %s (DMG: %d)",
				enemies[k].name,
				enemies[k].life,
				enemies[k].weapon,
				enemies[k].dmg
			);
			attroff(COLOR_PAIR(enemies[k].color));
		}
	}

	attron(COLOR_PAIR(PLAYER_COLOR));
	mvprintw(ARENA_H+2+offset, 0, "[You] Life: %d/%d | Weapon: %s (DMG: %d) | Potions: %d",
		player.life,
		player.maxlife,
		player.weapon.name,
		player.weapon.dmg,
		player.potions
	);
	attroff(COLOR_PAIR(PLAYER_COLOR));

	mvprintw(WIN_H-1, 0, "Use arrow keys/WASD to move, P to drink a potion, and ESC to return main menu");
	
	if (info[0] != '\0')
		mvprintw(ARENA_H, 0, info);

	info[0] = '\0';

	refresh();
	wrefresh(arena_win);
}