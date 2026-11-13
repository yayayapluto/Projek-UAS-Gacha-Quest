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

/* Shows the main menu and returns the player choice. */
int mainMenu(void)
{
    printLine();
    printf("       GACHA QUEST\n");
    printf(" 1. Game Baru\n");
    printf(" 2. Lanjutkan\n");
    printf(" 3. Leaderboard\n");
    printf(" 0. Keluar\n");
    return readInt("Pilih: ", 0, 3);
}

/* Shows the game hub and returns the player choice. */
int hubMenu(const Player *p)
{
    printLine();
    printf("--- Hub | %s | Lv %d | HP %d/%d | Gold %d | Skor %d ---\n",
           p->name, p->level, p->hp, p->maxHp, p->gold, p->score);
    printf(" 1. Battle\n");
    printf(" 2. Gacha\n");
    printf(" 3. Inventory\n");
    printf(" 4. Shop\n");
    printf(" 5. Dungeon\n");
    printf(" 6. Status\n");
    printf(" 7. Simpan\n");
    printf(" 0. Kembali ke menu utama\n");
    return readInt("Pilih: ", 0, 7);
}

/* Hub loop: every feature is reachable from here. */
void runHub(Player *p, const Item catalog[], int catalogCount, const Monster monsters[], int monsterCount)
{
    int saved = 0;

    for (;;) {
        int choice = hubMenu(p);
        switch (choice) {
        case 1: startBattle(p, monsters, monsterCount); break;
        case 2: gachaMenu(p, catalog, catalogCount); break;
        case 3: inventoryMenu(p); break;
        case 4: shopMenu(p); break;
        case 5: runDungeon(p); break;
        case 6: showStatus(p); break;
        case 7: saved = saveGame(p); break;
        case 0: {
            char answer[MAX_NAME];
            if (!saved) {
                readString("Simpan sebelum keluar? (y/n): ", answer, MAX_NAME);
                if (answer[0] == 'y' || answer[0] == 'Y') saveGame(p);
            }
            recordScore(p);
            printf("Kembali ke menu utama.\n");
            return;
        }
        }
    }
}

/* Entry point: load catalog and monsters, then run the main menu loop. */
int main(void)
{
    Item catalog[MAX_ITEM];
    Monster monsters[MAX_MONSTER];
    Player player;
    int catalogCount, monsterCount;
    int running = 1;

    srand((unsigned)time(NULL));
    catalogCount = loadCatalog("data/item.txt", catalog);
    monsterCount = loadMonsters("data/monster.txt", monsters);
    if (catalogCount == 0 || monsterCount == 0) {
        printf("Katalog item atau monster tidak bisa dimuat, program berhenti.\n");
        return 1;
    }
    memset(&player, 0, sizeof player);

    while (running) {
        int choice = mainMenu();
        switch (choice) {
        case 1: {
            char name[MAX_NAME];
            readString("Masukkan nama pemain: ", name, MAX_NAME);
            initPlayer(&player, name);
            printf("Selamat datang, %s! Kamu dapat 200 gold.\n", player.name);
            player.seed = promptWorldSeed();
            runHub(&player, catalog, catalogCount, monsters, monsterCount);
            break;
        }
        case 2:
            if (loadGame(&player, catalog, catalogCount)) {
                runHub(&player, catalog, catalogCount, monsters, monsterCount);
            }
            break;
        case 3:
            showLeaderboard();
            break;
        case 0:
            shutdownGame();
            running = 0;
            break;
        }
    }
    return 0;
}
