# Penjelasan BFS dan DFS di Dungeon Explorer

## Gambaran umum

Dungeon sekarang berupa dunia prosedural yang dibangun dari seed. Isi sel
dihitung dengan `dungeonCell(seed, x, y)`: koridor terbuka pada `y = 1 (mod 4)`
dan poros pada `x = 1 (mod 6)`, sedangkan treasure muncul di sekitar satu dari
89 sel lantai. Karena tidak ada peta di memori yang ukurannya tetap, BFS dan DFS
bekerja pada cache area 80 x 80 sel (`AREA_MAX`) yang dihitung ulang setiap
pemain berpindah chunk.

Simbol yang dilihat pemain: `#` tembok, `.` jalan, `$` treasure, `P` pemain,
`*` jalur hint. Titik start dunia selalu `(1,1)`.

## BFS (jalur terpendek, untuk Hint)

BFS (Breadth-First Search) menjelajah area per lapisan: dulu semua sel yang
jaraknya 1 langkah, lalu yang jaraknya 2 langkah, dst. Karena urutannya per
lapisan, sel target pertama yang ditemukan pasti lewat jalur terpendek.

Struktur data yang dipakai (dalam `dungeon.c`):

- `queue`: array berisi `Point` (row, col), dengan dua penunjuk `front` dan
  `rear`. Tidak ada malloc.
- `visited[row][col]`: tanda sel yang pernah dimasukkan antrian supaya tidak
  diproses dua kali.
- `parent[row][col]`: sel asal setiap sel, untuk merekonstruksi jalur.

Langkah BFS:

1. Masukkan sel awal ke antrian, tandai dikunjungi.
2. Selama `front < rear`: ambil sel paling depan (`front++`).
3. Cek 4 tetangga (atas, bawah, kiri, kanan). Tetangga yang bukan `#`, masih di
   dalam area, dan belum dikunjungi: tandai, catat `parent`, masukkan ke
   belakang antrian (`rear++`).
4. Jika sel yang ditemukan berisi `T`, mundur lewat `parent` sampai sel awal,
   lalu balik urutannya menjadi jalur dari awal ke target.

Dipakai di satu tempat:

- **Hint (tombol H):** BFS mencari treasure terdekat dari posisi pemain, bukan
  pintu keluar, karena dungeon tidak punya exit. Jalurnya digambar `*` pada
  tampilan saja. Biaya 30 gold, maks 3x per kunjungan.

Fungsi publiknya `bfsShortestPath`, dipakai juga oleh driver uji untuk mengukur
jalur dari start ke ujung koridor.

## DFS (validasi area awal)

DFS (Depth-First Search) menjelajah sedalam mungkin sebelum mundur. Di program
ini DFS menjawab satu pertanyaan: apakah ujung koridor area awal bisa dicapai
dari titik start?

Karena rekursi dilarang, DFS memakai `stack` dari array:

1. Tekan sel start ke stack, tandai dikunjungi.
2. Selama stack tidak kosong: ambil sel paling atas (`top--`).
3. Jika sel itu sama dengan target, jawab: tersambung.
4. Tekan semua tetangga yang jalan dan belum dikunjungi ke stack.
5. Jika stack kosong dan target tidak pernah ditemukan, area awal dianggap
   tidak tersambung.

Pesan hasilnya muncul di baris status dungeon: "Area awal tersambung dari titik
awal sampai ujung koridor (DFS)" atau "Area awal tidak tersambung". Fungsi ini tidak
menolak peta, karena peta memang tidak dimuat dari file lagi. Ia menjadi
pemeriksaan bahwa pola koridor dari seed tidak memutus area awal.

## Bedanya

| Aspek | BFS | DFS |
| --- | --- | --- |
| Tujuan | jalur terpendek | sekadar bisa dicapai atau tidak |
| Struktur data | antrian (queue array, front/rear) | tumpukan (stack array) |
| Urutan jelajah | per lapisan (dekatan dulu) | sedalam mungkin dulu |
| Dipakai untuk | Hint H dan pengujian jalur | validasi area awal |
