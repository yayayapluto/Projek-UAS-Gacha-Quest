#include <stdio.h>
#include <string.h>
#include "config.h"
#include "player.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    Player p;

    initPlayer(&p, "Budi");
    CEK(p.hp == 100 && p.maxHp == 100 && p.atk == 10 && p.def == 3);
    CEK(p.gold == 200 && p.potion == 2 && p.level == 1 && p.exp == 0);
    CEK(p.pity == 0 && p.totalPull == 0 && p.skor == 0);
    CEK(p.jumlahItem == 0 && p.idxSenjata == -1 && p.idxArmor == -1);

    tambahExp(&p, 100);
    CEK(p.level == 2 && p.exp == 50 && p.maxHp == 115 && p.atk == 12 && p.def == 4 && p.hp == 115);

    tambahExp(&p, 300);
    CEK(p.level == 4 && p.exp == 100 && p.maxHp == 145 && p.atk == 16 && p.def == 6);

    p.level = 9;
    p.exp = 0;
    p.maxHp = 100 + 15 * 8;
    tambahExp(&p, 10000);
    CEK(p.level == MAKS_LEVEL && p.exp == 0);
    CEK(p.maxHp == 100 + 15 * 9 && p.atk == 10 + 2 * 9 && p.def == 3 + 9);

    p.idxSenjata = -1;
    p.idxArmor = -1;
    CEK(hitungAtk(&p) == p.atk && hitungDef(&p) == p.def);
    p.inventory[0].power = 7;
    p.idxSenjata = 0;
    p.idxArmor = 0;
    p.inventory[1].power = 4;
    CEK(hitungAtk(&p) == p.atk + 7 && hitungDef(&p) == p.def + 4);
    return 0;
}
