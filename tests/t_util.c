#include <stdio.h>
#include <string.h>
#include "config.h"
#include "util.h"
#include "player.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    Player p;
    int number;
    char name[MAX_NAME];
    number = readInt("Test: ", 1, 5);
    CHECK(number == 3);
    readString("Test name: ", name, MAX_NAME);
    CHECK(strlen(name) == 29);
    initPlayer(&p, name);
    CHECK(strcmp(p.name, name) == 0);
    return 0;
}
