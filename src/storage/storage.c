#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "storage.h"
#include "util.h"

/* Membaca katalog item dari file; kembalikan jumlah item yang berhasil dibaca */
int muatKatalog(const char *file, Item k[])
{
    FILE *fp = fopen(file, "r");
    char baris[256];
    Item it;
    int jml = 0;

    if (fp == NULL) {
        printf("File katalog %s tidak bisa dibuka.\n", file);
        return 0;
    }
    while (jml < MAX_ITEM && fgets(baris, sizeof baris, fp) != NULL) {
        if (baris[0] == '\0' || baris[0] == '\n' || baris[0] == '\r') continue;
        if (sscanf(baris, "%d|%29[^|]| %c|%d|%d", &it.id, it.nama, &it.tipe, &it.rarity, &it.power) == 5) {
            if (it.rarity < RARITY_COMMON || it.rarity > RARITY_LEGENDARY) continue;
            if (it.tipe != 'S' && it.tipe != 'A') continue;
            k[jml] = it;
            jml++;
        }
    }
    fclose(fp);
    return jml;
}

/* Membaca tabel monster dari file; kembalikan jumlah monster */
int muatMonster(const char *file, Monster m[])
{
    FILE *fp = fopen(file, "r");
    char baris[256];
    Monster mo;
    int jml = 0;

    if (fp == NULL) {
        printf("File monster %s tidak bisa dibuka.\n", file);
        return 0;
    }
    while (jml < MAX_MONSTER && fgets(baris, sizeof baris, fp) != NULL) {
        if (baris[0] == '\0' || baris[0] == '\n' || baris[0] == '\r') continue;
        if (sscanf(baris, "%d|%29[^|]|%d|%d|%d|%d|%d|%d",
                   &mo.id, mo.nama, &mo.hp, &mo.atk, &mo.def, &mo.exp, &mo.gold, &mo.levelMin) == 8) {
            m[jml] = mo;
            jml++;
        }
    }
    fclose(fp);
    return jml;
}

/* Menyimpan progres pemain ke data/save.txt sesuai format bagian 9; 1 jika sukses */
int simpanGame(const Player *p)
{
    FILE *fp = fopen("data/save.txt", "w");
    int idSenjata = -1, idArmor = -1, i;

    if (fp == NULL) {
        printf("Gagal membuka data/save.txt untuk menulis.\n");
        return 0;
    }
    if (p->idxSenjata >= 0) idSenjata = p->inventory[p->idxSenjata].id;
    if (p->idxArmor >= 0) idArmor = p->inventory[p->idxArmor].id;
    fprintf(fp, "%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d\n",
            p->nama, p->hp, p->maxHp, p->level, p->exp, p->gold, p->atk, p->def,
            p->potion, p->pity, p->totalPull, p->skor, idSenjata, idArmor);
    fprintf(fp, "%d\n", p->jumlahItem);
    for (i = 0; i < p->jumlahItem; i++)
        fprintf(fp, "%d\n", p->inventory[i].id);
    fclose(fp);
    printf("Progres tersimpan di data/save.txt.\n");
    return 1;
}

/* Memuat progres dari data/save.txt; 0 jika file tidak ada atau rusak */
int muatGame(Player *p, const Item k[], int jml)
{
    FILE *fp = fopen("data/save.txt", "r");
    char baris[512];
    Player tmp;
    int idSenjata, idArmor, jumlah, i, j, id;

    if (fp == NULL) {
        printf("Gagal memuat: data/save.txt tidak ada.\n");
        return 0;
    }
    memset(&tmp, 0, sizeof tmp);
    if (fgets(baris, sizeof baris, fp) == NULL ||
        sscanf(baris, "%29[^|]|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",
               tmp.nama, &tmp.hp, &tmp.maxHp, &tmp.level, &tmp.exp, &tmp.gold,
               &tmp.atk, &tmp.def, &tmp.potion, &tmp.pity, &tmp.totalPull,
               &tmp.skor, &idSenjata, &idArmor) != 14) {
        fclose(fp);
        printf("Gagal memuat: format data/save.txt rusak.\n");
        return 0;
    }
    if (fgets(baris, sizeof baris, fp) == NULL || sscanf(baris, "%d", &jumlah) != 1 ||
        jumlah < 0 || jumlah > MAX_ITEM) {
        fclose(fp);
        printf("Gagal memuat: jumlah item tidak valid.\n");
        return 0;
    }
    for (i = 0; i < jumlah; i++) {
        if (fgets(baris, sizeof baris, fp) == NULL || sscanf(baris, "%d", &id) != 1) {
            fclose(fp);
            printf("Gagal memuat: daftar item tidak lengkap.\n");
            return 0;
        }
        for (j = 0; j < jml; j++) {
            if (k[j].id == id) break;
        }
        if (j == jml) {
            fclose(fp);
            printf("Gagal memuat: item ID %d tidak ada di katalog.\n", id);
            return 0;
        }
        tmp.inventory[i] = k[j];
    }
    if (tmp.hp < 0 || tmp.maxHp <= 0 || tmp.hp > tmp.maxHp || tmp.level < 1 || tmp.level > MAKS_LEVEL ||
        tmp.exp < 0 || tmp.gold < 0 || tmp.atk < 0 || tmp.def < 0 ||
        tmp.potion < 0 || tmp.potion > MAKS_POTION ||
        tmp.pity < 0 || tmp.pity > MAKS_PITY || tmp.totalPull < 0 || tmp.skor < 0 ||
        idSenjata < -1 || idArmor < -1) {
        fclose(fp);
        printf("Gagal memuat: nilai di luar rentang.\n");
        return 0;
    }
    tmp.jumlahItem = jumlah;
    tmp.idxSenjata = -1;
    tmp.idxArmor = -1;
    for (i = 0; i < jumlah; i++) {
        if (idSenjata == tmp.inventory[i].id) tmp.idxSenjata = i;
        if (idArmor == tmp.inventory[i].id) tmp.idxArmor = i;
    }
    *p = tmp;
    fclose(fp);
    printf("Selamat datang kembali, %s!\n", tmp.nama);
    return 1;
}

