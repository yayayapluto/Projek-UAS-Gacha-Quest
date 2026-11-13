#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "inventory.h"
#include "util.h"

/* Menampilkan tabel inventory dengan status terpasang */
static void tampilInventory(const Player *p)
{
    const char *namaRarity[] = { "-", "Common", "Rare", "Epic", "Legendary" };
    int i;

    printf("Inventory (%d/%d)\n", p->jumlahItem, MAX_ITEM);
    printf("%-4s %-20s %-4s %-10s %-5s %s\n", "Idx", "Nama", "Tipe", "Rarity", "Power", "Terpasang");
    for (i = 0; i < p->jumlahItem; i++) {
        printf("%-4d %-20s %-4c %-10s %-5d %s\n",
               i, p->inventory[i].nama, p->inventory[i].tipe,
               namaRarity[p->inventory[i].rarity], p->inventory[i].power,
               i == p->idxSenjata ? "[SENJATA]" : (i == p->idxArmor ? "[ARMOR]" : ""));
    }
}

/* Menggeser indeks item terpasang setelah posisi item berubah (sort atau jual) */
void sinkronkanEquip(Player *p, int idSenjataLama, int idArmorLama)
{
    int i;
    p->idxSenjata = -1;
    p->idxArmor = -1;
    for (i = 0; i < p->jumlahItem; i++) {
        if (idSenjataLama >= 0 && p->inventory[i].id == idSenjataLama) p->idxSenjata = i;
        if (idArmorLama >= 0 && p->inventory[i].id == idArmorLama) p->idxArmor = i;
    }
}

/* Menu inventory: lihat, urutkan, cari, equip */
void menuInventory(Player *p)
{
    int pilih;

    for (;;) {
        garis();
        tampilInventory(p);
        printf("=== Inventory ===\n");
        printf("1. Urutkan\n");
        printf("2. Cari item\n");
        printf("3. Equip item\n");
        printf("0. Kembali\n");
        pilih = bacaAngka("Pilih: ", 0, 3);
        if (pilih == 0) return;
        switch (pilih) {
        case 1: {
            int mode = bacaAngka("Urutkan berdasarkan (1 rarity, 2 power, 3 nama): ", 1, 3);
            urutkanInventory(p, mode);
            printf("Inventory terurut.\n");
            break;
        }
        case 2: {
            char kata[MAX_NAMA];
            int ketemu;
            bacaString("Cari nama item: ", kata, MAX_NAMA);
            ketemu = cariItemNama(p, kata);
            if (ketemu == 0) printf("Tidak ada item yang cocok.\n");
            else printf("Ditemukan %d item.\n", ketemu);
            break;
        }
        case 3: {
            int idx;
            if (p->jumlahItem == 0) {
                printf("Inventory kosong.\n");
                break;
            }
            idx = bacaAngka("Nomor indeks item yang dipasang (999 batal): ", 0, 999);
            if (idx == 999) break;
            if (idx >= p->jumlahItem) {
                printf("Indeks tidak ada di inventory.\n");
                break;
            }
            equipItem(p, idx);
            printf("%s dipasang.\n", p->inventory[idx].nama);
            break;
        }
        }
    }
}

/* Mengurutkan inventory: 1 rarity, 2 power menurun, 3 nama A-Z (tidak peka kapital) */
void urutkanInventory(Player *p, int mode)
{
    int idSenjataLama = -1, idArmorLama = -1;
    int i, j;

    if (p->idxSenjata >= 0) idSenjataLama = p->inventory[p->idxSenjata].id;
    if (p->idxArmor >= 0) idArmorLama = p->inventory[p->idxArmor].id;
    for (i = 0; i < p->jumlahItem - 1; i++) {
        for (j = 0; j < p->jumlahItem - 1 - i; j++) {
            int tukar = 0;
            char a[MAX_NAMA], b[MAX_NAMA];
            switch (mode) {
            case 1: tukar = p->inventory[j].rarity > p->inventory[j + 1].rarity; break;
            case 2: tukar = p->inventory[j].power < p->inventory[j + 1].power; break;
            case 3:
                strncpy(a, p->inventory[j].nama, MAX_NAMA);
                strncpy(b, p->inventory[j + 1].nama, MAX_NAMA);
                a[MAX_NAMA - 1] = '\0'; b[MAX_NAMA - 1] = '\0';
                {
                    int k;
                    for (k = 0; a[k] != '\0'; k++) a[k] = (char)tolower((unsigned char)a[k]);
                    for (k = 0; b[k] != '\0'; k++) b[k] = (char)tolower((unsigned char)b[k]);
                }
                tukar = strcmp(a, b) > 0;
                break;
            }
            if (tukar) {
                Item tmp = p->inventory[j];
                p->inventory[j] = p->inventory[j + 1];
                p->inventory[j + 1] = tmp;
            }
        }
    }
    sinkronkanEquip(p, idSenjataLama, idArmorLama);
}

/* Mencari item yang namanya memuat 'kata' (tidak peka kapital); kembalikan jumlah dan tampilkan hasil */
int cariItemNama(const Player *p, const char *kata)
{
    char kecilKata[MAX_NAMA];
    int i, k, ketemu = 0;

    strncpy(kecilKata, kata, MAX_NAMA - 1);
    kecilKata[MAX_NAMA - 1] = '\0';
    for (k = 0; kecilKata[k] != '\0'; k++) kecilKata[k] = (char)tolower((unsigned char)kecilKata[k]);
    for (i = 0; i < p->jumlahItem; i++) {
        char kecilNama[MAX_NAMA];
        strncpy(kecilNama, p->inventory[i].nama, MAX_NAMA - 1);
        kecilNama[MAX_NAMA - 1] = '\0';
        for (k = 0; kecilNama[k] != '\0'; k++) kecilNama[k] = (char)tolower((unsigned char)kecilNama[k]);
        if (strstr(kecilNama, kecilKata) != NULL) {
            printf("Ditemukan: idx %d %s (tipe %c, power %d)\n",
                   i, p->inventory[i].nama, p->inventory[i].tipe, p->inventory[i].power);
            ketemu++;
        }
    }
    return ketemu;
}

/* Memasang item pada slot senjata atau armor, menggantikan yang lama */
void equipItem(Player *p, int idx)
{
    if (p->inventory[idx].tipe == 'S') p->idxSenjata = idx;
    else if (p->inventory[idx].tipe == 'A') p->idxArmor = idx;
}
