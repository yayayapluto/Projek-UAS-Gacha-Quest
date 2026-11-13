#include <stdio.h>
#include "config.h"
#include "battle.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    int i, dmg;
    for (i = 0; i < 1000; i++) {
        dmg = calculateDamage(10, 3);
        CHECK(dmg >= 7 && dmg <= 10);
        dmg = calculateDamage(5, 20);
        CHECK(dmg >= 1 && dmg <= 4);
    }
    return 0;
}
