#ifndef CONN_GAME_C
#define CONN_GAME_C

#include "./game.h"

static ssize
connBoardCountConsecutive(ConnBoard* self, ssize column, ssize row, ssize dx, ssize dy, u32* value)
{
    if (self == 0 || (dx == 0 && dy == 0)) return 0;

    ssize result = 0;
    u32   pivot  = connBoardGet(self, column, row);
    u32   other  = pivot;

    for (; pivot == other; result += 1) {
        column += dx;
        row    += dy;

        other = connBoardGet(self, column, row);
    }

    if (value != 0) * value = pivot;

    return result;
}

ssize
connBoardCreate(ConnBoard* self, RnMemoryArena* arena, ssize width, ssize height)
{
    if (self == 0) return 0;

    self->values = rnMemoryArenaReserveManyOf(arena, u32, width * height);

    self->width  = width;
    self->height = height;

    return self->width * self->height;
}

void
connBoardClear(ConnBoard* self)
{
    for (ssize i = 0; i < self->width * self->height; i += 1)
        self->values[i] = 0;
}

ssize
connBoardCount(ConnBoard* self)
{
    ssize result = 0;

    for (ssize i = 0; i < self->width * self->height; i += 1) {
        if (self->values[i] != 0)
            result += 1;
    }

    return result;
}

b32
connBoardIsEmpty(ConnBoard* self)
{
    return connBoardCount(self) == 0 ? 1 : 0;
}

b32
connBoardIsFull(ConnBoard* self)
{
    return connBoardCount(self) == self->width * self->height ? 1 : 0;
}

b32
connBoardHeight(ConnBoard* self, ssize column, ssize* height)
{
    if (self == 0 || column < 0 || column >= self->width)
        return 0;

    for (ssize i = self->height; i > 0; i -= 1) {
        if (self->values[(i - 1) * self->width + column] != 0)
            continue;

        if (height != 0) *height = i - 1;

        return 1;
    }

    return 0;
}

b32
connBoardInsert(ConnBoard* self, ssize column, u32 value)
{
    if (self == 0 || column < 0 || column >= self->width || value == 0)
        return 0;

    ssize height = 0;

    if (connBoardHeight(self, column, &height) == 0) return 0;

    self->values[height * self->width + column] = value;

    return 1;
}

u32
connBoardGet(ConnBoard* self, ssize column, ssize row)
{
    if (self == 0) return 0;

    if (column < 0 || column >= self->width)  return 0;
    if (row    < 0 || row    >= self->height) return 0;

    return self->values[row * self->width + column];
}

b32
connBoardIsWinner(ConnBoard* self, ssize column, u32 code)
{
    #define DIRS ((ssize) 8)

    static ssize dirsCos[DIRS] = {+1, +1,  0, -1, -1, -1,  0, +1};
    static ssize dirsSin[DIRS] = { 0, +1, +1, +1,  0, -1, -1, -1};

    if (self == 0 || column < 0 || column >= self->width || code == 0)
        return 0;

    ssize height = 0;

    if (connBoardHeight(self, column, &height) == 0) return 0;

    ssize row = height + 1;

    for (ssize i = 0; i < DIRS; i += 1) {
        ssize j = (i + DIRS / 2) % DIRS;

        ssize forw = connBoardCountConsecutive(self,
            column, row, dirsCos[i], dirsSin[i], 0);

        ssize back = connBoardCountConsecutive(self,
            column, row, dirsCos[j], dirsSin[j], 0);

        if (forw + back > 4) return 1;
    }

    return 0;
}

#endif // CONN_GAME_C
