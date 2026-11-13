#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "dungeon.h"
#include "util.h"
#include "player.h"

/* Membaca file peta, mengisi peta (sisanya '#'), mencari S dan E, ukuran nyata; 1 jika valid */
int muatPetaUkuran(const char *file, char peta[][MAX_KOLOM], Titik *s, Titik *e, int *baris, int *kolom)
{
    FILE *fp = fopen(file, "r");
    char teks[MAX_KOLOM * 2];
    int r = 0, c, i;
    int ketemuS = 0, ketemuE = 0;
    int lebar = 0;

    s->baris = -1; s->kolom = -1;
    e->baris = -1; e->kolom = -1;
    *baris = 0; *kolom = 0;
    if (fp == NULL) {
        printf("Peta %s tidak bisa dibuka.\n", file);
        return 0;
    }
    for (r = 0; r < MAX_BARIS; r++)
        for (i = 0; i < MAX_KOLOM; i++)
            peta[r][i] = '#';

    r = 0;
    while (r < MAX_BARIS && fgets(teks, sizeof teks, fp) != NULL) {
        teks[strcspn(teks, "\r\n")] = '\0';
        if (teks[0] == '\0') continue;
        if ((int)strlen(teks) > MAX_KOLOM) {
            fclose(fp);
            printf("Peta %s terlalu lebar (maks %d kolom).\n", file, MAX_KOLOM);
            return 0;
        }
        lebar = (int)strlen(teks);
        for (c = 0; teks[c] != '\0'; c++) {
            peta[r][c] = teks[c];
            if (teks[c] == 'S' && !ketemuS) { s->baris = r; s->kolom = c; ketemuS = 1; }
            if (teks[c] == 'E' && !ketemuE) { e->baris = r; e->kolom = c; ketemuE = 1; }
        }
        r++;
    }
    fclose(fp);
    if (r == 0 || !ketemuS || !ketemuE) {
        printf("Peta %s tidak punya S atau E.\n", file);
        return 0;
    }
    *baris = r;
    *kolom = lebar;
    return 1;
}

/* Kontrak 10.1: muat peta tanpa ukuran */
int muatPeta(const char *file, char peta[][MAX_KOLOM], Titik *s, Titik *e)
{
    int baris, kolom;
    return muatPetaUkuran(file, peta, s, e, &baris, &kolom);
}

/* DFS iteratif dengan stack array: 1 jika 'ke' tercapai dari 'dari' */
int dfsBisaDicapai(char peta[][MAX_KOLOM], Titik dari, Titik ke)
{
    Titik stack[MAX_BARIS * MAX_KOLOM];
    int dikunjungi[MAX_BARIS][MAX_KOLOM] = { { 0 } };
    int top = 0;
    int dBaris[4] = { -1, 1, 0, 0 };
    int dKolom[4] = { 0, 0, -1, 1 };
    int i;

    if (dari.baris < 0 || ke.baris < 0) return 0;
    stack[top] = dari;
    top++;
    dikunjungi[dari.baris][dari.kolom] = 1;
    while (top > 0) {
        Titik t = stack[--top];
        if (t.baris == ke.baris && t.kolom == ke.kolom) return 1;
        for (i = 0; i < 4; i++) {
            int b = t.baris + dBaris[i];
            int k = t.kolom + dKolom[i];
            if (b < 0 || b >= MAX_BARIS || k < 0 || k >= MAX_KOLOM) continue;
            if (peta[b][k] == '#' || dikunjungi[b][k]) continue;
            dikunjungi[b][k] = 1;
            stack[top].baris = b;
            stack[top].kolom = k;
            top++;
        }
    }
    return 0;
}

