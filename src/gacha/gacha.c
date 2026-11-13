#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "gacha.h"
#include "util.h"

const char *rarityName(int rarity)
{
    switch (rarity) {
    case RARITY_COMMON: return "Common";
    case RARITY_RARE: return "Rare";
    case RARITY_EPIC: return "Epic";
    case RARITY_LEGENDARY: return "Legendary";
    }
    return "?";
}

int duplicatePrice(int rarity)
{
    switch (rarity) {
    case RARITY_COMMON: return DUPLICATE_PRICE_COMMON;
    case RARITY_RARE: return DUPLICATE_PRICE_RARE;
    case RARITY_EPIC: return DUPLICATE_PRICE_EPIC;
    case RARITY_LEGENDARY: return DUPLICATE_PRICE_LEGENDARY;
    }
    return 0;
}

/* Sequential search by item ID in the inventory; -1 when absent. */
int findItemInInventory(const Player *p, int id)
{
    int i;
    for (i = 0; i < p->itemCount; i++)
        if (p->inventory[i].id == id) return i;
    return -1;
}

/* Rolls rarity from rand() % 100 + 1 and applies pity; updates *pity. */
int rollRarity(int *pity)
{
    int roll = rand() % 100 + 1;
    int r;

    if (roll >= 98) r = RARITY_LEGENDARY;
    else if (roll >= 86) r = RARITY_EPIC;
    else if (roll >= 61) r = RARITY_RARE;
    else r = RARITY_COMMON;

    if (r >= RARITY_EPIC) {
        *pity = 0;
    } else {
        (*pity)++;
        if (*pity >= PITY_THRESHOLD) {
            r = RARITY_EPIC;
            *pity = 0;
        }
    }
    return r;
}

/* One pull: pick a random item of the given rarity, convert duplicates to gold, print the result. */
void pullOnce(Player *p, const Item catalog[], int count, int rarity)
{
    Item candidates[MAX_ITEM];
    int candidateCount = 0, i, index, picked;

    for (i = 0; i < count; i++)
        if (catalog[i].rarity == rarity) {
            candidates[candidateCount] = catalog[i];
            candidateCount++;
        }
    if (candidateCount == 0) return;
    picked = rand() % candidateCount;
    index = findItemInInventory(p, candidates[picked].id);
    if (index >= 0) {
        int price = duplicatePrice(candidates[picked].rarity);
        p->gold += price;
        printf("  [%s] %s -> DUPLIKAT, jadi +%d gold\n", rarityName(rarity), candidates[picked].name, price);
    } else if (p->itemCount < MAX_ITEM) {
        p->inventory[p->itemCount] = candidates[picked];
        p->itemCount++;
        printf("  [%s] %s (power %d) -> baru!\n", rarityName(rarity), candidates[picked].name, candidates[picked].power);
    }
}

/* Gacha menu: one pull, ten pulls, or back. */
void gachaMenu(Player *p, const Item catalog[], int count)
{
    int choice;

    for (;;) {
        printLine();
        printf("=== Gacha ===\n");
        printf("Gold: %d | Pity: %d/%d | Total pull: %d\n", p->gold, p->pity, PITY_THRESHOLD, p->totalPull);
        printf("1. Tarikan 1x (%d gold)\n", PULL_COST);
        printf("2. Tarikan 10x (%d gold)\n", PULL10_COST);
        printf("0. Kembali\n");
        choice = readInt("Pilih: ", 0, 2);
        if (choice == 0) return;
        if (choice == 1) {
            if (p->gold < PULL_COST) {
                printf("Gold kurang! Tarikan 1x butuh %d gold.\n", PULL_COST);
                continue;
            }
            p->gold -= PULL_COST;
            p->totalPull++;
            pullOnce(p, catalog, count, rollRarity(&p->pity));
        } else {
            int i;
            if (p->gold < PULL10_COST) {
                printf("Gold kurang! Tarikan 10x butuh %d gold.\n", PULL10_COST);
                continue;
            }
            p->gold -= PULL10_COST;
            printf("Hasil tarikan 10x:\n");
            for (i = 0; i < 10; i++) {
                p->totalPull++;
                pullOnce(p, catalog, count, rollRarity(&p->pity));
            }
        }
    }
}
