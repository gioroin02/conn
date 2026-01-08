#ifndef CONN_GAME_C
#define CONN_GAME_C

#include "game.h"

static ssize connBoardCountConsecutive(ConnBoard* self, ssize column, ssize row, ssize dx, ssize dy, u16* value)
{
    ssize result = 0;
    u16   pivot  = connBoardGet(self, column, row);
    u16   other  = pivot;

    if (dx == 0 && dy == 0) return result;

    for (result = 0; pivot == other; result += 1) {
        column += dx;
        row    += dy;

        other = connBoardGet(self, column, row);
    }

    if (value != 0) *value = pivot;

    return result;
}

ConnPlayer connPlayerMake(ConnClientFlag flag, u16 client)
{
    ConnPlayer result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.flag   = flag;
    result.client = client;

    return result;
}

b32 connBoardCreate(ConnBoard* self, PxMemoryArena* arena, ssize width, ssize height)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    if (width <= 0 || height <= 0 || width > PX_SSIZE_MAX / height)
        return 0;

    self->values = pxMemoryArenaReserveManyOf(arena, u16, width * height);

    if (self->values == PX_NULL) return 0;

    self->width  = width;
    self->height = height;

    connBoardClear(self);

    return 1;
}

void connBoardClear(ConnBoard* self)
{
    ssize index = 0;
    ssize size  = self->width * self->height;

    for (index = 0; index < size; index += 1)
        self->values[index] = 0;
}

ssize connBoardCount(ConnBoard* self)
{
    ssize result = 0;
    ssize index  = 0;
    ssize size   = self->width * self->height;

    for (index = 0; index < size; index += 1) {
        if (self->values[index] != 0) result += 1;
    }

    return result;
}

ssize connBoardSize(ConnBoard* self)
{
    return self->width * self->height;
}

b32 connBoardIsEmpty(ConnBoard* self)
{
    return connBoardCount(self) == 0 ? 1 : 0;
}

b32 connBoardIsFull(ConnBoard* self)
{
    return connBoardCount(self) == connBoardSize(self) ? 1 : 0;
}

b32 connBoardHeight(ConnBoard* self, ssize column, ssize* height)
{
    ssize index = 0;

    if (column < 0 || column >= self->width) return 0;

    for (index = self->height; index > 0; index -= 1) {
        ssize row = index - 1;
        u16   val = self->values[row * self->width + column];

        if (val != 0) continue;

        if (height != 0) *height = row;

        return 1;
    }

    return 0;
}

b32 connBoardInsert(ConnBoard* self, ssize column, u16 value)
{
    ssize row = 0;

    if (column < 0 || column >= self->width || value == 0)
        return 0;

    if (connBoardHeight(self, column, &row) == 0) return 0;

    self->values[row * self->width + column] = value;

    return 1;
}

u16 connBoardGet(ConnBoard* self, ssize column, ssize row)
{
    if (column < 0 || column >= self->width)  return 0;
    if (row    < 0 || row    >= self->height) return 0;

    return self->values[row * self->width + column];
}

b32 connBoardIsWinner(ConnBoard* self, ssize column, u16 value)
{
    #define DIRS ((ssize) 8)

    static ssize dirsCos[DIRS] = {+1, +1,  0, -1, -1, -1,  0, +1};
    static ssize dirsSin[DIRS] = { 0, +1, +1, +1,  0, -1, -1, -1};

    if (column < 0 || column >= self->width || value == 0) return 0;

    ssize row = 0;

    if (connBoardHeight(self, column, &row) == 0) return 0;

    ssize index = 0;

    for (index = 0; index < DIRS; index += 1) {
        ssize other = (index + DIRS / 2) % DIRS;

        ssize forw = connBoardCountConsecutive(self,
            column, row + 1, dirsCos[index], dirsSin[index], 0);

        ssize back = connBoardCountConsecutive(self,
            column, row + 1, dirsCos[other], dirsSin[other], 0);

        if (forw + back > 4) return 1;
    }

    return 0;
}

#endif // CONN_GAME_C
