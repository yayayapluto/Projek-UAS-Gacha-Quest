#ifndef CONFIG_H
#define CONFIG_H

#define MAX_ITEM 16
#define MAX_MONSTER 10
#define MAX_NAMA 30

#define MAX_BARIS 20
#define MAX_KOLOM 40
#define MAX_PETA 3

#define MAKS_LEVEL 10
#define MAKS_POTION 20
#define MAX_LEADERBOARD 10
#define TOP_TAMPIL 5

#define BIAYA_PULL 100
#define BIAYA_PULL10 900
#define BIAYA_POTION 40
#define BIAYA_HINT 30

#define MAKS_HINT 3
#define MAKS_PITY 10

#define GOLD_TREASURE 30
#define GOLD_EXIT 100
#define EXP_EXIT 40
#define SKOR_EXIT 50

#define RARITY_COMMON 1
#define RARITY_RARE 2
#define RARITY_EPIC 3
#define RARITY_LEGENDARY 4

#define HARGA_DUP_COMMON 20
#define HARGA_DUP_RARE 50
#define HARGA_DUP_EPIC 150
#define HARGA_DUP_LEGENDARY 500

typedef struct {
    int id;
    char nama[MAX_NAMA];
    char tipe;      /* 'S' senjata, 'A' armor */
    int rarity;     /* 1 Common, 2 Rare, 3 Epic, 4 Legendary */
    int power;
} Item;

typedef struct {
    int id;
    char nama[MAX_NAMA];
    int hp, atk, def;
    int exp, gold, levelMin;
} Monster;

typedef struct {
    char nama[MAX_NAMA];
    int hp, maxHp, atk, def;
    int level, exp, gold, potion;
    int pity, totalPull, skor;
    Item inventory[MAX_ITEM];
    int jumlahItem;
    int idxSenjata, idxArmor;   /* -1 jika kosong */
} Player;

typedef struct { int baris, kolom; } Titik;

#endif
