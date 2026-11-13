#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "util.h"
#include "player.h"
#include "battle.h"
#include "gacha.h"
#include "inventory.h"
#include "shop.h"
#include "storage.h"
#include "dungeon.h"

/* Menampilkan menu utama dan mengembalikan pilihan pemain */
int menuUtama(void)
{
    garis();
    printf("       GACHA QUEST\n");
    printf(" 1. Game Baru\n");
    printf(" 2. Lanjutkan\n");
    printf(" 3. Leaderboard\n");
    printf(" 0. Keluar\n");
    return bacaAngka("Pilih: ", 0, 3);
}

/* Menampilkan hub game dan mengembalikan pilihan pemain */
int menuHub(const Player *p)
{
    garis();
    printf("--- Hub | %s | Lv %d | HP %d/%d | Gold %d | Skor %d ---\n",
           p->nama, p->level, p->hp, p->maxHp, p->gold, p->skor);
    printf(" 1. Battle\n");
    printf(" 2. Gacha\n");
    printf(" 3. Inventory\n");
    printf(" 4. Shop\n");
    printf(" 5. Dungeon\n");
    printf(" 6. Status\n");
    printf(" 7. Simpan\n");
    printf(" 0. Kembali ke menu utama\n");
    return bacaAngka("Pilih: ", 0, 7);
}

/* Loop hub game: semua fitur bisa diakses dari sini */
void jalankanHub(Player *p, const Item katalog[], int jmlKatalog, const Monster monster[], int jmlMonster)
{
    int sudahSimpan = 0;

    for (;;) {
        int pilih = menuHub(p);
        switch (pilih) {
        case 1: mulaiBattle(p, monster, jmlMonster); break;
        case 2: menuGacha(p, katalog, jmlKatalog); break;
        case 3: menuInventory(p); break;
        case 4: menuShop(p); break;
        case 5: jalankanDungeon(p); break;
        case 6: tampilStatus(p); break;
        case 7: sudahSimpan = simpanGame(p); break;
        case 0: {
            char jawab[MAX_NAMA];
            if (!sudahSimpan) {
                bacaString("Simpan sebelum keluar? (y/n): ", jawab, MAX_NAMA);
                if (jawab[0] == 'y' || jawab[0] == 'Y') simpanGame(p);
            }
            catatSkor(p);
            printf("Kembali ke menu utama.\n");
            return;
        }
        }
    }
}

/* Fungsi utama: memuat katalog dan monster, lalu loop menu utama */
int main(void)
{
    Item katalog[MAX_ITEM];
    Monster monster[MAX_MONSTER];
    Player pemain;
    int jmlKatalog, jmlMonster;
    int jalan = 1;

    srand((unsigned)time(NULL));
    jmlKatalog = muatKatalog("data/item.txt", katalog);
    jmlMonster = muatMonster("data/monster.txt", monster);
    if (jmlKatalog == 0 || jmlMonster == 0) {
        printf("Katalog item atau monster tidak bisa dimuat, program berhenti.\n");
        return 1;
    }
    memset(&pemain, 0, sizeof pemain);

    while (jalan) {
        int pilih = menuUtama();
        switch (pilih) {
        case 1: {
            char nama[MAX_NAMA];
            bacaString("Masukkan nama pemain: ", nama, MAX_NAMA);
            initPlayer(&pemain, nama);
            printf("Selamat datang, %s! Kamu dapat 200 gold.\n", pemain.nama);
            jalankanHub(&pemain, katalog, jmlKatalog, monster, jmlMonster);
            break;
        }
        case 2:
            if (muatGame(&pemain, katalog, jmlKatalog)) {
                jalankanHub(&pemain, katalog, jmlKatalog, monster, jmlMonster);
            }
            break;
        case 3:
            tampilLeaderboard();
            break;
        case 0:
            shutdownGame();
            jalan = 0;
            break;
        }
    }
    return 0;
}
