#include <stdio.h>
#include "config.h"
#include "gacha.h"
#include "player.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    Item katalog[MAX_ITEM];
    Player p;
    int pity = 0, i, r, prevPity;
    int jumlah[5] = { 0, 0, 0, 0, 0 };

    for (i = 0; i < MAX_ITEM; i++) {
        katalog[i].id = i + 1;
        katalog[i].rarity = (i % 4) + 1;
        katalog[i].tipe = 'S';
        katalog[i].power = 1;
    }

    pity = 0;
    for (i = 0; i < 20000; i++) {
        prevPity = pity;
        r = tentukanRarity(&pity);
        CEK(r >= RARITY_COMMON && r <= RARITY_LEGENDARY);
        CEK(pity >= 0 && pity < MAKS_PITY);
        if (prevPity >= MAKS_PITY - 1) CEK(r >= RARITY_EPIC);
        jumlah[r]++;
    }
    CEK(jumlah[RARITY_COMMON] > 10500 && jumlah[RARITY_COMMON] < 12000);
    CEK(jumlah[RARITY_RARE] > 4300 && jumlah[RARITY_RARE] < 5200);
    CEK(jumlah[RARITY_EPIC] > 2900 && jumlah[RARITY_EPIC] < 3400);
    CEK(jumlah[RARITY_LEGENDARY] > 500 && jumlah[RARITY_LEGENDARY] < 700);

    initPlayer(&p, "Dup");
    p.jumlahItem = 0;
    p.gold = 0;
    for (i = 0; i < 100; i++)
        tarikSekali(&p, katalog, MAX_ITEM, RARITY_COMMON);
    CEK(p.jumlahItem == 4);
    CEK(p.gold == 96 * HARGA_DUP_COMMON);

    p.jumlahItem = 0;
    p.inventory[0].id = 5;
    p.jumlahItem = 1;
    CEK(cariItemDiInventory(&p, 5) == 0);
    CEK(cariItemDiInventory(&p, 6) == -1);
    return 0;
}
