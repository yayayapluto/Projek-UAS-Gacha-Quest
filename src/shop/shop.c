#include <stdio.h>
#include <string.h>
#include "config.h"
#include "shop.h"
#include "util.h"
#include "inventory.h"
#include "gacha.h"

/* Shop menu: buy potions or sell items. */
void shopMenu(Player *p)
{
    int choice;

    for (;;) {
        printLine();
        printf("=== Shop ===\n");
        printf("Gold: %d | Potion: %d/%d\n", p->gold, p->potion, MAX_POTION);
        printf("1. Beli potion (%d gold)\n", POTION_COST);
        printf("2. Jual item\n");
        printf("0. Kembali\n");
        choice = readInt("Pilih: ", 0, 2);
        if (choice == 0) return;
        switch (choice) {
        case 1: {
            int maxBuy, amount;
            if (p->potion >= MAX_POTION) {
                printf("Stok potion sudah penuh (maks %d).\n", MAX_POTION);
                break;
            }
            maxBuy = (MAX_POTION - p->potion) < (p->gold / POTION_COST)
                     ? (MAX_POTION - p->potion) : (p->gold / POTION_COST);
            if (maxBuy == 0) {
                printf("Gold tidak cukup untuk membeli potion.\n");
                break;
            }
            amount = readInt("Jumlah potion yang dibeli: ", 1, maxBuy);
            p->gold -= amount * POTION_COST;
            p->potion += amount;
            printf("Beli %d potion. Gold: %d, Potion: %d.\n", amount, p->gold, p->potion);
            break;
        }
        case 2: {
            int oldWeaponId = -1, oldArmorId = -1;
            int index, i;
            if (p->itemCount == 0) {
                printf("Inventory kosong, tidak ada yang bisa dijual.\n");
                break;
            }
            printf("%-4s %-20s %-10s %-5s %s\n", "Idx", "Nama", "Rarity", "Power", "Harga jual");
            for (i = 0; i < p->itemCount; i++) {
                printf("%-4d %-20s %-10s %-5d %d\n",
                       i, p->inventory[i].name, rarityName(p->inventory[i].rarity),
                       p->inventory[i].power, duplicatePrice(p->inventory[i].rarity));
            }
            index = readInt("Nomor indeks item yang dijual (999 batal): ", 0, 999);
            if (index == 999) break;
            if (index >= p->itemCount) {
                printf("Indeks tidak ada di inventory.\n");
                break;
            }
            if (index == p->weaponIndex || index == p->armorIndex) {
                printf("Item sedang terpasang, tidak bisa dijual.\n");
                break;
            }
            {
                int price = duplicatePrice(p->inventory[index].rarity);
                if (p->weaponIndex >= 0) oldWeaponId = p->inventory[p->weaponIndex].id;
                if (p->armorIndex >= 0) oldArmorId = p->inventory[p->armorIndex].id;
                p->gold += price;
                printf("Terjual: %s +%d gold.\n", p->inventory[index].name, price);
                for (i = index; i < p->itemCount - 1; i++)
                    p->inventory[i] = p->inventory[i + 1];
                p->itemCount--;
                syncEquipped(p, oldWeaponId, oldArmorId);
            }
            break;
        }
        }
    }
}
