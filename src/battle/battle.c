#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "battle.h"
#include "util.h"
#include "player.h"

/* Damage formula: max(1, ATK - DEF) + rand() % 4. */
int calculateDamage(int atk, int def)
{
    int base = atk - def;
    if (base < 1) base = 1;
    return base + rand() % 4;
}

/* Applies damage with a 10% critical chance (x2); returns the damage dealt. */
int attack(const char *from, const char *to, int atk, int targetDef, int *targetHp, int *critical)
{
    int dmg = calculateDamage(atk, targetDef);
    *critical = 0;
    if (rand() % 100 < 10) {
        dmg *= 2;
        *critical = 1;
    }
    *targetHp -= dmg;
    printf("%s menyerang %s: %d damage%s\n", from, to, dmg, *critical ? " (KRITIS!)" : "");
    return dmg;
}

/* Picks a random monster whose minLevel is at most the player level; returns its index. */
int pickMonster(const Monster m[], int count, int playerLevel)
{
    int candidates[MAX_MONSTER];
    int candidateCount = 0, i;

    for (i = 0; i < count; i++)
        if (m[i].minLevel <= playerLevel) {
            candidates[candidateCount] = i;
            candidateCount++;
        }
    if (candidateCount == 0) return -1;
    return candidates[rand() % candidateCount];
}

/* Runs the turn-based battle until the player wins, loses, or flees. */
void startBattle(Player *p, const Monster m[], int count)
{
    int monsterIndex = pickMonster(m, count, p->level);
    Monster enemy;
    int enemyHp;
    int critical;
    int alive = 1;

    if (monsterIndex < 0) {
        printf("Tidak ada monster yang tersedia.\n");
        return;
    }
    enemy = m[monsterIndex];
    enemyHp = enemy.hp;
    printf("Monster muncul: %s (HP %d, ATK %d, DEF %d)\n", enemy.name, enemy.hp, enemy.atk, enemy.def);

    while (alive) {
        int action;
        printf("--- Battle vs %s ---\n", enemy.name);
        printf("HP kamu: %d/%d (ATK %d, DEF %d, potion %d) | HP %s: %d\n",
               p->hp, p->maxHp, totalAtk(p), totalDef(p), p->potion, enemy.name, enemyHp);
        printf("1. Serang\n2. Bertahan\n3. Potion\n4. Kabur\n");
        action = readInt("Aksi: ", 1, 4);
        if (feof(stdin)) {
            printf("Battle dihentikan (input berakhir).\n");
            alive = 0;
            break;
        }

        switch (action) {
        case 1: {
            attack(p->name, enemy.name, totalAtk(p), enemy.def, &enemyHp, &critical);
            if (enemyHp <= 0) {
                printf("%s kalah! Kamu menang!\n", enemy.name);
                printf("Hadiah: +%d EXP, +%d gold, +%d skor.\n", enemy.exp, enemy.gold, enemy.exp);
                addExp(p, enemy.exp);
                p->gold += enemy.gold;
                p->score += enemy.exp;
                alive = 0;
                break;
            }
            attack(enemy.name, p->name, enemy.atk, totalDef(p), &p->hp, &critical);
            break;
        }
        case 2: {
            int dmg;
            printf("Kamu bertahan.\n");
            dmg = calculateDamage(enemy.atk, totalDef(p));
            if (rand() % 100 < 10) {
                dmg *= 2;
                printf("%s menyerang: %d damage (KRITIS!)\n", enemy.name, dmg);
            } else {
                printf("%s menyerang: %d damage\n", enemy.name, dmg);
            }
            dmg /= 2;
            if (dmg < 1) dmg = 1;
            p->hp -= dmg;
            printf("Karena bertahan, damage masuk hanya %d.\n", dmg);
            break;
        }
        case 3:
            if (p->potion <= 0) {
                printf("Potion habis! (giliran tidak terpakai)\n");
                break;
            }
            p->potion--;
            p->hp += 50;
            if (p->hp > p->maxHp) p->hp = p->maxHp;
            printf("Potion dipakai, HP sekarang %d/%d.\n", p->hp, p->maxHp);
            attack(enemy.name, p->name, enemy.atk, totalDef(p), &p->hp, &critical);
            break;
        case 4:
            if (rand() % 2 == 0) {
                printf("Kabur berhasil! Battle selesai tanpa hadiah.\n");
                alive = 0;
            } else {
                printf("Kabur gagal!\n");
                attack(enemy.name, p->name, enemy.atk, totalDef(p), &p->hp, &critical);
            }
            break;
        }

        if (p->hp <= 0) {
            printf("Kamu dikalahkan oleh %s...\n", enemy.name);
            p->gold /= 2;
            p->hp = p->maxHp;
            printf("Gold tinggal %d, HP dipulihkan penuh. Kembali ke hub.\n", p->gold);
            alive = 0;
        }
    }
}
