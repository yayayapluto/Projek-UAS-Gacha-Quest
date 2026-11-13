#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"

int muatKatalog(const char *file, Item k[]);
int muatMonster(const char *file, Monster m[]);
int simpanGame(const Player *p);
int muatGame(Player *p, const Item k[], int jml);
void catatSkor(const Player *p);
void tampilLeaderboard(void);

#endif
