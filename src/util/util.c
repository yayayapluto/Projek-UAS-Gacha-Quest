#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

/* Membaca satu baris stdin; mengembalikan 1 jika ada data, 0 jika EOF */
int bacaBaris(char *buf, int maks)
{
    if (fgets(buf, maks, stdin) == NULL) return 0;
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

int bacaAngka(const char *prompt, int min, int max)
{
    char buf[256];
    char *akhir;
    long nilai;

    for (;;) {
        printf("%s", prompt);
        if (!bacaBaris(buf, sizeof buf)) {
            printf("Input berakhir (EOF), memakai nilai %d.\n", min);
            return min;
        }
        nilai = strtol(buf, &akhir, 10);
        if (akhir == buf) {
            printf("Input tidak valid, masukkan angka %d sampai %d.\n", min, max);
            continue;
        }
        while (*akhir != '\0' && isspace((unsigned char)*akhir)) akhir++;
        if (*akhir != '\0') {
            printf("Input tidak valid, masukkan angka %d sampai %d.\n", min, max);
            continue;
        }
        if (nilai < min || nilai > max) {
            printf("Angka harus %d sampai %d.\n", min, max);
            continue;
        }
        return (int)nilai;
    }
}

void bacaString(const char *prompt, char *out, int maks)
{
    char buf[256];
    out[0] = '\0';
    for (;;) {
        printf("%s", prompt);
        if (!bacaBaris(buf, sizeof buf)) {
            printf("Input berakhir (EOF).\n");
            return;
        }
        if (buf[0] == '\0') {
            printf("Input tidak boleh kosong.\n");
            continue;
        }
        strncpy(out, buf, (size_t)maks - 1);
        out[maks - 1] = '\0';
        return;
    }
}

void bersihkanLayar(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void garis(void)
{
    printf("------------------------------\n");
}

/* Menutup program secara rapi: goodbye, flush semua output, tanpa sisa loop */
void shutdownGame(void)
{
    printf("Terima kasih sudah bermain Gacha Quest!\n");
    fflush(stdout);
}
