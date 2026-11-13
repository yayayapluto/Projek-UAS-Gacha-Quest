# DECISIONS.md

Keputusan untuk hal yang tidak dispesifikasi PRD, diambil sederhana dan konsisten:

1. **EOF pada prompt.** `readInt` yang kehabisan input (EOF/Ctrl+D) memakai
   nilai `min` dan program bergerak ke jalur keluar yang aman (menu bernilai 0
   keluar; battle berhenti lewat cek `feof(stdin)`); tidak ada loop abadi.
2. **Nama kosong.** Ditolak dan diminta ulang; nama lebih panjang dari 29
   karakter dipotong (skenario 1: "ditolak atau dipotong").
3. **Beli potion.** Satu transaksi bisa membeli beberapa potion sekaligus;
   jumlah diminta dan divalidasi terhadap sisa stok (maks 20) dan gold.
4. **Potion saat HP penuh.** Tetap boleh dipakai (heal di-cap di maxHp); PRD
   tidak melarangnya. Potion 0 ditolak tanpa memakai giliran.
5. **Pity pada paket 10x.** Pity diproses per tarikan secara berurutan; tarikan
   yang dijamin pity menghasilkan Epic (bukan Legendary) kecuali roll acak
   memberi Legendary.
6. **EXP di level 10.** EXP direset ke 0 dan tidak lagi dihitung (maks level 10).
7. **Dungeon prosedural, bukan peta dari file.** PRD bagian 7 meminta peta
   `dungeon1-3.txt`, tetapi permintaan terbaru adalah dungeon dengan seed
   seperti Minecraft. Dunia sekarang dihitung per sel dari
   `cellHash(seed, x, y)`: koridor pada `y = 1 (mod 4)`, poros pada
   `x = 1 (mod 6)`, treasure kira-kira 1 dari 89 sel lantai. File peta
   dihapus, `muatPeta` dan konstanta `MAX_BARIS`/`MAX_KOLOM` ikut dibuang.
8. **Seed dunia.** Seed diminta sekali saat game baru, disimpan di
   `data/save.txt`, dan dipakai ulang setelah Lanjutkan. Prompt yang dijawab
   Enter memakai seed dari jam sistem lalu menampilkannya. Input selain angka
   0 sampai 4294967295 ditolak dan diminta ulang. Seed 0 sah dipakai.
9. **Keluar dungeon.** Q keluar tanpa bonus exit karena dungeon tidak berujung
   dan tidak punya pintu keluar. Treasure yang sudah diambil tetap dihitung
   (tidak dikembalikan).
10. **Hint BFS.** BFS mencari treasure terdekat di dalam cache, bukan exit.
    Jalur tanda `*` hanya tampil pada tampilan saat itu dan hilang setelah
    langkah berikutnya; peta asli tidak berubah. Biaya 30 gold, maks 3x.
11. **Cache area dan kamera.** Dungeon menghitung area 5x5 chunk (80x80 sel)
    di sekitar pemain dan menggesernya saat pemain pindah chunk. Kamera
    menampilkan jendela 25x15 dengan pemain di tengah. Ukuran tetap ini menjaga
    pemakaian memori statis tanpa malloc.
12. **Batas treasure per run.** Maksimal 64 posisi treasure disimpan per
    kunjungan (`MAX_TAKEN_TREASURES`). Lewat dari itu treasure tetap memberi
    gold, hanya tidak lagi dicatat sehingga bisa muncul kembali setelah cache
    dibangun ulang.
13. **Simpan ID terpasang.** data/save.txt menyimpan `weaponIndex`/`armorIndex`
    sebagai ID item (bukan indeks), lalu dimetakan kembali ke indeks inventory
    saat muat, sesuai bagian 9 PRD.
14. **Graceful shutdown.** Keluar dari program hanya lewat
    satu pintu (`shutdownGame`): pesan perpisahan + flush output. Ctrl+C di
    Windows tetap mematikan proses karena signal.h dilarang batasan library;
    sebagai gantinya semua jalur EOF sudah berakhir rapi tanpa loop abadi.
15. **`recordScore` saat penuh.** Jika leaderboard sudah 10 entri, entri terbaru
    menggantikan skor terendah; file selalu diurut menurun dan dipotong 10.
16. **Jual item terpasang.** Ditolak dengan pesan; sinkronisasi
    `weaponIndex`/`armorIndex` setelah sort/jual memakai ID item (fungsi
    `syncEquipped`).
17. **Format save bertambah satu field.** Field `seed` disisipkan sebelum
    `idSenjata`, sehingga baris pertama berisi 15 field. Save lama 14 field
    ditolak dengan pesan "format data/save.txt rusak" dan pemain perlu memulai
    game baru sekali. Membaca dua format sekaligus dianggap tidak sepadan
    karena save hanya berisi progres sesi, bukan data yang dibagikan.
18. **make debug di Windows.** Karena runtime sanitizer gcc MSYS2 (libasan/
    libubsan) tidak tersedia, target debug memakai clang64 (clang.exe) yang
    punya libclang_rt.asan/ubsan; di Linux tetap gcc.
