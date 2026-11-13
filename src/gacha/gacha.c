#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "gacha.h"
#include "util.h"

const char *namaRarity(int rarity)
{
    switch (rarity) {
    case RARITY_COMMON: return "Common";
    case RARITY_RARE: return "Rare";
    case RARITY_EPIC: return "Epic";
    case RARITY_LEGENDARY: return "Legendary";
    }
    return "?";
}

int hargaDup(int rarity)
{
    switch (rarity) {
    case RARITY_COMMON: return HARGA_DUP_COMMON;
    case RARITY_RARE: return HARGA_DUP_RARE;
    case RARITY_EPIC: return HARGA_DUP_EPIC;
    case RARITY_LEGENDARY: return HARGA_DUP_LEGENDARY;
    }
    return 0;
}

/* Sequential search item berdasarkan ID di inventory; -1 jika tidak ada */
int cariItemDiInventory(const Player *p, int id)
{
    int i;
    for (i = 0; i < p->jumlahItem; i++)
        if (p->inventory[i].id == id) return i;
    return -1;
}

/* Menentukan rarity dari rand()%100+1 dengan pity; mengubah *pity */
int tentukanRarity(int *pity)
{
    int roll = rand() % 100 + 1;
    int r;

    if (roll >= 98) r = RARITY_LEGENDARY;
    else if (roll >= 86) r = RARITY_EPIC;
    else if (roll >= 61) r = RARITY_RARE;
    else r = RARITY_COMMON;

    if (r >= RARITY_EPIC) {
        *pity = 0;
    } else {
        (*pity)++;
        if (*pity >= MAKS_PITY) {
            r = RARITY_EPIC;
            *pity = 0;
        }
    }
    return r;
}

/* Satu tarikan: pilih item acak sesuai rarity, duplikat jadi gold; tampilkan hasil */
void tarikSekali(Player *p, const Item katalog[], int jml, int rarity)
{
    Item kandidat[MAX_ITEM];
    int jmlKandidat = 0, i, idx, dipilih;

    for (i = 0; i < jml; i++)
        if (katalog[i].rarity == rarity) {
            kandidat[jmlKandidat] = katalog[i];
            jmlKandidat++;
        }
    if (jmlKandidat == 0) return;
    dipilih = rand() % jmlKandidat;
    idx = cariItemDiInventory(p, kandidat[dipilih].id);
    if (idx >= 0) {
        int harga = hargaDup(kandidat[dipilih].rarity);
        p->gold += harga;
        printf("  [%s] %s -> DUPLIKAT, jadi +%d gold\n", namaRarity(rarity), kandidat[dipilih].nama, harga);
    } else if (p->jumlahItem < MAX_ITEM) {
        p->inventory[p->jumlahItem] = kandidat[dipilih];
        p->jumlahItem++;
        printf("  [%s] %s (power %d) -> baru!\n", namaRarity(rarity), kandidat[dipilih].nama, kandidat[dipilih].power);
    }
}

/* Menu gacha: tarikan 1x, 10x, atau kembali */
void menuGacha(Player *p, const Item katalog[], int jml)
{
    int pilih;

    for (;;) {
        garis();
        printf("=== Gacha ===\n");
        printf("Gold: %d | Pity: %d/%d | Total pull: %d\n", p->gold, p->pity, MAKS_PITY, p->totalPull);
        printf("1. Tarikan 1x (%d gold)\n", BIAYA_PULL);
        printf("2. Tarikan 10x (%d gold)\n", BIAYA_PULL10);
        printf("0. Kembali\n");
        pilih = bacaAngka("Pilih: ", 0, 2);
        if (pilih == 0) return;
        if (pilih == 1) {
            if (p->gold < BIAYA_PULL) {
                printf("Gold kurang! Tarikan 1x butuh %d gold.\n", BIAYA_PULL);
                continue;
            }
            p->gold -= BIAYA_PULL;
            p->totalPull++;
            tarikSekali(p, katalog, jml, tentukanRarity(&p->pity));
        } else {
            int i;
            if (p->gold < BIAYA_PULL10) {
                printf("Gold kurang! Tarikan 10x butuh %d gold.\n", BIAYA_PULL10);
                continue;
            }
            p->gold -= BIAYA_PULL10;
            printf("Hasil tarikan 10x:\n");
            for (i = 0; i < 10; i++) {
                p->totalPull++;
                tarikSekali(p, katalog, jml, tentukanRarity(&p->pity));
            }
        }
    }
}
