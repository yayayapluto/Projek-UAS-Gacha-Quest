#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "battle.h"
#include "util.h"
#include "player.h"

/* Rumus damage: max(1, ATK - DEF) + rand()%4 */
int hitungDamage(int atk, int def)
{
    int dasar = atk - def;
    if (dasar < 1) dasar = 1;
    return dasar + rand() % 4;
}

/* Menerapkan damage ke target dengan peluang kritis 10% (x2); kembalikan damage yang terjadi */
int serang(const char *dari, const char *ke, int atk, int defLawan, int *hpLawan, int *kritis)
{
    int dmg = hitungDamage(atk, defLawan);
    *kritis = 0;
    if (rand() % 100 < 10) {
        dmg *= 2;
        *kritis = 1;
    }
    *hpLawan -= dmg;
    printf("%s menyerang %s: %d damage%s\n", dari, ke, dmg, *kritis ? " (KRITIS!)" : "");
    return dmg;
}

/* Memilih monster acak yang levelMin-nya <= level pemain; kembalikan indeks */
int pilihMonster(const Monster m[], int jml, int levelPemain)
{
    int kandidat[MAX_MONSTER];
    int jmlKandidat = 0, i;

    for (i = 0; i < jml; i++)
        if (m[i].levelMin <= levelPemain) {
            kandidat[jmlKandidat] = i;
            jmlKandidat++;
        }
    if (jmlKandidat == 0) return -1;
    return kandidat[rand() % jmlKandidat];
}

/* Seluruh loop battle bergiliran sampai menang, kalah, atau kabur */
void mulaiBattle(Player *p, const Monster m[], int jml)
{
    int idxM = pilihMonster(m, jml, p->level);
    Monster lawan;
    int hpMusuh;
    int kritis;
    int hidup = 1;

    if (idxM < 0) {
        printf("Tidak ada monster yang tersedia.\n");
        return;
    }
    lawan = m[idxM];
    hpMusuh = lawan.hp;
    printf("Monster muncul: %s (HP %d, ATK %d, DEF %d)\n", lawan.nama, lawan.hp, lawan.atk, lawan.def);

    while (hidup) {
        int aksi;
        printf("--- Battle vs %s ---\n", lawan.nama);
        printf("HP kamu: %d/%d (ATK %d, DEF %d, potion %d) | HP %s: %d\n",
               p->hp, p->maxHp, hitungAtk(p), hitungDef(p), p->potion, lawan.nama, hpMusuh);
        printf("1. Serang\n2. Bertahan\n3. Potion\n4. Kabur\n");
        aksi = bacaAngka("Aksi: ", 1, 4);
        if (feof(stdin)) {
            printf("Battle dihentikan (input berakhir).\n");
            hidup = 0;
            break;
        }

        switch (aksi) {
        case 1: {
            serang(p->nama, lawan.nama, hitungAtk(p), lawan.def, &hpMusuh, &kritis);
            if (hpMusuh <= 0) {
                printf("%s kalah! Kamu menang!\n", lawan.nama);
                printf("Hadiah: +%d EXP, +%d gold, +%d skor.\n", lawan.exp, lawan.gold, lawan.exp);
                tambahExp(p, lawan.exp);
                p->gold += lawan.gold;
                p->skor += lawan.exp;
                hidup = 0;
                break;
            }
            serang(lawan.nama, p->nama, lawan.atk, hitungDef(p), &p->hp, &kritis);
            break;
        }
        case 2: {
            int dmg;
            printf("Kamu bertahan.\n");
            dmg = hitungDamage(lawan.atk, hitungDef(p));
            if (rand() % 100 < 10) {
                dmg *= 2;
                printf("%s menyerang: %d damage (KRITIS!)\n", lawan.nama, dmg);
            } else {
                printf("%s menyerang: %d damage\n", lawan.nama, dmg);
            }
            dmg /= 2;
            if (dmg < 1) dmg = 1;
            p->hp -= dmg;
            printf("Karena bertahan, damage masuk hanya %d.\n", dmg);
            break;
        }
        case 3:
            if (p->potion <= 0) {
                printf("Potion habis! (giliran tidak terpakai)\n");
                break;
            }
            p->potion--;
            p->hp += 50;
            if (p->hp > p->maxHp) p->hp = p->maxHp;
            printf("Potion dipakai, HP sekarang %d/%d.\n", p->hp, p->maxHp);
            serang(lawan.nama, p->nama, lawan.atk, hitungDef(p), &p->hp, &kritis);
            break;
        case 4:
            if (rand() % 2 == 0) {
                printf("Kabur berhasil! Battle selesai tanpa hadiah.\n");
                hidup = 0;
            } else {
                printf("Kabur gagal!\n");
                serang(lawan.nama, p->nama, lawan.atk, hitungDef(p), &p->hp, &kritis);
            }
            break;
        }

        if (p->hp <= 0) {
            printf("Kamu dikalahkan oleh %s...\n", lawan.nama);
            p->gold /= 2;
            p->hp = p->maxHp;
            printf("Gold tinggal %d, HP dipulihkan penuh. Kembali ke hub.\n", p->gold);
            hidup = 0;
        }
    }
}
