#ifndef INVENTORY_H
#define INVENTORY_H

#include "config.h"

void menuInventory(Player *p);
void urutkanInventory(Player *p, int mode);
int cariItemNama(const Player *p, const char *kata);
void equipItem(Player *p, int idx);
void sinkronkanEquip(Player *p, int idSenjataLama, int idArmorLama);

#endif
