#ifndef CONN_MESSAGE_C
#define CONN_MESSAGE_C

#include "message.h"

#include <stdio.h>

static Int connMessageSize(ConnMessageKind kind)
{
    switch (kind) {
        case ConnMessage_Join:   return CONN_MESSAGE_SIZE_HEADER + 2;
        case ConnMessage_Quit:   return CONN_MESSAGE_SIZE_HEADER + 2;
        case ConnMessage_Data:   return CONN_MESSAGE_SIZE_HEADER + 6;
        case ConnMessage_Turn:   return CONN_MESSAGE_SIZE_HEADER + 2;
        case ConnMessage_Move:   return CONN_MESSAGE_SIZE_HEADER + 4;
        case ConnMessage_Result: return CONN_MESSAGE_SIZE_HEADER + 2;

        default: break;
    }

    return 0;
}

ConnMessage connMessageJoin(ConnClientFlag flag)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind      = ConnMessage_Join;
    result.join.flag = flag;

    return result;
}

ConnMessage connMessageQuit(U16 client)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Quit;
    result.quit.client = client;

    return result;
}

ConnMessage connMessageData(ConnClientFlag flag, U16 client)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Data;
    result.data.flag   = flag;
    result.data.client = client;

    return result;
}

ConnMessage connMessageTurn(U16 client)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Turn;
    result.turn.client = client;

    return result;
}

ConnMessage connMessageMove(U16 client, U16 column)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Move;
    result.move.client = client;
    result.move.column = column;

    return result;
}

ConnMessage connMessageResult(U16 client)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind          = ConnMessage_Result;
    result.result.client = client;

    return result;
}

Bool connMessageDecode(ConnMessage* message, U8* pntr, Int size)
{
    ConnMessage result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind    = ConnMessage_None;
    result.version = 0;
    result.length  = 0;

    if (pntr == NULL || size < CONN_MESSAGE_SIZE_HEADER) return 1;

    U8* memory = pntr;
    Int length = size;

    memory = connMemoryReadU32Net(memory, &length, &result.version);
    memory = connMemoryReadU32Net(memory, &length, &result.length);
    memory = connMemoryReadU32Net(memory, &length, (U32*) &result.kind);

    if (size < result.length || result.length != connMessageSize(result.kind))
        return 0;

    switch (result.kind) {
        case ConnMessage_Join:
            memory = connMemoryReadU32Net(memory, &length, (U32*) &result.join.flag);
        break;

        case ConnMessage_Quit:
            memory = connMemoryReadU16Net(memory, &length, &result.quit.client);
        break;

        case ConnMessage_Data: {
            memory = connMemoryReadU32Net(memory, &length, (U32*) &result.data.flag);
            memory = connMemoryReadU16Net(memory, &length, &result.data.client);
        } break;

        case ConnMessage_Turn:
            memory = connMemoryReadU16Net(memory, &length, &result.turn.client);
        break;

        case ConnMessage_Move: {
            memory = connMemoryReadU16Net(memory, &length, &result.move.client);
            memory = connMemoryReadU16Net(memory, &length, &result.move.column);
        } break;

        case ConnMessage_Result:
            memory = connMemoryReadU16Net(memory, &length, &result.result.client);
        break;

        default: break;
    }

    if (memory == NULL) return 0;

    if (message != NULL) *message = result;

    return 1;
}

Int connMessageEncode(ConnMessage message, U8* pntr, Int size)
{
    if (pntr == NULL || size < connMessageSize(message.kind)) return 0;

    U8* memory = pntr;
    Int length = size;

    memory = connMemoryWriteU32Net(memory, &length, 1);
    memory = connMemoryWriteU32Net(memory, &length, connMessageSize(message.kind));
    memory = connMemoryWriteU32Net(memory, &length, message.kind);

    switch (message.kind) {
        case ConnMessage_Join:
            memory = connMemoryWriteU32Net(memory, &length, message.join.flag);
        break;

        case ConnMessage_Quit:
            memory = connMemoryWriteU16Net(memory, &length, message.quit.client);
        break;

        case ConnMessage_Data: {
            memory = connMemoryWriteU32Net(memory, &length, message.data.flag);
            memory = connMemoryWriteU16Net(memory, &length, message.data.client);
        } break;

        case ConnMessage_Turn:
            memory = connMemoryWriteU16Net(memory, &length, message.turn.client);
        break;

        case ConnMessage_Move: {
            memory = connMemoryWriteU16Net(memory, &length, message.move.client);
            memory = connMemoryWriteU16Net(memory, &length, message.move.column);
        } break;

        case ConnMessage_Result:
            memory = connMemoryWriteU16Net(memory, &length, message.result.client);
        break;

        default: break;
    }

    if (memory != NULL) return size - length;

    return 0;
}

Int connMessageToString(ConnMessage message, U8* pntr, Int size)
{
    Int result = 0;

    switch (message.kind) {
        case ConnMessage_Join: {
            result = snprintf((I8*) pntr, size, "(Join) {.flag = %u}",
                message.join.flag);
        } break;

        case ConnMessage_Quit: {
            result = snprintf((I8*) pntr, size, "(Quit) {.client = %u}",
                message.quit.client);
        } break;

        case ConnMessage_Data: {
            result = snprintf((I8*) pntr, size, "(Data) {.flag = %u, .client = %u}",
                message.data.flag, message.data.client);
        } break;

        case ConnMessage_Turn: {
            result = snprintf((I8*) pntr, size, "(Turn) {.client = %u}",
                message.turn.client);
        } break;

        case ConnMessage_Move: {
            result = snprintf((I8*) pntr, size, "(Move) {.client = %u, .column = %u}",
                message.move.client, message.move.column);
        } break;

        case ConnMessage_Result: {
            result = snprintf((I8*) pntr, size, "(Result) {.client = %u}",
                message.result.client);
        } break;

        default: result = snprintf((I8*) pntr, size, "(None) {}"); break;
    }

    if (result >= 0 && result < size) return result;

    return 0;
}

#endif // CONN_MESSAGE_C
