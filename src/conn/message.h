#ifndef CONN_MESSAGE_H
#define CONN_MESSAGE_H

#include "memory.h"
#include "game.h"

#define CONN_MESSAGE_SIZE_HEADER ((Int) 12)
#define CONN_MESSAGE_SIZE        ((Int) 18)

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
    U16 client;
}
ConnMessageQuit;

typedef struct ConnMessageData
{
    ConnClientFlag flag;
    U16            client;
}
ConnMessageData;

typedef struct ConnMessageTurn
{
    U16 client;
}
ConnMessageTurn;

typedef struct ConnMessageMove
{
    U16 client;
    U16 column;
}
ConnMessageMove;

typedef struct ConnMessageResult
{
    U16 client;
}
ConnMessageResult;

typedef struct ConnMessage
{
    ConnMessageKind kind;

    U32 version;
    U32 length;

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

ConnMessage connMessageQuit(U16 client);

ConnMessage connMessageData(ConnClientFlag flag, U16 client);

ConnMessage connMessageTurn(U16 client);

ConnMessage connMessageMove(U16 client, U16 column);

ConnMessage connMessageResult(U16 client);

Bool connMessageDecode(ConnMessage* message, U8* pntr, Int size);

Int connMessageEncode(ConnMessage message, U8* pntr, Int size);

Int connMessageToString(ConnMessage message, U8* pntr, Int size);

#endif // CONN_MESSAGE_H
