#include <stdio.h>
#include "config.h"
#include "dungeon.h"

#define CHECK(cond) do { if (!(cond)) { printf("FAIL: %s line %d\n", #cond, __LINE__); return 1; } } while (0)

int main(void)
{
    char area[AREA_MAX][AREA_MAX];
    char again[AREA_MAX][AREA_MAX];
    Point path[MAX_PATH_CELLS];
    unsigned int seed = 20240601u;
    unsigned int other = 20240602u;
    int r, c, len, treasures = 0, differences = 0;

    /* The generator is pure: repeated calls with the same arguments agree. */
    CHECK(dungeonCell(seed, 5, -3) == dungeonCell(seed, 5, -3));
    CHECK(cellHash(seed, 7, 2) == cellHash(seed, 7, 2));

    /* Corridor pattern: x = 1 (mod 6) and y = 1 (mod 4) are walkable, everything else is wall. */
    CHECK(dungeonCell(seed, 1, 0) != '#');
    CHECK(dungeonCell(seed, 0, 0) == '#');
    CHECK(dungeonCell(seed, -5, -3) != '#');
    CHECK(dungeonCell(seed, 1, 1) == '.');

    for (r = 0; r < AREA_MAX; r++)
        for (c = 0; c < AREA_MAX; c++)
            area[r][c] = dungeonCell(seed, c, r);
    for (r = 0; r < AREA_MAX; r++)
        for (c = 0; c < AREA_MAX; c++)
            again[r][c] = dungeonCell(seed, c, r);

    /* Same seed reproduces the world cell by cell. */
    for (r = 0; r < AREA_MAX; r++)
        for (c = 0; c < AREA_MAX; c++)
            CHECK(area[r][c] == again[r][c]);

    /* A different seed produces a different world. */
    for (r = 0; r < AREA_MAX; r++)
        for (c = 0; c < AREA_MAX; c++)
            if (area[r][c] != dungeonCell(other, c, r)) differences++;
    CHECK(differences > 0);

    for (r = 0; r < AREA_MAX; r++)
        for (c = 0; c < AREA_MAX; c++)
            if (area[r][c] == 'T') treasures++;
    CHECK(treasures > 0);

    /* DFS: the starting area is connected from the start cell to the far corridor end. */
    CHECK(dfsReachable(area, (Point){1, 1}, (Point){AREA_MAX - 3, AREA_MAX - 3}) == 1);

    /* BFS: the path starts and ends exactly on the endpoints and never crosses a wall. */
    len = bfsShortestPath(area, (Point){1, 1}, (Point){AREA_MAX - 3, AREA_MAX - 3}, path);
    CHECK(len > 0);
    CHECK(path[0].row == 1 && path[0].col == 1);
    CHECK(path[len - 1].row == AREA_MAX - 3 && path[len - 1].col == AREA_MAX - 3);
    CHECK(area[path[len / 2].row][path[len / 2].col] != '#');

    /* Walls are unreachable. */
    CHECK(dfsReachable(area, (Point){1, 1}, (Point){0, 0}) == 0);
    CHECK(bfsShortestPath(area, (Point){1, 1}, (Point){0, 0}, path) == -1);

    printf("All dungeon tests passed.\n");
    return 0;
}
