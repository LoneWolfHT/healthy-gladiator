#include <ncurses.h>

typedef struct vector vector;

typedef struct enemy {
	char *name;
	char sym;
	int life;
	int dmg;
	char *weapon;
	int color;
	vector pos;
} ENEMY;

ENEMY ENEMY_PEASANT = {
	"Peasant",
	'P',
	10,
	1,
	"Dagger",
	COLOR_WHITE,
	(vector){1, 1}
};

ENEMY ENEMY_THIEF = {
	"Thief",
	'T',
	15,
	1,
	"Dagger",
	COLOR_WHITE,
	(vector){1, 1}
};

ENEMY ENEMY_BARBARIAN = {
	"Barbarian",
	'b',
	20,
	1,
	"Club",
	COLOR_WHITE,
	(vector){1, 1}
};

ENEMY ENEMY_BANDIT = {
	"Bandit",
	'B',
	20,
	2,
	"Spiked Club",
	COLOR_WHITE,
	(vector){1, 1}
};

ENEMY ENEMY_DESERTER = {
	"Deserter",
	'D',
	25,
	3,
	"Rusty Spear",
	COLOR_WHITE,
	(vector){1, 1}
};

ENEMY ENEMY_GLADIATOR = {
	"Gladiator",
	'G',
	35,
	4,
	"Rusty Sword",
	COLOR_WHITE,
	(vector){1, 1}
};

ENEMY ENEMY_CHAMP = {
	"Champion",
	'C',
	50,
	7,
	"Sharpened Sword",
	COLOR_RED,
	(vector){1, 1}
};