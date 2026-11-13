# Penjelasan BFS dan DFS di Dungeon Explorer

## Gambaran umum

Peta dungeon adalah kotak berisi huruf: `#` tembok, `.` jalan, `S` start,
`T` treasure, `E` exit. Semua pemrosesan memakai array 2D berukuran tetap
(20 x 40) tanpa rekursi.

## BFS (jalur terpendek, untuk Hint)

BFS (Breadth-First Search) menjelajah peta per lapisan: dulu semua sel yang
jaraknya 1 langkah, lalu yang jaraknya 2 langkah, dst. Karena urutannya per
lapisan, pertama kali sel `E` ditemukan pasti lewat jalur terpendek.

Struktur data yang dipakai (dalam `dungeon.c`):

- `queue` — array berisi `Titik` (baris, kolom), dengan dua penunjuk
  `front` (depan) dan `rear` (belakang). Tidak ada malloc.
- `dikunjungi[baris][kolom]` — tanda sel yang pernah dimasukkan antrian supaya
  tidak diproses dua kali.
- `asal[baris][kolom]` — menyimpan sel asal setiap sel, untuk merekonstruksi
  jalur.

Langkah BFS:

1. Masukkan sel awal ke antrian, tandai dikunjungi.
2. Selama `front < rear`: ambil sel paling depan (`front++`).
3. Cek 4 tetangga (atas, bawah, kiri, kanan). Tetangga yang bukan `#`, dalam
   batas peta, dan belum dikunjungi: tandai, catat `asal`, masukkan ke belakang
   antrian (`rear++`).
4. Jika sel yang ditemukan adalah `E`, kembali mundur lewat `asal` sampai sel
   awal; hasilnya dibalik menjadi jalur dari awal ke `E`.

Dipakai di dua tempat:

- **Hint (tombol H):** jalur dari posisi pemain sekarang ke `E` digambar
  dengan `*` pada tampilan saja. Biaya 30 gold, maks 3x per kunjungan.
- **Batas langkah:** panjang jalur terpendek dari `S` ke `E` dikali 2 menjadi
  maksimum langkah pemain.

## DFS (validasi peta)

DFS (Depth-First Search) menjelajah sedalam mungkin sebelum mundur. Di program
ini DFS hanya menjawab satu pertanyaan: "apakah `E` bisa dicapai dari `S`?"

Karena rekursi dilarang, DFS memakai `stack` dari array:

1. Tekan sel `S` ke stack, tandai dikunjungi.
2. Selama stack tidak kosong: ambil sel paling atas (`top--`).
3. Jika sel itu adalah `E`, jawab: peta valid.
4. Tekan semua tetangga yang jalan dan belum dikunjungi ke stack.
5. Jika stack kosong dan `E` tidak pernah ditemukan, peta ditolak dan peta itu
   tidak muncul di menu dungeon.

## Bedanya

| Aspek | BFS | DFS |
| --- | --- | --- |
| Tujuan | jalur terpendek | sekadar bisa dicapai atau tidak |
| Struktur data | antrian (queue array, front/rear) | tumpukan (stack array) |
| Urutan jelajah | per lapisan (dekatan dulu) | sedalam mungkin dulu |
| Dipakai untuk | Hint H dan batas langkah | validasi peta saat dimuat |
