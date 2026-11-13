#include <stdio.h>
#include <string.h>
#include "config.h"
#include "player.h"
#include "util.h"

/* Fills a new player with the starting values from rule 6.1. */
void initPlayer(Player *p, const char *name)
{
    memset(p, 0, sizeof *p);
    strncpy(p->name, name, MAX_NAME - 1);
    p->name[MAX_NAME - 1] = '\0';
    p->hp = 100;
    p->maxHp = 100;
    p->atk = 10;
    p->def = 3;
    p->level = 1;
    p->exp = 0;
    p->gold = 200;
    p->potion = 2;
    p->pity = 0;
    p->totalPull = 0;
    p->score = 0;
    p->seed = 0;
    p->itemCount = 0;
    p->weaponIndex = -1;
    p->armorIndex = -1;
}

/* Adds EXP and resolves level-ups (possibly several at once). */
void addExp(Player *p, int exp)
{
    int needed;
    p->exp += exp;
    while (p->level < MAX_LEVEL) {
        needed = p->level * 50;
        if (p->exp < needed) break;
        p->exp -= needed;
        p->level++;
        p->maxHp += 15;
        p->atk += 2;
        p->def += 1;
        p->hp = p->maxHp;
        printf("Naik level! Sekarang level %d (maxHp %d, ATK %d, DEF %d).\n",
               p->level, p->maxHp, p->atk, p->def);
    }
    if (p->level >= MAX_LEVEL) p->exp = 0;
}

/* Total ATK = base ATK + equipped weapon power. */
int totalAtk(const Player *p)
{
    if (p->weaponIndex >= 0) return p->atk + p->inventory[p->weaponIndex].power;
    return p->atk;
}

/* Total DEF = base DEF + equipped armor power. */
int totalDef(const Player *p)
{
    if (p->armorIndex >= 0) return p->def + p->inventory[p->armorIndex].power;
    return p->def;
}

/* Shows the full player status. */
void showStatus(const Player *p)
{
    printf("=== Status ===\n");
    printf("Nama       : %s\n", p->name);
    printf("Level      : %d (EXP %d/%d)\n",
           p->level, p->exp, p->level >= MAX_LEVEL ? 0 : p->level * 50);
    printf("HP         : %d/%d\n", p->hp, p->maxHp);
    printf("ATK        : %d", totalAtk(p));
    if (p->weaponIndex >= 0)
        printf(" (%d + %d %s)", p->atk, p->inventory[p->weaponIndex].power, p->inventory[p->weaponIndex].name);
    printf("\n");
    printf("DEF        : %d", totalDef(p));
    if (p->armorIndex >= 0)
        printf(" (%d + %d %s)", p->def, p->inventory[p->armorIndex].power, p->inventory[p->armorIndex].name);
    printf("\n");
    printf("Gold       : %d\n", p->gold);
    printf("Potion     : %d\n", p->potion);
    printf("Pity       : %d/%d\n", p->pity, PITY_THRESHOLD);
    printf("Total pull : %d\n", p->totalPull);
    printf("Skor       : %d\n", p->score);
    printf("Seed dunia : %u\n", p->seed);
    printf("Item       : %d/%d", p->itemCount, MAX_ITEM);
    if (p->weaponIndex >= 0) printf(" | Senjata: %s", p->inventory[p->weaponIndex].name);
    if (p->armorIndex >= 0) printf(" | Armor: %s", p->inventory[p->armorIndex].name);
    printf("\n");
}
