# DECISIONS.md

Keputusan untuk hal yang tidak dispesifikasi PRD, diambil sederhana dan konsisten:

1. **EOF pada prompt.** `bacaAngka` yang kehabisan input (EOF/Ctrl+D) memakai
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
7. **Peta dungeon.** Peta harus punya S dan E; baris lebih lebar dari 40 kolom
   ditolak; lebar peta mengikuti baris terakhir; sel kosong di luar peta
   dipadatkan '#' supaya gerak selalu dalam batas.
8. **Keluar dungeon.** Tombol Q keluar tanpa hadiah; batas langkah terlampaui
   juga keluar tanpa hadiah exit, treasure yang sudah diambil tetap dihitung
   (tidak dikembalikan).
9. **Hint BFS.** Jalur tanda '*' hanya tampil pada tampilan saat itu dan hilang
   setelah langkah berikutnya; peta asli tidak berubah.
10. **Simpan ID terpasang.** data/save.txt menyimpan idSenjata/idArmor sebagai
    ID item (bukan indeks), lalu dimetakan kembali ke indeks inventory saat
    muat, sesuai bagian 9 PRD.
11. **Graceful shutdown.** Keluar dari program hanya lewat
    satu pintu (`shutdownGame`): pesan perpisahan + flush output. Ctrl+C di
    Windows tetap mematikan proses karena signal.h dilarang batasan library;
    sebagai gantinya semua jalur EOF sudah berakhir rapi tanpa loop abadi.
12. **catatSkor saat penuh.** Jika leaderboard sudah 10 entri, entri terbaru
    menggantikan skor terendah; file selalu diurut menurun dan dipotong 10.
13. **Jual item terpasang.** Ditolak dengan pesan; sinkronisasi
    idxSenjata/idxArmor setelah sort/jual memakai ID item (fungsi
    `sinkronkanEquip`).
14. **make debug di Windows.** Karena runtime sanitizer gcc MSYS2 (libasan/
    libubsan) tidak tersedia, target debug memakai clang64 (clang.exe) yang
    punya libclang_rt.asan/ubsan; di Linux tetap gcc.
