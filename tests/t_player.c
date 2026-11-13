#include <stdio.h>
#include <string.h>
#include "config.h"
#include "player.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    Player p;

    initPlayer(&p, "Budi");
    CHECK(p.hp == 100 && p.maxHp == 100 && p.atk == 10 && p.def == 3);
    CHECK(p.gold == 200 && p.potion == 2 && p.level == 1 && p.exp == 0);
    CHECK(p.pity == 0 && p.totalPull == 0 && p.score == 0 && p.seed == 0);
    CHECK(p.itemCount == 0 && p.weaponIndex == -1 && p.armorIndex == -1);

    addExp(&p, 100);
    CHECK(p.level == 2 && p.exp == 50 && p.maxHp == 115 && p.atk == 12 && p.def == 4 && p.hp == 115);

    addExp(&p, 300);
    CHECK(p.level == 4 && p.exp == 100 && p.maxHp == 145 && p.atk == 16 && p.def == 6);

    p.level = 9;
    p.exp = 0;
    p.maxHp = 100 + 15 * 8;
    addExp(&p, 10000);
    CHECK(p.level == MAX_LEVEL && p.exp == 0);
    CHECK(p.maxHp == 100 + 15 * 9 && p.atk == 10 + 2 * 9 && p.def == 3 + 9);

    p.weaponIndex = -1;
    p.armorIndex = -1;
    CHECK(totalAtk(&p) == p.atk && totalDef(&p) == p.def);
    p.inventory[0].power = 7;
    p.weaponIndex = 0;
    p.armorIndex = 0;
    p.inventory[1].power = 4;
    CHECK(totalAtk(&p) == p.atk + 7 && totalDef(&p) == p.def + 4);
    return 0;
}
