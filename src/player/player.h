#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"

void initPlayer(Player *p, const char *name);
void addExp(Player *p, int exp);
int totalAtk(const Player *p);
int totalDef(const Player *p);
void showStatus(const Player *p);

#endif
