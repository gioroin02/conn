#ifndef CONN_GAME_C
#define CONN_GAME_C

#include "game.h"

static Int connBoardCountConsecutive(ConnBoard* self, Int column, Int row, Int dx, Int dy, U16* value)
{
    Int result = 0;
    U16 pivot  = connBoardGet(self, column, row);
    U16 other  = pivot;

    if (dx == 0 && dy == 0) return result;

    for (result = 0; pivot == other; result += 1) {
        column += dx;
        row    += dy;

        other = connBoardGet(self, column, row);
    }

    if (value != 0) *value = pivot;

    return result;
}

ConnPlayer connPlayerMake(ConnClientFlag flag, U16 client)
{
    ConnPlayer result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.flag   = flag;
    result.client = client;

    return result;
}

Bool connBoardCreate(ConnBoard* self, PMemoryArena* arena, Int width, Int height)
{
    pMemorySet(self, sizeof *self, 0xAB);

    if (width <= 0 || height <= 0 || width > P_INT_MAX / height)
        return 0;

    self->values = pMemoryArenaReserveManyOf(arena, U16, width * height);

    if (self->values == NULL) return 0;

    self->width  = width;
    self->height = height;

    connBoardClear(self);

    return 1;
}

void connBoardClear(ConnBoard* self)
{
    Int index = 0;
    Int size  = self->width * self->height;

    for (index = 0; index < size; index += 1)
        self->values[index] = 0;
}

Int connBoardCount(ConnBoard* self)
{
    Int result = 0;
    Int index  = 0;
    Int size   = self->width * self->height;

    for (index = 0; index < size; index += 1) {
        if (self->values[index] != 0)
            result += 1;
    }

    return result;
}

Int connBoardSize(ConnBoard* self)
{
    return self->width * self->height;
}

Bool connBoardIsEmpty(ConnBoard* self)
{
    return connBoardCount(self) == 0 ? 1 : 0;
}

Bool connBoardIsFull(ConnBoard* self)
{
    return connBoardCount(self) == connBoardSize(self) ? 1 : 0;
}

Bool connBoardHeight(ConnBoard* self, Int column, Int* height)
{
    Int index = 0;

    if (column < 0 || column >= self->width) return 0;

    for (index = self->height; index > 0; index -= 1) {
        Int row = index - 1;
        U16   val = self->values[row * self->width + column];

        if (val != 0) continue;

        if (height != 0) *height = row;

        return 1;
    }

    return 0;
}

Bool connBoardInsert(ConnBoard* self, Int column, U16 value)
{
    Int row = 0;

    if (column < 0 || column >= self->width || value == 0)
        return 0;

    if (connBoardHeight(self, column, &row) == 0) return 0;

    self->values[row * self->width + column] = value;

    return 1;
}

U16 connBoardGet(ConnBoard* self, Int column, Int row)
{
    if (column < 0 || column >= self->width)  return 0;
    if (row    < 0 || row    >= self->height) return 0;

    return self->values[row * self->width + column];
}

Bool connBoardIsWinner(ConnBoard* self, Int column, U16 value)
{
    #define CIRC      ((Int) 8)
    #define CIRC_HALF ((Int) 4)

    static Int dirsCos[CIRC] = {+1, +1,  0, -1, -1, -1,  0, +1};
    static Int dirsSin[CIRC] = { 0, +1, +1, +1,  0, -1, -1, -1};

    if (column < 0 || column >= self->width || value == 0) return 0;

    Int row = 0;

    if (connBoardHeight(self, column, &row) == 0) return 0;

    Int index = 0;

    for (index = 0; index < CIRC; index += 1) {
        Int other = (index + CIRC_HALF) % CIRC;

        Int forw = connBoardCountConsecutive(self,
            column, row + 1, dirsCos[index], dirsSin[index], 0);

        Int back = connBoardCountConsecutive(self,
            column, row + 1, dirsCos[other], dirsSin[other], 0);

        if (forw + back > 4) return 1;
    }

    return 0;
}

#endif // CONN_GAME_C