/* BFS dengan queue array dan array asal; isi jalur dari 'dari' ke 'ke', kembalikan panjang atau -1 */
int bfsJalurTerpendek(char peta[][MAX_KOLOM], Titik dari, Titik ke, Titik jalur[])
{
    Titik queue[MAX_BARIS * MAX_KOLOM];
    Titik asal[MAX_BARIS][MAX_KOLOM];
    int dikunjungi[MAX_BARIS][MAX_KOLOM] = { { 0 } };
    int front = 0, rear = 0;
    int dBaris[4] = { -1, 1, 0, 0 };
    int dKolom[4] = { 0, 0, -1, 1 };
    int i, panjang = 0;
    Titik jalan;

    if (dari.baris < 0 || ke.baris < 0) return -1;
    queue[rear] = dari;
    rear++;
    dikunjungi[dari.baris][dari.kolom] = 1;
    asal[dari.baris][dari.kolom].baris = -1;
    asal[dari.baris][dari.kolom].kolom = -1;

    while (front < rear) {
        Titik t = queue[front];
        front++;
        if (t.baris == ke.baris && t.kolom == ke.kolom) {
            jalan = t;
            while (jalan.baris != -1) {
                jalur[panjang] = jalan;
                panjang++;
                jalan = asal[jalan.baris][jalan.kolom];
            }
            for (i = 0; i < panjang / 2; i++) {
                Titik tmp = jalur[i];
                jalur[i] = jalur[panjang - 1 - i];
                jalur[panjang - 1 - i] = tmp;
            }
            return panjang;
        }
        for (i = 0; i < 4; i++) {
            int b = t.baris + dBaris[i];
            int k = t.kolom + dKolom[i];
            if (b < 0 || b >= MAX_BARIS || k < 0 || k >= MAX_KOLOM) continue;
            if (peta[b][k] == '#' || dikunjungi[b][k]) continue;
            dikunjungi[b][k] = 1;
            asal[b][k] = t;
            queue[rear].baris = b;
            queue[rear].kolom = k;
            rear++;
        }
    }
    return -1;
}

/* Menampilkan peta dengan posisi pemain 'P' dan jejak hint '*' */
void tampilPeta(char peta[][MAX_KOLOM], int baris, int kolom, Titik pos, char jejak[][MAX_KOLOM])
{
    int b, k;
    for (b = 0; b < baris; b++) {
        for (k = 0; k < kolom; k++) {
            if (b == pos.baris && k == pos.kolom) printf("P");
            else if (jejak != NULL && jejak[b][k] == '*') printf("*");
            else printf("%c", peta[b][k]);
        }
        printf("\n");
    }
}

