#include <stdio.h>
#include <string.h>
#include "config.h"
#include "player.h"
#include "util.h"

/* Mengisi nilai awal pemain baru sesuai aturan 6.1 */
void initPlayer(Player *p, const char *nama)
{
    memset(p, 0, sizeof *p);
    strncpy(p->nama, nama, MAX_NAMA - 1);
    p->nama[MAX_NAMA - 1] = '\0';
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
    p->skor = 0;
    p->jumlahItem = 0;
    p->idxSenjata = -1;
    p->idxArmor = -1;
}

/* Menambah EXP dan memproses naik level (bisa beberapa level sekaligus) */
void tambahExp(Player *p, int exp)
{
    int butuh;
    p->exp += exp;
    while (p->level < MAKS_LEVEL) {
        butuh = p->level * 50;
        if (p->exp < butuh) break;
        p->exp -= butuh;
        p->level++;
        p->maxHp += 15;
        p->atk += 2;
        p->def += 1;
        p->hp = p->maxHp;
        printf("Naik level! Sekarang level %d (maxHp %d, ATK %d, DEF %d).\n",
               p->level, p->maxHp, p->atk, p->def);
    }
    if (p->level >= MAKS_LEVEL) p->exp = 0;
}

/* ATK total = ATK dasar + power senjata terpasang */
int hitungAtk(const Player *p)
{
    if (p->idxSenjata >= 0) return p->atk + p->inventory[p->idxSenjata].power;
    return p->atk;
}

/* DEF total = DEF dasar + power armor terpasang */
int hitungDef(const Player *p)
{
    if (p->idxArmor >= 0) return p->def + p->inventory[p->idxArmor].power;
    return p->def;
}

/* Menampilkan status lengkap pemain */
void tampilStatus(const Player *p)
{
    printf("=== Status ===\n");
    printf("Nama       : %s\n", p->nama);
    printf("Level      : %d (EXP %d/%d)\n",
           p->level, p->exp, p->level >= MAKS_LEVEL ? 0 : p->level * 50);
    printf("HP         : %d/%d\n", p->hp, p->maxHp);
    printf("ATK        : %d", hitungAtk(p));
    if (p->idxSenjata >= 0)
        printf(" (%d + %d %s)", p->atk, p->inventory[p->idxSenjata].power, p->inventory[p->idxSenjata].nama);
    printf("\n");
    printf("DEF        : %d", hitungDef(p));
    if (p->idxArmor >= 0)
        printf(" (%d + %d %s)", p->def, p->inventory[p->idxArmor].power, p->inventory[p->idxArmor].nama);
    printf("\n");
    printf("Gold       : %d\n", p->gold);
    printf("Potion     : %d\n", p->potion);
    printf("Pity       : %d/%d\n", p->pity, MAKS_PITY);
    printf("Total pull : %d\n", p->totalPull);
    printf("Skor       : %d\n", p->skor);
    printf("Item       : %d/%d", p->jumlahItem, MAX_ITEM);
    if (p->idxSenjata >= 0) printf(" | Senjata: %s", p->inventory[p->idxSenjata].nama);
    if (p->idxArmor >= 0) printf(" | Armor: %s", p->inventory[p->idxArmor].nama);
    printf("\n");
}
