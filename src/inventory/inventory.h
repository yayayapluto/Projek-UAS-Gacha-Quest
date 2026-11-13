#ifndef INVENTORY_H
#define INVENTORY_H

#include "config.h"

void inventoryMenu(Player *p);
void sortInventory(Player *p, int mode);
int searchItemByName(const Player *p, const char *word);
void equipItem(Player *p, int index);
void syncEquipped(Player *p, int oldWeaponId, int oldArmorId);

#endif
