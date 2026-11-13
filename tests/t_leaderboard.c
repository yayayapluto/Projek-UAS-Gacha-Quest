#include <stdio.h>
#include "config.h"
#include "storage.h"
#include "player.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    Player p;
    FILE *fp;
    char baris[256];
    int skor[20], i, jml = 0, urut = 1;

    remove("data/leaderboard.txt");

    initPlayer(&p, "Andi");
    p.skor = 100;
    catatSkor(&p);
    initPlayer(&p, "Budi");
    p.skor = 50;
    catatSkor(&p);
    initPlayer(&p, "Citra");
    p.skor = 200;
    catatSkor(&p);

    fp = fopen("data/leaderboard.txt", "r");
    CEK(fp != NULL);
    while (fgets(baris, sizeof baris, fp) != NULL && jml < 20) {
        if (sscanf(baris, "%*[^|]|%*d|%d", &skor[jml]) == 1) jml++;
    }
    fclose(fp);
    CEK(jml == 3);
    for (i = 0; i < jml - 1; i++)
        if (skor[i] < skor[i + 1]) urut = 0;
    CEK(urut == 1);
    CEK(skor[0] == 200 && skor[1] == 100 && skor[2] == 50);

    remove("data/leaderboard.txt");
    for (i = 1; i <= 12; i++) {
        char nama[MAX_NAMA];
        sprintf(nama, "P%d", i);
        initPlayer(&p, nama);
        p.skor = i * 10;
        catatSkor(&p);
    }
    fp = fopen("data/leaderboard.txt", "r");
    CEK(fp != NULL);
    jml = 0;
    while (fgets(baris, sizeof baris, fp) != NULL) jml++;
    fclose(fp);
    CEK(jml == MAX_LEADERBOARD);

    fp = fopen("data/leaderboard.txt", "r");
    jml = 0;
    while (fgets(baris, sizeof baris, fp) != NULL && jml < 20) {
        if (sscanf(baris, "%*[^|]|%*d|%d", &skor[jml]) == 1) jml++;
    }
    fclose(fp);
    CEK(skor[0] == 120 && skor[MAX_LEADERBOARD - 1] == 30);

    tampilLeaderboard();
    return 0;
}