/* Loop utama dungeon: pilih peta, bergerak, treasure, hint, exit */
void jalankanDungeon(Player *p)
{
    const char *filePeta[MAX_PETA] = { "data/dungeon1.txt", "data/dungeon2.txt", "data/dungeon3.txt" };
    char peta[MAX_BARIS][MAX_KOLOM];
    char jejak[MAX_BARIS][MAX_KOLOM];
    Titik s, e, pos, tmpS, tmpE, jalur[MAX_BARIS * MAX_KOLOM];
    int baris, kolom;
    int nomor[MAX_PETA];
    int jmlValid = 0, i, pilih;
    int langkah = 0, batas, hintDipakai = 0, selesai = 0;
    char buf[256];

    for (i = 0; i < MAX_PETA; i++) {
        if (muatPeta(filePeta[i], peta, &tmpS, &tmpE) && dfsBisaDicapai(peta, tmpS, tmpE)) {
            nomor[jmlValid] = i;
            jmlValid++;
        }
    }
    if (jmlValid == 0) {
        printf("Tidak ada peta dungeon yang valid.\n");
        return;
    }
    printf("Peta dungeon yang tersedia:\n");
    for (i = 0; i < jmlValid; i++) printf("  %d. Dungeon %d\n", i + 1, nomor[i] + 1);
    pilih = bacaAngka("Pilih peta: ", 1, jmlValid);
    i = nomor[pilih - 1];
    muatPetaUkuran(filePeta[i], peta, &s, &e, &baris, &kolom);

    pos = s;
    batas = bfsJalurTerpendek(peta, s, e, jalur) * 2;
    memset(jejak, ' ', sizeof jejak);
    printf("Batas langkah: %d. Gerak W/A/S/D, H hint (%d gold, maks %d), Q keluar.\n",
           batas, BIAYA_HINT, MAKS_HINT);

    while (!selesai) {
        tampilPeta(peta, baris, kolom, pos, jejak);
        printf("Posisi (%d,%d) | langkah %d/%d | HP %d | gold %d\n",
               pos.baris, pos.kolom, langkah, batas, p->hp, p->gold);
        printf("Gerak: ");
        if (!bacaBaris(buf, sizeof buf)) break;
        if (buf[0] == '\0' || buf[1] != '\0') {
            printf("Masukkan satu huruf: W/A/S/D, H, atau Q.\n");
            continue;
        }
        switch (toupper((unsigned char)buf[0])) {
        case 'W':
        case 'A':
        case 'S':
        case 'D': {
            int bBaris = 0, bKolom = 0;
            if (buf[0] == 'W' || buf[0] == 'w') bBaris = -1;
            if (buf[0] == 'S' || buf[0] == 's') bBaris = 1;
            if (buf[0] == 'A' || buf[0] == 'a') bKolom = -1;
            if (buf[0] == 'D' || buf[0] == 'd') bKolom = 1;
            {
                int nb = pos.baris + bBaris, nk = pos.kolom + bKolom;
                if (nb < 0 || nb >= baris || nk < 0 || nk >= kolom || peta[nb][nk] == '#') {
                    printf("Tembok! Tidak bisa lewat.\n");
                } else {
                    memset(jejak, ' ', sizeof jejak);
                    pos.baris = nb;
                    pos.kolom = nk;
                    langkah++;
                    if (peta[nb][nk] == 'T') {
                        peta[nb][nk] = '.';
                        p->gold += GOLD_TREASURE;
                        printf("Treasure! +%d gold.\n", GOLD_TREASURE);
                    } else if (peta[nb][nk] == 'E') {
                        if (langkah <= batas) {
                            printf("Exit tercapai! +%d gold, +%d EXP, +%d skor.\n",
                                   GOLD_EXIT, EXP_EXIT, SKOR_EXIT);
                            p->gold += GOLD_EXIT;
                            tambahExp(p, EXP_EXIT);
                            p->skor += SKOR_EXIT;
                        } else {
                            printf("Langkah melebihi batas, keluar tanpa hadiah exit.\n");
                        }
                        selesai = 1;
                    }
                    if (!selesai && langkah > batas) {
                        printf("Batas langkah terlampaui! Keluar dungeon tanpa hadiah exit.\n");
                        selesai = 1;
                    }
                }
            }
            break;
        }
        case 'H':
            if (hintDipakai >= MAKS_HINT) {
                printf("Hint sudah dipakai %d kali.\n", MAKS_HINT);
            } else if (p->gold < BIAYA_HINT) {
                printf("Gold kurang, hint butuh %d gold.\n", BIAYA_HINT);
            } else {
                p->gold -= BIAYA_HINT;
                hintDipakai++;
                memset(jejak, ' ', sizeof jejak);
                {
                    int len = bfsJalurTerpendek(peta, pos, e, jalur);
                    for (i = 0; i < len; i++) jejak[jalur[i].baris][jalur[i].kolom] = '*';
                }
                tampilPeta(peta, baris, kolom, pos, jejak);
                printf("Jalur terpendek ke E ditandai '*'. Sisa hint: %d.\n", MAKS_HINT - hintDipakai);
            }
            break;
        case 'Q':
            printf("Keluar dungeon.\n");
            selesai = 1;
            break;
        default:
            printf("Gerakan tidak dikenal.\n");
            break;
        }
    }
}
