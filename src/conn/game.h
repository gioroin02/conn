#ifndef CONN_GAME_H
#define CONN_GAME_H

#include "./import.h"

typedef struct ConnBoard
{
    u32*  values;
    ssize width;
    ssize height;
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
    u32            client;
}
ConnPlayer;

ConnPlayer
connPlayerMake(ConnClientFlag flag, u32 client);

ssize
connBoardCreate(ConnBoard* self, PxMemoryArena* arena, ssize width, ssize height);

void
connBoardClear(ConnBoard* self);

ssize
connBoardCount(ConnBoard* self);

b32
connBoardIsEmpty(ConnBoard* self);

b32
connBoardIsFull(ConnBoard* self);

b32
connBoardHeight(ConnBoard* self, ssize column, ssize* height);

b32
connBoardInsert(ConnBoard* self, ssize column, u32 value);

u32
connBoardGet(ConnBoard* self, ssize column, ssize row);

b32
connBoardIsWinner(ConnBoard* self, ssize column, u32 value);

#endif // CONN_GAME_H
