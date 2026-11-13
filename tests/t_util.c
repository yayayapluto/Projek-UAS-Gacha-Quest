#include <stdio.h>
#include <string.h>
#include "config.h"
#include "util.h"
#include "player.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    Player p;
    int angka;
    char nama[MAX_NAMA];

    angka = bacaAngka("Uji: ", 1, 5);
    CEK(angka == 3);
    bacaString("Uji nama: ", nama, MAX_NAMA);
    CEK(strlen(nama) == 29);
    initPlayer(&p, nama);
    CEK(strcmp(p.nama, nama) == 0);
    return 0;
}
