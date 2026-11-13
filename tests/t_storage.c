#include <stdio.h>
#include <string.h>
#include "config.h"
#include "storage.h"
#include "player.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    Item katalog[MAX_ITEM];
    Monster monster[MAX_MONSTER];
    Player p, q;
    int jml, i;

    jml = muatKatalog("data/item.txt", katalog);
    CEK(jml == 16);
    CEK(strcmp(katalog[6].nama, "Excalibur") == 0);
    CEK(katalog[6].rarity == 4 && katalog[6].power == 30 && katalog[6].tipe == 'S');
    CEK(strcmp(katalog[14].nama, "Armor Titan") == 0 && katalog[14].tipe == 'A');

    jml = muatMonster("data/monster.txt", monster);
    CEK(jml == 7);
    CEK(strcmp(monster[0].nama, "Slime") == 0 && monster[0].hp == 30);
    CEK(strcmp(monster[6].nama, "Naga (Boss)") == 0 && monster[6].levelMin == 7);

    initPlayer(&p, "Budi");
    p.gold = 555;
    p.skor = 77;
    p.potion = 9;
    p.pity = 6;
    p.totalPull = 33;
    p.level = 3;
    p.exp = 40;
    p.maxHp = 130;
    p.hp = 88;
    p.atk = 14;
    p.def = 5;
    p.jumlahItem = 3;
    for (i = 0; i < 3; i++) {
        p.inventory[i].id = katalog[i].id;
        strncpy(p.inventory[i].nama, katalog[i].nama, MAX_NAMA - 1);
        p.inventory[i].tipe = katalog[i].tipe;
        p.inventory[i].rarity = katalog[i].rarity;
        p.inventory[i].power = katalog[i].power;
    }
    p.idxSenjata = 2;
    p.idxArmor = 0;

    CEK(simpanGame(&p) == 1);
    initPlayer(&q, "Lain");
    q.gold = 1;
    CEK(muatGame(&q, katalog, 16) == 1);
    CEK(strcmp(q.nama, "Budi") == 0);
    CEK(q.gold == 555 && q.skor == 77 && q.potion == 9 && q.pity == 6 && q.totalPull == 33);
    CEK(q.level == 3 && q.exp == 40 && q.maxHp == 130 && q.hp == 88);
    CEK(q.atk == 14 && q.def == 5 && q.jumlahItem == 3);
    CEK(q.idxSenjata == 2 && q.idxArmor == 0);
    CEK(q.inventory[0].id == katalog[0].id && q.inventory[2].id == katalog[2].id);
    return 0;
}
