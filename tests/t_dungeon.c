#include <stdio.h>
#include <string.h>
#include "config.h"
#include "dungeon.h"

#define CEK(k) do { if (!(k)) { printf("GAGAL: %s baris %d\n", #k, __LINE__); return 1; } } while (0)

int main(void)
{
    char peta[MAX_BARIS][MAX_KOLOM];
    Titik s, e, jalur[MAX_BARIS * MAX_KOLOM];
    int ok, len, i;
    Titik jalan[MAX_BARIS * MAX_KOLOM];

    ok = muatPeta("data/dungeon1.txt", peta, &s, &e);
    CEK(ok == 1);
    CEK(s.baris == 1 && s.kolom == 1 && e.baris == 3 && e.kolom == 7);
    CEK(dfsBisaDicapai(peta, s, e) == 1);
    len = bfsJalurTerpendek(peta, s, e, jalur);
    CEK(len == 13);
    CEK(jalur[0].baris == s.baris && jalur[0].kolom == s.kolom);
    CEK(jalur[len - 1].baris == e.baris && jalur[len - 1].kolom == e.kolom);
    for (i = 0; i < len - 1; i++) {
        int db = jalur[i + 1].baris - jalur[i].baris;
        int dk = jalur[i + 1].kolom - jalur[i].kolom;
        CEK((db == 0 ? (dk == 1 || dk == -1) : (dk == 0 && (db == 1 || db == -1))) && (db == 0 || dk == 0));
    }

    ok = muatPeta("tests/dungeon_invalid.txt", peta, &s, &e);
    CEK(ok == 1);
    CEK(dfsBisaDicapai(peta, s, e) == 0);
    CEK(bfsJalurTerpendek(peta, s, e, jalan) == -1);

    ok = muatPeta("data/dungeon2.txt", peta, &s, &e);
    CEK(ok == 1 && dfsBisaDicapai(peta, s, e) == 1 && bfsJalurTerpendek(peta, s, e, jalan) > 0);

    ok = muatPeta("data/dungeon3.txt", peta, &s, &e);
    CEK(ok == 1 && dfsBisaDicapai(peta, s, e) == 1 && bfsJalurTerpendek(peta, s, e, jalan) == 44);

    ok = muatPeta("tests/dungeon_toobig.txt", peta, &s, &e);
    CEK(ok == 0);

    ok = muatPeta("data/tidak_ada.txt", peta, &s, &e);
    CEK(ok == 0);
    return 0;
}
