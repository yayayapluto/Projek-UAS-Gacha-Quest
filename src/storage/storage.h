#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"

int loadCatalog(const char *file, Item items[]);
int loadMonsters(const char *file, Monster monsters[]);
int saveGame(const Player *p);
int loadGame(Player *p, const Item items[], int count);
void recordScore(const Player *p);
void showLeaderboard(void);

#endif
