#!/bin/sh
# Menjalankan semua pengujian Gacha Quest: driver C plus skenario menu bagian 15 PRD.
# Pemakaian: sh tests/run_tests.sh [opsi-sanitizer-tambahan]
set -u

CC="${CC_BIN:-gcc}"
case "$CC" in
*clang64*)
    CLANGDIR=$(dirname "$CC")
    POSIXDIR=$(cygpath -u "$CLANGDIR" 2>/dev/null || echo "$CLANGDIR")
    PATH="$POSIXDIR:$PATH"
    export PATH
    ;;
esac
CFLAGS="-std=c99 -Wall -Wextra"
INC="-Isrc/core -Isrc/util -Isrc/player -Isrc/battle -Isrc/gacha -Isrc/inventory -Isrc/shop -Isrc/storage -Isrc/dungeon"
CFLAGS="$CFLAGS $INC"
SAN="${1:-}"
LIB="src/player/player.c src/battle/battle.c src/gacha/gacha.c src/inventory/inventory.c src/shop/shop.c src/storage/storage.c src/dungeon/dungeon.c src/util/util.c"

PASS=0
FAIL=0

lulus() { PASS=$((PASS + 1)); echo "LULUS  $1"; }
gagal() { FAIL=$((FAIL + 1)); echo "GAGAL  $1"; }

skenario() {
    nama="$1"
    input="$2"
    pola="$3"
    out_file="tests/out_last.txt"
    ./bin/game < "$input" > "$out_file" 2>&1
    status=$?
    if [ $status -eq 0 ] && grep -q -- "$pola" "$out_file"; then
        lulus "$nama"
    else
        gagal "$nama"
    fi
}

driver() {
    nama="$1"
    exe="$2"
    input="${3:-}"
    if [ -n "$input" ]; then
        if "./$exe" < "$input" > tests/out_last.txt 2>&1; then
            lulus "$nama"
        else
            gagal "$nama"
        fi
    else
        if "./$exe" > tests/out_last.txt 2>&1; then
            lulus "$nama"
        else
            gagal "$nama"
        fi
    fi
}

echo "=== Membangun driver uji ==="
mkdir -p bin
$CC $CFLAGS $SAN -o bin/t_util tests/t_util.c $LIB
$CC $CFLAGS $SAN -o bin/t_player tests/t_player.c $LIB
$CC $CFLAGS $SAN -o bin/t_battle tests/t_battle.c $LIB
$CC $CFLAGS $SAN -o bin/t_gacha tests/t_gacha.c $LIB
$CC $CFLAGS $SAN -o bin/t_inventory tests/t_inventory.c $LIB
$CC $CFLAGS $SAN -o bin/t_storage tests/t_storage.c $LIB
$CC $CFLAGS $SAN -o bin/t_leaderboard tests/t_leaderboard.c $LIB
$CC $CFLAGS $SAN -o bin/t_dungeon tests/t_dungeon.c $LIB
$CC $CFLAGS $SAN -o bin/game main.c $LIB

echo "=== Pengujian otomatis ==="
rm -f data/leaderboard.txt

driver "S08 urut & cari inventory" bin/t_inventory
driver "S06/S07 rarity, pity, duplikat" bin/t_gacha
driver "S12/S14 dungeon prosedur: seed deterministik, DFS & BFS" bin/t_dungeon

driver "S10 save-load round trip" bin/t_storage
driver "S15 leaderboard urut & top 10" bin/t_leaderboard

driver "S01/S02 bacaAngka & bacaString" bin/t_util tests/in_util.txt

skenario "S01 nama kosong/terlalu panjang" tests/in_s01.txt "Hub | AAAAAAAAAAAAAAAAAAAAAAAAAAAAA |"
skenario "S02 input huruf pada menu" tests/in_s02.txt "Input tidak valid"
skenario "S03 battle sampai menang" tests/in_s03.txt "Hadiah:"
skenario "S04 battle sampai kalah" tests/in_s04.txt "Kamu dikalahkan"
skenario "S05 gacha 10x gold kurang" tests/in_s05.txt "Gold kurang! Tarikan 10x butuh 900"
skenario "S09 jual item terpasang" tests/in_s09.txt "tidak bisa dijual"

cp -f data/leaderboard.txt data/leaderboard.uji.bak
skenario "S10 simpan-lanjutkan" tests/in_s10.txt "Selamat datang kembali, UjiSv!"
skenario "S15 leaderboard tercatat" tests/in_s15.txt "Skor UjiSkr"
mv -f data/leaderboard.uji.bak data/leaderboard.txt

if [ -f data/save.txt ]; then cp -f data/save.txt data/save.uji.bak; fi
printf "xxx|1|1|1|1|1|1|1|1|1|1|1|1|1|1\n" > data/save.txt
skenario "S11 save.txt dirusak" tests/in_s11.txt "Gagal memuat"
if [ -f data/save.uji.bak ]; then mv -f data/save.uji.bak data/save.txt; else rm -f data/save.txt; fi
skenario "S12 gerak ke tembok" tests/in_s12.txt "Tembok! Tidak bisa lewat."
skenario "S13 hint gold kurang" tests/in_s13.txt "Gold kurang, hint butuh 30 gold"

skenario "S16 seed dunia diminta saat game baru" tests/in_s16.txt "dibangun dari seed 777"
hits=$(grep -c "dibangun dari seed 777" tests/out_last.txt)
if [ "$hits" -eq 2 ]; then
    lulus "S16 seed yang sama dipakai lagi setelah Lanjutkan"
else
    gagal "S16 seed yang sama dipakai lagi setelah Lanjutkan (ditemukan $hits kali)"
fi

echo ""
echo "=== Ringkasan: $PASS lulus, $FAIL gagal ==="
[ $FAIL -eq 0 ]
