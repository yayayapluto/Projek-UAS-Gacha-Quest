#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"

void initPlayer(Player *p, const char *nama);
void tambahExp(Player *p, int exp);
int hitungAtk(const Player *p);
int hitungDef(const Player *p);
void tampilStatus(const Player *p);

#endif
