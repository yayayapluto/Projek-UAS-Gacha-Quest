#include <stdio.h>
#include <string.h>
#include "config.h"
#include "shop.h"
#include "util.h"
#include "inventory.h"
#include "gacha.h"

/* Menu shop: beli potion atau jual item */
void menuShop(Player *p)
{
    int pilih;

    for (;;) {
        garis();
        printf("=== Shop ===\n");
        printf("Gold: %d | Potion: %d/%d\n", p->gold, p->potion, MAKS_POTION);
        printf("1. Beli potion (%d gold)\n", BIAYA_POTION);
        printf("2. Jual item\n");
        printf("0. Kembali\n");
        pilih = bacaAngka("Pilih: ", 0, 2);
        if (pilih == 0) return;
        switch (pilih) {
        case 1: {
            int maksBeli, jumlah;
            if (p->potion >= MAKS_POTION) {
                printf("Stok potion sudah penuh (maks %d).\n", MAKS_POTION);
                break;
            }
            maksBeli = (MAKS_POTION - p->potion) < (p->gold / BIAYA_POTION)
                       ? (MAKS_POTION - p->potion) : (p->gold / BIAYA_POTION);
            if (maksBeli == 0) {
                printf("Gold tidak cukup untuk membeli potion.\n");
                break;
            }
            jumlah = bacaAngka("Jumlah potion yang dibeli: ", 1, maksBeli);
            p->gold -= jumlah * BIAYA_POTION;
            p->potion += jumlah;
            printf("Beli %d potion. Gold: %d, Potion: %d.\n", jumlah, p->gold, p->potion);
            break;
        }
        case 2: {
            int idSenjataLama = -1, idArmorLama = -1;
            int idx, i;
            if (p->jumlahItem == 0) {
                printf("Inventory kosong, tidak ada yang bisa dijual.\n");
                break;
            }
            printf("%-4s %-20s %-10s %-5s %s\n", "Idx", "Nama", "Rarity", "Power", "Harga jual");
            for (i = 0; i < p->jumlahItem; i++) {
                printf("%-4d %-20s %-10s %-5d %d\n",
                       i, p->inventory[i].nama, namaRarity(p->inventory[i].rarity),
                       p->inventory[i].power, hargaDup(p->inventory[i].rarity));
            }
            idx = bacaAngka("Nomor indeks item yang dijual (999 batal): ", 0, 999);
            if (idx == 999) break;
            if (idx >= p->jumlahItem) {
                printf("Indeks tidak ada di inventory.\n");
                break;
            }
            if (idx == p->idxSenjata || idx == p->idxArmor) {
                printf("Item sedang terpasang, tidak bisa dijual.\n");
                break;
            }
            {
                int harga = hargaDup(p->inventory[idx].rarity);
                if (p->idxSenjata >= 0) idSenjataLama = p->inventory[p->idxSenjata].id;
                if (p->idxArmor >= 0) idArmorLama = p->inventory[p->idxArmor].id;
                p->gold += harga;
                printf("Terjual: %s +%d gold.\n", p->inventory[idx].nama, harga);
                for (i = idx; i < p->jumlahItem - 1; i++)
                    p->inventory[i] = p->inventory[i + 1];
                p->jumlahItem--;
                sinkronkanEquip(p, idSenjataLama, idArmorLama);
            }
            break;
        }
        }
    }
}
