#include <stdio.h>
#include "config.h"
#include "battle.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    int i, dmg;

    for (i = 0; i < 1000; i++) {
        dmg = hitungDamage(10, 3);
        CEK(dmg >= 7 && dmg <= 10);
        dmg = hitungDamage(5, 20);
        CEK(dmg >= 1 && dmg <= 4);
    }
    return 0;
}
