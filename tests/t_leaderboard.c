#include <stdio.h>
#include "config.h"
#include "storage.h"
#include "player.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    Player p;
    FILE *fp;
    char line[256];
    int scores[20], i, count = 0, sorted = 1;

    remove("data/leaderboard.txt");

    initPlayer(&p, "Andi");
    p.score = 100;
    recordScore(&p);
    initPlayer(&p, "Budi");
    p.score = 50;
    recordScore(&p);
    initPlayer(&p, "Citra");
    p.score = 200;
    recordScore(&p);

    fp = fopen("data/leaderboard.txt", "r");
    CHECK(fp != NULL);
    while (fgets(line, sizeof line, fp) != NULL && count < 20) {
        if (sscanf(line, "%*[^|]|%*d|%d", &scores[count]) == 1) count++;
    }
    fclose(fp);
    CHECK(count == 3);
    for (i = 0; i < count - 1; i++)
        if (scores[i] < scores[i + 1]) sorted = 0;
    CHECK(sorted == 1);
    CHECK(scores[0] == 200 && scores[1] == 100 && scores[2] == 50);

    remove("data/leaderboard.txt");
    for (i = 1; i <= 12; i++) {
        char name[MAX_NAME];
        sprintf(name, "P%d", i);
        initPlayer(&p, name);
        p.score = i * 10;
        recordScore(&p);
    }
    fp = fopen("data/leaderboard.txt", "r");
    CHECK(fp != NULL);
    count = 0;
    while (fgets(line, sizeof line, fp) != NULL) count++;
    fclose(fp);
    CHECK(count == MAX_LEADERBOARD);

    fp = fopen("data/leaderboard.txt", "r");
    count = 0;
    while (fgets(line, sizeof line, fp) != NULL && count < 20) {
        if (sscanf(line, "%*[^|]|%*d|%d", &scores[count]) == 1) count++;
    }
    fclose(fp);
    CHECK(scores[0] == 120 && scores[MAX_LEADERBOARD - 1] == 30);

    showLeaderboard();
    return 0;
}
