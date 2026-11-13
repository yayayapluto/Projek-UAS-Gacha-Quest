#include <stdio.h>
#include <string.h>
#include "config.h"
#include "inventory.h"
#include "player.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

static void tambah(Player *p, int id, const char *nama, char tipe, int rarity, int power)
{
    p->inventory[p->jumlahItem].id = id;
    strncpy(p->inventory[p->jumlahItem].nama, nama, MAX_NAMA - 1);
    p->inventory[p->jumlahItem].nama[MAX_NAMA - 1] = '\0';
    p->inventory[p->jumlahItem].tipe = tipe;
    p->inventory[p->jumlahItem].rarity = rarity;
    p->inventory[p->jumlahItem].power = power;
    p->jumlahItem++;
}

int main(void)
{
    Player p;
    int ketemu;

    initPlayer(&p, "Inv");
    tambah(&p, 1, "Pedang Kayu", 'S', 1, 3);
    tambah(&p, 7, "Excalibur", 'S', 4, 30);
    tambah(&p, 5, "Tombak Naga", 'S', 3, 17);
    tambah(&p, 9, "Baju Kain", 'A', 1, 2);
    tambah(&p, 11, "zirah besi", 'A', 2, 6);

    urutkanInventory(&p, 2);
    CEK(p.inventory[0].id == 7 && p.inventory[1].id == 5 && p.inventory[4].id == 9);

    urutkanInventory(&p, 1);
    CEK(p.inventory[0].id == 1 && p.inventory[2].id == 11 && p.inventory[4].id == 7);

    urutkanInventory(&p, 3);
    CEK(strcmp(p.inventory[0].nama, "Baju Kain") == 0);
    CEK(strcmp(p.inventory[1].nama, "Excalibur") == 0);
    CEK(strcmp(p.inventory[4].nama, "zirah besi") == 0);

    equipItem(&p, 1);
    CEK(p.idxSenjata == 1);
    equipItem(&p, 4);
    CEK(p.idxArmor == 4);
    equipItem(&p, 2);
    CEK(p.idxSenjata == 2);

    urutkanInventory(&p, 1);
    CEK(strcmp(p.inventory[p.idxSenjata].nama, "Pedang Kayu") == 0);
    CEK(strcmp(p.inventory[p.idxArmor].nama, "zirah besi") == 0);

    ketemu = cariItemNama(&p, "PEDANG");
    CEK(ketemu == 1);
    ketemu = cariItemNama(&p, "kayu");
    CEK(ketemu == 1);
    ketemu = cariItemNama(&p, "ZIRAH");
    CEK(ketemu == 1);
    ketemu = cariItemNama(&p, "xyz");
    CEK(ketemu == 0);
    return 0;
}