/* Menambah skor pemain ke leaderboard, urut menurun, simpan hanya 10 teratas */
void catatSkor(const Player *p)
{
    char nama[MAX_LEADERBOARD][MAX_NAMA];
    int level[MAX_LEADERBOARD], skor[MAX_LEADERBOARD];
    FILE *fp;
    char baris[512];
    int jml = 0, i, j;
    char tmpNama[MAX_NAMA];
    int tmpLevel, tmpSkor;

    fp = fopen("data/leaderboard.txt", "r");
    if (fp != NULL) {
        while (jml < MAX_LEADERBOARD && fgets(baris, sizeof baris, fp) != NULL) {
            if (sscanf(baris, "%29[^|]|%d|%d", tmpNama, &tmpLevel, &tmpSkor) == 3) {
                strcpy(nama[jml], tmpNama);
                level[jml] = tmpLevel;
                skor[jml] = tmpSkor;
                jml++;
            }
        }
        fclose(fp);
    }
    if (jml < MAX_LEADERBOARD) {
        strcpy(nama[jml], p->nama);
        level[jml] = p->level;
        skor[jml] = p->skor;
        jml++;
    } else {
        jml = MAX_LEADERBOARD - 1;
        strcpy(nama[jml], p->nama);
        level[jml] = p->level;
        skor[jml] = p->skor;
        jml++;
    }
    for (i = 0; i < jml - 1; i++) {
        for (j = 0; j < jml - 1 - i; j++) {
            if (skor[j] < skor[j + 1]) {
                int tl = level[j], ts = skor[j];
                char tn[MAX_NAMA];
                strcpy(tn, nama[j]);
                level[j] = level[j + 1]; skor[j] = skor[j + 1]; strcpy(nama[j], nama[j + 1]);
                level[j + 1] = tl; skor[j + 1] = ts; strcpy(nama[j + 1], tn);
            }
        }
    }
    fp = fopen("data/leaderboard.txt", "w");
    if (fp == NULL) {
        printf("Gagal membuka data/leaderboard.txt untuk menulis.\n");
        return;
    }
    for (i = 0; i < jml && i < MAX_LEADERBOARD; i++)
        fprintf(fp, "%s|%d|%d\n", nama[i], level[i], skor[i]);
    fclose(fp);
    printf("Skor %s (%d) dicatat ke leaderboard.\n", p->nama, p->skor);
}

/* Menampilkan top 5 leaderboard */
void tampilLeaderboard(void)
{
    FILE *fp = fopen("data/leaderboard.txt", "r");
    char baris[512];
    char nama[MAX_NAMA];
    int level, skor, peringkat = 0;

    printf("=== Leaderboard (Top %d) ===\n", TOP_TAMPIL);
    if (fp == NULL) {
        printf("Belum ada data leaderboard.\n");
        return;
    }
    while (peringkat < TOP_TAMPIL && fgets(baris, sizeof baris, fp) != NULL) {
        if (sscanf(baris, "%29[^|]|%d|%d", nama, &level, &skor) == 3) {
            printf("%d. %-15s level %2d skor %d\n", peringkat + 1, nama, level, skor);
            peringkat++;
        }
    }
    if (peringkat == 0) printf("Belum ada data leaderboard.\n");
    fclose(fp);
}
