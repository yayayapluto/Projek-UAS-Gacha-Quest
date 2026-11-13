#include <stdio.h>
#include <string.h>
#include "config.h"
#include "storage.h"
#include "player.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    Item catalog[MAX_ITEM];
    Monster monsters[MAX_MONSTER];
    Player p, q;
    int count, i;

    count = loadCatalog("data/item.txt", catalog);
    CHECK(count == 36);
    CHECK(strcmp(catalog[16].name, "Excalibur") == 0);
    CHECK(catalog[16].rarity == 4 && catalog[16].power == 40 && catalog[16].type == 'S');
    CHECK(strcmp(catalog[34].name, "Titan Aegis") == 0 && catalog[34].type == 'A');

    count = loadMonsters("data/monster.txt", monsters);
    CHECK(count == 12);
    CHECK(strcmp(monsters[0].name, "Slime") == 0 && monsters[0].hp == 30);
    CHECK(strcmp(monsters[11].name, "Ancient Wyrm") == 0 && monsters[11].minLevel == 10);

    initPlayer(&p, "Budi");
    p.gold = 555;
    p.score = 77;
    p.potion = 9;
    p.pity = 6;
    p.totalPull = 33;
    p.seed = 123456789u;
    p.level = 3;
    p.exp = 40;
    p.maxHp = 130;
    p.hp = 88;
    p.atk = 14;
    p.def = 5;
    p.itemCount = 3;
    for (i = 0; i < 3; i++) {
        p.inventory[i].id = catalog[i].id;
        strncpy(p.inventory[i].name, catalog[i].name, MAX_NAME - 1);
        p.inventory[i].type = catalog[i].type;
        p.inventory[i].rarity = catalog[i].rarity;
        p.inventory[i].power = catalog[i].power;
    }
    p.weaponIndex = 2;
    p.armorIndex = 0;

    CHECK(saveGame(&p) == 1);
    initPlayer(&q, "Other");
    q.gold = 1;
    CHECK(loadGame(&q, catalog, 36) == 1);
    CHECK(strcmp(q.name, "Budi") == 0);
    CHECK(q.gold == 555 && q.score == 77 && q.potion == 9 && q.pity == 6 && q.totalPull == 33);
    CHECK(q.level == 3 && q.exp == 40 && q.maxHp == 130 && q.hp == 88);
    CHECK(q.atk == 14 && q.def == 5 && q.itemCount == 3);
    CHECK(q.weaponIndex == 2 && q.armorIndex == 0);
    CHECK(q.seed == 123456789u);
    CHECK(q.inventory[0].id == catalog[0].id && q.inventory[2].id == catalog[2].id);
    return 0;
}
