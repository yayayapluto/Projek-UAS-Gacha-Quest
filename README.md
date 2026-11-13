# Gacha Quest

Mini RPG + Gacha berbasis CLI (Bahasa C) sesuai `docs/PRD Gacha Quest.md`.

## Cara Kompilasi

Butuh `gcc` (C99) dan `make` (GNU make).

- Linux / WSL:
  ```
  make            # kompilasi ke bin/game, tanpa warning (-Wall -Wextra)
  make test       # jalankan 15 skenario uji otomatis
  make debug      # kompilasi + uji dengan AddressSanitizer/UBSan
  make clean
  ```
- Windows (MSYS2 / MinGW): tambahkan `C:\msys64\usr\bin` ke PATH agar `sh`,
  `rm`, `grep`, `cp` tersedia untuk Makefile, lalu jalankan perintah yang sama.
  Target `make debug` otomatis memakai clang64 (`C:\msys64\clang64\bin\clang.exe`)
  karena runtime sanitizer milik gcc MSYS2 mungkin belum terpasang.

Buka shell pada folder proyek (root tempat Makefile berada) sebelum
menjalankan make (file data dibaca dengan path relatif).

## Cara Bermain

```
bin/game          (Linux: ./bin/game)
```

1. **Game Baru** → masukkan nama → dapat HP 100, ATK 10, DEF 3, 200 gold, 2 potion.
2. **Battle** (hub menu 1) → monster acak sesuai level; serang / bertahan /
   potion / kabur. Menang: EXP + gold + skor. Kalah: gold setengah, HP pulih.
3. **Gacha** (hub menu 2) → 100 gold per tarikan, 900 gold per 10x. Rarity:
   Common/Rare/Epic/Legendary; pity dijamin Epic setelah 10 tarikan tanpa
   Epic+. Item duplikat otomatis jadi gold.
4. **Inventory** (hub menu 3) → lihat, urutkan (rarity/power/nama), cari, equip.
5. **Shop** (hub menu 4) → beli potion (40 gold, maks 20), jual item.
6. **Dungeon** (hub menu 5) → peta dari file, gerak WASD, T = +30 gold,
   E = exit (+100 gold, +40 EXP, +50 skor, batas langkah 2x jalur terpendek),
   H = hint BFS (30 gold, maks 3x), Q = keluar.
7. **Simpan** (hub menu 7) atau Kembali ke menu utama (dipaksa menawarkan simpan,
   lalu skor dicatat ke leaderboard).
8. Menu utama **Lanjutkan** memuat data/save.txt; **Leaderboard** menampilkan top 5.

## Daftar Modul

| File | Isi |
| --- | --- |
| main.c | Menu utama dan loop hub game |
| config.h | Konstanta dan struct Item, Monster, Player, Titik |
| util.c/.h | bacaAngka, bacaString, bersihkanLayar, garis, shutdownGame |
| player.c/.h | initPlayer, tambahExp, hitungAtk/Def, tampilStatus |
| battle.c/.h | mulaiBattle, hitungDamage |
| gacha.c/.h | menuGacha, tentukanRarity, cariItemDiInventory, tarikSekali |
| inventory.c/.h | menuInventory, urutkanInventory, cariItemNama, equipItem |
| shop.c/.h | menuShop (beli potion, jual item) |
| storage.c/.h | muatKatalog, muatMonster, simpanGame, muatGame, catatSkor, tampilLeaderboard |
| dungeon.c/.h | muatPeta, dfsBisaDicapai, bfsJalurTerpendek, jalankanDungeon |
| data/ | item.txt, monster.txt, dungeon1-3.txt, save.txt, leaderboard.txt |
| tests/ | Driver uji C + skenario input pipa + run_tests.sh |

## Pembagian Tugas (anggota A-E, sesuai PRD bagian 13)

| Anggota | Peran | Modul |
| --- | --- | --- |
| A | Arsitek dungeon | config.h, dungeon.c/.h, data peta |
| B | Integrator | main.c, battle.c/.h, storage.c/.h |
| C | Gacha | gacha.c/.h |
| D | Inventory | inventory.c/.h |
| E | Shop dan status | shop.c/.h, player.c/.h, util.c/.h |

## Pengujian

`make test` menjalankan `tests/run_tests.sh`:
- Driver C (logika murni): bacaAngka/bacaString, level & EXP, damage,
  rarity/pity/duplikat, sort & search inventory, katalog/monster/save-load
  round-trip, leaderboard, BFS/DFS/peta.
- Skenario menu: input pipa dari file ke program utama, hasil dicek grep.
- `make debug` mengulang seluruh pengujian dengan AddressSanitizer + UBSan.

## Demo alur (new game -> battle -> gacha -> equip -> dungeon -> save -> load -> leaderboard)

Tiga run input pipa yang sudah disiapkan di `tests/`:

```
bin/game < tests/in_demoA.txt   # game baru Demo: gacha, equip, dungeon, simpan
bin/game < tests/in_demoB.txt   # lanjutkan, status, skor dicatat, leaderboard
bin/game < tests/in_demoC.txt   # lanjutkan, battle beberapa kali (EOF menghentikan dengan rapi)
```
