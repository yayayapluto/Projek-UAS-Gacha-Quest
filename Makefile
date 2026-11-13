CC = gcc
CFLAGS = -std=c99 -Wall -Wextra
INC = -Isrc/core -Isrc/util -Isrc/player -Isrc/battle -Isrc/gacha -Isrc/inventory -Isrc/shop -Isrc/storage -Isrc/dungeon
CFLAGS += $(INC)
MODUL = main.c \
        src/util/util.c src/player/player.c src/battle/battle.c \
        src/gacha/gacha.c src/inventory/inventory.c src/shop/shop.c \
        src/storage/storage.c src/dungeon/dungeon.c
BIN = bin

ifeq ($(OS),Windows_NT)
CC_SAN = C:/msys64/clang64/bin/clang.exe
else
CC_SAN = $(CC)
endif

.PHONY: all clean test debug

all: $(BIN)/game

$(BIN)/game: $(MODUL)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/game $(MODUL)

debug:
	@mkdir -p $(BIN)
	$(CC_SAN) $(CFLAGS) -fsanitize=address,undefined -g -o $(BIN)/game $(MODUL)
	CC_BIN="$(CC_SAN)" sh tests/run_tests.sh "-fsanitize=address,undefined -g"

test: all
	sh tests/run_tests.sh

clean:
	rm -rf $(BIN)
