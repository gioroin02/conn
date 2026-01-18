#ifndef CONN_GAME_H
#define CONN_GAME_H

#include "import.h"

typedef struct ConnBoard
{
    U16* values;
    Int  width;
    Int  height;
}
ConnBoard;

typedef enum ConnClientFlag
{
    ConnClient_None     = 0,
    ConnClient_Player   = 1 << 0,
    ConnClient_Computer = 1 << 1,
}
ConnClientFlag;

typedef struct ConnPlayer
{
    ConnClientFlag flag;
    U16            client;
}
ConnPlayer;

ConnPlayer connPlayerMake(ConnClientFlag flag, U16 client);

Bool connBoardCreate(ConnBoard* self, PMemoryArena* arena, Int width, Int height);

void connBoardClear(ConnBoard* self);

Int connBoardCount(ConnBoard* self);

Int connBoardSize(ConnBoard* self);

Bool connBoardIsEmpty(ConnBoard* self);

Bool connBoardIsFull(ConnBoard* self);

Bool connBoardHeight(ConnBoard* self, Int column, Int* height);

Bool connBoardInsert(ConnBoard* self, Int column, U16 value);

U16 connBoardGet(ConnBoard* self, Int column, Int row);

Bool connBoardIsWinner(ConnBoard* self, Int column, U16 value);

#endif // CONN_GAME_H
