#include <stdio.h>
#include "config.h"
#include "gacha.h"
#include "player.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    Item catalog[MAX_ITEM];
    Player p;
    int pity = 0, i, r, previousPity;
    int counts[5] = { 0, 0, 0, 0, 0 };

    /* Every fixture item is Common, so the duplicate run below is deterministic. */
    for (i = 0; i < MAX_ITEM; i++) {
        catalog[i].id = i + 1;
        catalog[i].rarity = RARITY_COMMON;
        catalog[i].type = 'S';
        catalog[i].power = 1;
    }
    pity = 0;
    for (i = 0; i < 20000; i++) {
        previousPity = pity;
        r = rollRarity(&pity);
        CHECK(r >= RARITY_COMMON && r <= RARITY_LEGENDARY);
        CHECK(pity >= 0 && pity < PITY_THRESHOLD);
        if (previousPity >= PITY_THRESHOLD - 1) CHECK(r >= RARITY_EPIC);
        counts[r]++;
    }
    CHECK(counts[RARITY_COMMON] > 10500 && counts[RARITY_COMMON] < 12000);
    CHECK(counts[RARITY_RARE] > 4300 && counts[RARITY_RARE] < 5200);
    CHECK(counts[RARITY_EPIC] > 2900 && counts[RARITY_EPIC] < 3400);
    CHECK(counts[RARITY_LEGENDARY] > 500 && counts[RARITY_LEGENDARY] < 700);

    initPlayer(&p, "Dup");
    p.itemCount = 0;
    p.gold = 0;
    /* 1000 uniform pulls over 36 distinct Common items fill the inventory
       (missing one id needs ~1e-13 luck) and pay duplicates afterwards. */
    for (i = 0; i < 1000; i++)
        pullOnce(&p, catalog, MAX_ITEM, RARITY_COMMON);
    CHECK(p.itemCount == MAX_ITEM);
    CHECK(p.gold == (1000 - MAX_ITEM) * DUPLICATE_PRICE_COMMON);

    p.itemCount = 0;
    p.inventory[0].id = 5;
    p.itemCount = 1;
    CHECK(findItemInInventory(&p, 5) == 0);
    CHECK(findItemInInventory(&p, 6) == -1);
    return 0;
}
