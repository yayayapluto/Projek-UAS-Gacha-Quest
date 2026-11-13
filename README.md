# Gacha Quest

Mini RPG + Gacha berbasis CLI (Bahasa C) sesuai `docs/Projek UAS Daspro Gacha Quest 41774797398a802c9345c49291a6f695.md`.

## Cara Kompilasi

Butuh `gcc` (C99) dan `make` (GNU make).

- Linux / WSL:
  ```
  make            # kompilasi ke bin/game, tanpa warning (-Wall -Wextra)
  make test       # jalankan seluruh pengujian otomatis
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

1. **Game Baru** → masukkan nama, lalu **seed dunia** → dapat HP 100, ATK 10,
   DEF 3, 200 gold, 2 potion.
2. **Battle** (hub menu 1) → monster acak sesuai level, 12 jenis dari Slime
   sampai Ancient Wyrm (level minimum 1 sampai 10); serang / bertahan /
   potion / kabur. Menang: EXP + gold + skor. Kalah: gold setengah, HP pulih.
3. **Gacha** (hub menu 2) → 100 gold per tarikan, 900 gold per 10x. Katalog 36
   item, 18 senjata dan 18 armor dengan nama Inggris: 12 Common, 12 Rare,
   8 Epic, 4 Legendary. Rarity: Common/Rare/Epic/Legendary; pity dijamin Epic
   setelah 10 tarikan tanpa Epic+. Item duplikat otomatis jadi gold.
4. **Inventory** (hub menu 3) → lihat, urutkan (rarity/power/nama), cari, equip.
5. **Shop** (hub menu 4) → beli potion (40 gold, maks 20), jual item.
6. **Dungeon** (hub menu 5) → dunia prosedural tak berujung dari seed, gerak
   WASD, `$` = treasure (+30 gold), H = hint BFS ke treasure terdekat
   (30 gold, maks 3x), Q = keluar. Rincian di bagian berikut.
7. **Simpan** (hub menu 7) atau Kembali ke menu utama (dipaksa menawarkan simpan,
   lalu skor dicatat ke leaderboard).
8. Menu utama **Lanjutkan** memuat data/save.txt; **Leaderboard** menampilkan top 5.

## Dungeon Prosedural dan Seed

Peta dungeon tidak lagi dibaca dari file. Setiap sel dunia dihitung dari seed
pemain, jadi dunia tidak terbatas ke segala arah dan tidak butuh memori untuk
menyimpan peta.

- **Isi sel.** Koridor terbuka pada `y = 1 (mod 4)` dan poros vertikal pada
  `x = 1 (mod 6)`. Sel lantai lain kosong, dan sekitar satu dari 89 sel lantai
  berisi treasure. Semua keputusan itu berasal dari `cellHash(seed, x, y)`,
  sebuah hash integer murni tanpa `rand()`.
- **Seed sama, peta sama.** Seed yang diminta saat game baru disimpan di
  `data/save.txt`, jadi **Lanjutkan** mengembalikan dunia yang sama persis.
  Seed juga tampil di menu Status dan di baris info dungeon.
- **Seed kosong.** Kalau prompt seed dijawab Enter, program memakai seed dari
  jam sistem dan menampilkannya supaya bisa dicatat dan dipakai ulang.
- **Cache 5x5 chunk.** Dungeon hanya menghitung area 80x80 sel di sekitar
  pemain, lalu menggeser cache ketika pemain pindah chunk. Kamera menampilkan
  jendela 25x15 dengan pemain selalu di tengah.
- **DFS.** Saat masuk, DFS memeriksa apakah ujung koridor area awal bisa
  dicapai dari titik start. Hasilnya muncul sebagai pesan status.
- **BFS.** Tombol H menandai jalur terpendek ke treasure terdekat di dalam
  cache. Tanda `*` hanya ada di tampilan dan hilang setelah langkah berikutnya.
- **Keluar.** Q keluar tanpa bonus exit. Treasure yang sudah diambil tetap
  dihitung ke gold, dan dunia yang sama tidak lagi punya treasure di posisi itu
  selama run berlangsung.

## Daftar Modul

| File | Isi |
| --- | --- |
| main.c | Menu utama dan loop hub game |
| config.h | Konstanta dan struct Item, Monster, Player, Point, DungeonState |
| util.c/.h | readInt, readString, readLine, clearScreen, printLine, shutdownGame |
| player.c/.h | initPlayer, addExp, totalAtk/totalDef, showStatus |
| battle.c/.h | startBattle, calculateDamage |
| gacha.c/.h | gachaMenu, rollRarity, findItemInInventory, pullOnce |
| inventory.c/.h | inventoryMenu, sortInventory, searchItemByName, equipItem |
| shop.c/.h | shopMenu (beli potion, jual item) |
| storage.c/.h | loadCatalog, loadMonsters, saveGame, loadGame, recordScore, showLeaderboard |
| dungeon.c/.h | cellHash, dungeonCell, promptWorldSeed, dfsReachable, bfsShortestPath, runDungeon |
| data/ | item.txt, monster.txt, save.txt, leaderboard.txt |
| tests/ | Driver uji C + skenario input pipa + run_tests.sh |

Nama fungsi dan komentar di kode memakai bahasa Inggris. Teks yang dilihat
pemain tetap bahasa Indonesia.

## Format data/save.txt

Baris pertama berisi 15 field dipisah `|`:

```
nama|hp|maxHp|level|exp|gold|atk|def|potion|pity|totalPull|skor|seed|idSenjata|idArmor
```

Baris kedua adalah jumlah item, lalu satu ID item per baris. Field `seed`
bertipe unsigned dan menyimpan seed dunia dungeon. Save lama tanpa field seed
ditolak dengan pesan "format data/save.txt rusak", jadi mulai game baru sekali
setelah pembaruan ini.

## Pembagian Tugas (anggota A-E, sesuai PRD bagian 13)

| Anggota | Peran | Modul |
| --- | --- | --- |
| A | Arsitek dungeon | config.h, dungeon.c/.h |
| B | Integrator | main.c, battle.c/.h, storage.c/.h |
| C | Gacha | gacha.c/.h |
| D | Inventory | inventory.c/.h |
| E | Shop dan status | shop.c/.h, player.c/.h, util.c/.h |

## Pengujian

`make test` menjalankan `tests/run_tests.sh` dengan 19 pemeriksaan:
- Driver C (logika murni): readInt/readString, level & EXP, damage,
  rarity/pity/duplikat, sort & search inventory, katalog/monster/save-load
  round-trip termasuk seed, leaderboard, serta determinisme sel dungeon,
  BFS, dan DFS.
- Skenario menu: input pipa dari file ke program utama, hasil dicek grep.
  S16 memastikan seed 777 dipakai lagi setelah simpan dan Lanjutkan.
- `make debug` mengulang seluruh pengujian dengan AddressSanitizer + UBSan.

## Demo alur (new game -> battle -> gacha -> equip -> dungeon -> save -> load -> leaderboard)

Tiga run input pipa yang sudah disiapkan di `tests/`:

```
bin/game < tests/in_demoA.txt   # game baru Demo seed 20240601: gacha, equip, dungeon, simpan
bin/game < tests/in_demoB.txt   # lanjutkan, status, skor dicatat, leaderboard
bin/game < tests/in_demoC.txt   # lanjutkan, battle beberapa kali (EOF menghentikan dengan rapi)
```

Jalankan berurutan karena demoB dan demoC memakai save dari demoA.
