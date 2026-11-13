#include <stdio.h>
#include <string.h>
#include "config.h"
#include "inventory.h"
#include "player.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

static void addItem(Player *p, int id, const char *name, char type, int rarity, int power)
{
    p->inventory[p->itemCount].id = id;
    strncpy(p->inventory[p->itemCount].name, name, MAX_NAME - 1);
    p->inventory[p->itemCount].name[MAX_NAME - 1] = '\0';
    p->inventory[p->itemCount].type = type;
    p->inventory[p->itemCount].rarity = rarity;
    p->inventory[p->itemCount].power = power;
    p->itemCount++;
}

int main(void)
{
    Player p;
    int found;

    initPlayer(&p, "Inv");
    addItem(&p, 1, "Wooden Sword", 'S', 1, 3);
    addItem(&p, 17, "Excalibur", 'S', 4, 40);
    addItem(&p, 14, "Shadow Katana", 'S', 3, 24);
    addItem(&p, 19, "Cloth Tunic", 'A', 1, 2);
    addItem(&p, 25, "Iron Plate", 'A', 2, 10);

    /* power descending: Excalibur, Shadow Katana, Iron Plate, Wooden Sword, Cloth Tunic */
    sortInventory(&p, 2);
    CHECK(p.inventory[0].id == 17 && p.inventory[1].id == 14 && p.inventory[4].id == 19);

    /* rarity ascending, stable for equal rarity: 1,19 | 25 | 14 | 17 */
    sortInventory(&p, 1);
    CHECK(p.inventory[0].id == 1 && p.inventory[2].id == 25 && p.inventory[4].id == 17);

    /* name A-Z */
    sortInventory(&p, 3);
    CHECK(strcmp(p.inventory[0].name, "Cloth Tunic") == 0);
    CHECK(strcmp(p.inventory[1].name, "Excalibur") == 0);
    CHECK(strcmp(p.inventory[4].name, "Wooden Sword") == 0);

    /* Name A-Z order: Cloth Tunic, Excalibur, Iron Plate, Shadow Katana, Wooden Sword. */
    equipItem(&p, 1);
    CHECK(p.weaponIndex == 1);
    equipItem(&p, 2);
    CHECK(p.armorIndex == 2);

    sortInventory(&p, 1);
    CHECK(strcmp(p.inventory[p.weaponIndex].name, "Excalibur") == 0);
    CHECK(strcmp(p.inventory[p.armorIndex].name, "Iron Plate") == 0);

    found = searchItemByName(&p, "SWORD");
    CHECK(found == 1);
    found = searchItemByName(&p, "wood");
    CHECK(found == 1);
    found = searchItemByName(&p, "IRON");
    CHECK(found == 1);
    found = searchItemByName(&p, "xyz");
    CHECK(found == 0);
    return 0;
}
