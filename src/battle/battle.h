#ifndef BATTLE_H
#define BATTLE_H

#include "config.h"

void startBattle(Player *p, const Monster m[], int count);
int calculateDamage(int atk, int def);

#endif
