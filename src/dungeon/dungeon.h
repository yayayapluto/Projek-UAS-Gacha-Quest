#ifndef DUNGEON_H
#define DUNGEON_H

#include "config.h"

int muatPeta(const char *file, char peta[][MAX_KOLOM], Titik *s, Titik *e);
int muatPetaUkuran(const char *file, char peta[][MAX_KOLOM], Titik *s, Titik *e, int *baris, int *kolom);
int dfsBisaDicapai(char peta[][MAX_KOLOM], Titik dari, Titik ke);
int bfsJalurTerpendek(char peta[][MAX_KOLOM], Titik dari, Titik ke, Titik jalur[]);
void jalankanDungeon(Player *p);

#endif
