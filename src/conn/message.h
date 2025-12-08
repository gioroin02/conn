#ifndef CONN_MESSAGE_H
#define CONN_MESSAGE_H

#include "./memory.h"

#define CONN_MESSAGE_SIZE_HEADER ((ssize) 8)
#define CONN_MESSAGE_SIZE        ((ssize) sizeof(ConnMessage))

typedef enum ConnClientFlag
{
    ConnClient_None     = 0,
    ConnClient_Player   = 1 << 0,
    ConnClient_Computer = 1 << 1,
}
ConnClientFlag;

typedef struct ConnPlayer
{
    ConnClientFlag flags;
    u32            code;
    u8             symbol;
}
ConnPlayer;

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
    ConnClientFlag clientFlags;
}
ConnMessageJoin;

typedef struct ConnMessageData
{
    ConnClientFlag clientFlags;
    u32            clientCode;
    u8             clientSymbol;
}
ConnMessageData;

typedef struct ConnMessageTurn
{
    u32 clientCode;
}
ConnMessageTurn;

typedef struct ConnMessageMove
{
    u32 clientCode;
    u32 column;
}
ConnMessageMove;

typedef struct ConnMessageResult
{
    u32 clientCode;
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
connMessageJoin(ConnClientFlag flags);

ConnMessage
connMessageData(ConnPlayer player);

ConnMessage
connMessageTurn(u32 code);

ConnMessage
connMessageMove(u32 code, u32 column);

ConnMessage
connMessageResult(u32 code);

#endif // CONN_MESSAGE_H
