#ifndef GACHA_H
#define GACHA_H

#include "config.h"

void menuGacha(Player *p, const Item katalog[], int jml);
int tentukanRarity(int *pity);
int cariItemDiInventory(const Player *p, int id);
int hargaDup(int rarity);
const char *namaRarity(int rarity);
void tarikSekali(Player *p, const Item katalog[], int jml, int rarity);

#endif
