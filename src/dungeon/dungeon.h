#ifndef DUNGEON_H
#define DUNGEON_H

#include "config.h"

unsigned int cellHash(unsigned int seed, int x, int y);
char dungeonCell(unsigned int seed, int x, int y);
unsigned int randomWorldSeed(void);
unsigned int promptWorldSeed(void);
int dfsReachable(char area[][AREA_MAX], Point from, Point to);
int bfsShortestPath(char area[][AREA_MAX], Point from, Point to, Point path[]);
void runDungeon(Player *p);

#endif
