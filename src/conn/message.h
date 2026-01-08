#ifndef CONN_MESSAGE_H
#define CONN_MESSAGE_H

#include "memory.h"
#include "game.h"

#define CONN_MESSAGE_SIZE_HEADER ((ssize) 12)
#define CONN_MESSAGE_SIZE        ((ssize) 18)

typedef enum ConnMessageKind
{
    ConnMessage_None   = 0,
    ConnMessage_Accept = 1,
    ConnMessage_Reject = 2,
    ConnMessage_Join   = 3,
    ConnMessage_Quit   = 4,
    ConnMessage_Data   = 5,
    ConnMessage_Turn   = 6,
    ConnMessage_Move   = 7,
    ConnMessage_Result = 8,
}
ConnMessageKind;

typedef struct ConnMessageJoin
{
    ConnClientFlag flag;
}
ConnMessageJoin;

typedef struct ConnMessageQuit
{
    u16 client;
}
ConnMessageQuit;

typedef struct ConnMessageData
{
    ConnClientFlag flag;
    u16            client;
}
ConnMessageData;

typedef struct ConnMessageTurn
{
    u16 client;
}
ConnMessageTurn;

typedef struct ConnMessageMove
{
    u16 client;
    u16 column;
}
ConnMessageMove;

typedef struct ConnMessageResult
{
    u16 client;
}
ConnMessageResult;

typedef struct ConnMessage
{
    ConnMessageKind kind;

    u32 version;
    u32 length;

    union
    {
        ConnMessageJoin   join;
        ConnMessageQuit   quit;
        ConnMessageData   data;
        ConnMessageTurn   turn;
        ConnMessageMove   move;
        ConnMessageResult result;
    };
}
ConnMessage;

ConnMessage connMessageJoin(ConnClientFlag flag);

ConnMessage connMessageQuit(u16 client);

ConnMessage connMessageData(ConnClientFlag flag, u16 client);

ConnMessage connMessageTurn(u16 client);

ConnMessage connMessageMove(u16 client, u16 column);

ConnMessage connMessageResult(u16 client);

ConnMessage connMessageDecode(u8* pntr, ssize size);

ssize connMessageEncode(ConnMessage message, u8* pntr, ssize size);

ssize connMessageToString(ConnMessage message, u8* pntr, ssize size);

#endif // CONN_MESSAGE_H
