#ifndef CONN_MESSAGE_H
#define CONN_MESSAGE_H

#include "./memory.h"
#include "./game.h"

#define CONN_MESSAGE_SIZE_HEADER ((ssize) 8)
#define CONN_MESSAGE_SIZE        ((ssize) sizeof(ConnMessage))

typedef enum ConnMessageKind
{
    ConnMessage_None   = 0,
    ConnMessage_Join   = 1,
    ConnMessage_Quit   = 2,
    ConnMessage_Data   = 3,
    ConnMessage_Turn   = 4,
    ConnMessage_Move   = 5,
    ConnMessage_Result = 6,
}
ConnMessageKind;

typedef struct ConnMessageJoin
{
    ConnClientFlag flag;
}
ConnMessageJoin;

typedef struct ConnMessageData
{
    ConnClientFlag flag;
    u32            client;
}
ConnMessageData;

typedef struct ConnMessageTurn
{
    u32 client;
}
ConnMessageTurn;

typedef struct ConnMessageMove
{
    u32 client;
    u32 column;
}
ConnMessageMove;

typedef struct ConnMessageResult
{
    u32 client;
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
        ConnMessageData   data;
        ConnMessageTurn   turn;
        ConnMessageMove   move;
        ConnMessageResult result;
    };
}
ConnMessage;

ConnMessage
connMessageDecode(u8* values, ssize size);

ssize
connMessageEncode(ConnMessage message, u8* values, ssize size);

ConnMessage
connMessageJoin(ConnClientFlag flag);

ConnMessage
connMessageData(ConnClientFlag flag, u32 client);

ConnMessage
connMessageTurn(u32 client);

ConnMessage
connMessageMove(u32 client, u32 column);

ConnMessage
connMessageResult(u32 client);

ssize
connMessageToString(ConnMessage message, u8* values, ssize size);

#endif // CONN_MESSAGE_H
