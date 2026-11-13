#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

/* Reads one line from stdin; returns 1 when data is available, 0 on EOF. */
int readLine(char *buf, int max)
{
    if (fgets(buf, max, stdin) == NULL) return 0;
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

int readInt(const char *prompt, int min, int max)
{
    char buf[256];
    char *end;
    long value;

    for (;;) {
        printf("%s", prompt);
        if (!readLine(buf, sizeof buf)) {
            printf("Input berakhir (EOF), memakai nilai %d.\n", min);
            return min;
        }
        value = strtol(buf, &end, 10);
        if (end == buf) {
            printf("Input tidak valid, masukkan angka %d sampai %d.\n", min, max);
            continue;
        }
        while (*end != '\0' && isspace((unsigned char)*end)) end++;
        if (*end != '\0') {
            printf("Input tidak valid, masukkan angka %d sampai %d.\n", min, max);
            continue;
        }
        if (value < min || value > max) {
            printf("Angka harus %d sampai %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

void readString(const char *prompt, char *out, int max)
{
    char buf[256];
    out[0] = '\0';
    for (;;) {
        printf("%s", prompt);
        if (!readLine(buf, sizeof buf)) {
            printf("Input berakhir (EOF).\n");
            return;
        }
        if (buf[0] == '\0') {
            printf("Input tidak boleh kosong.\n");
            continue;
        }
        strncpy(out, buf, (size_t)max - 1);
        out[max - 1] = '\0';
        return;
    }
}

void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine(void)
{
    printf("------------------------------\n");
}

/* Single exit path: goodbye message, then flush all output. */
void shutdownGame(void)
{
    printf("Terima kasih sudah bermain Gacha Quest!\n");
    fflush(stdout);
}
