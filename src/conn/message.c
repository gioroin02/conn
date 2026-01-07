#ifndef CONN_MESSAGE_C
#define CONN_MESSAGE_C

#include "message.h"

#include <stdio.h>

static ssize conn_message_size[] =
{
    [ConnMessage_None]   = 0,
    [ConnMessage_Accept] = 0,
    [ConnMessage_Reject] = 0,
    [ConnMessage_Join]   = CONN_MESSAGE_SIZE_HEADER + 2,
    [ConnMessage_Quit]   = CONN_MESSAGE_SIZE_HEADER + 2,
    [ConnMessage_Data]   = CONN_MESSAGE_SIZE_HEADER + 6,
    [ConnMessage_Turn]   = CONN_MESSAGE_SIZE_HEADER + 2,
    [ConnMessage_Move]   = CONN_MESSAGE_SIZE_HEADER + 4,
    [ConnMessage_Result] = CONN_MESSAGE_SIZE_HEADER + 2,
    [ConnMessage_Count]  = 0,
};

ConnMessage
connMessageDecode(u8* values, ssize size)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind    = ConnMessage_None;
    result.version = 0;
    result.length  = 0;

    if (values == PX_NULL || size < CONN_MESSAGE_SIZE_HEADER) return result;

    u8*   memory = values;
    ssize length = size;

    memory = connMemoryReadU32Net(memory, &length, &result.version);
    memory = connMemoryReadU32Net(memory, &length, &result.length);
    memory = connMemoryReadU32Net(memory, &length, (u32*) &result.kind);

    if (result.kind < 0 || result.kind >= ConnMessage_Count) return result;

    if (size < result.length || result.length != conn_message_size[result.kind])
        return result;

    switch (result.kind) {
        case ConnMessage_Join:
            memory = connMemoryReadU32Net(memory, &length, (u32*) &result.join.flag);
        break;

        case ConnMessage_Quit:
            memory = connMemoryReadU16Net(memory, &length, &result.quit.client);
        break;

        case ConnMessage_Data: {
            memory = connMemoryReadU32Net(memory, &length, (u32*) &result.data.flag);
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

    if (memory != PX_NULL) return result;

    pxMemorySet(&result, sizeof result, 0xAB);

    return result;
}

ssize
connMessageEncode(ConnMessage message, u8* values, ssize size)
{
    if (message.kind < 0 || message.kind >= ConnMessage_Count) return 0;

    if (values == PX_NULL || size < conn_message_size[message.kind])
        return 0;

    u8*   memory = values;
    ssize length = size;

    memory = connMemoryWriteU32Net(memory, &length, 1);
    memory = connMemoryWriteU32Net(memory, &length, conn_message_size[message.kind]);
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

    if (memory != PX_NULL) return size - length;

    return 0;
}

ConnMessage
connMessageJoin(ConnClientFlag flag)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind      = ConnMessage_Join;
    result.join.flag = flag;

    return result;
}

ConnMessage
connMessageQuit(u16 client)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Quit;
    result.quit.client = client;

    return result;
}

ConnMessage
connMessageData(ConnClientFlag flag, u16 client)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Data;
    result.data.flag   = flag;
    result.data.client = client;

    return result;
}

ConnMessage
connMessageTurn(u16 client)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Turn;
    result.turn.client = client;

    return result;
}

ConnMessage
connMessageMove(u16 client, u16 column)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = ConnMessage_Move;
    result.move.client = client;
    result.move.column = column;

    return result;
}

ConnMessage
connMessageResult(u16 client)
{
    ConnMessage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind          = ConnMessage_Result;
    result.result.client = client;

    return result;
}

ssize
connMessageToString(ConnMessage message, u8* values, ssize size)
{
    ssize result = 0;

    switch (message.kind) {
        case ConnMessage_Join: {
            result = snprintf(((char*) values), size, "(Join) {.flag = %u}",
                message.join.flag);
        } break;

        case ConnMessage_Quit: {
            result = snprintf(((char*) values), size, "(Quit) {.client = %u}",
                message.quit.client);
        } break;

        case ConnMessage_Data: {
            result = snprintf(((char*) values), size, "(Data) {.flag = %u, .client = %u}",
                message.data.flag, message.data.client);
        } break;

        case ConnMessage_Turn: {
            result = snprintf(((char*) values), size, "(Turn) {.client = %u}",
                message.turn.client);
        } break;

        case ConnMessage_Move: {
            result = snprintf(((char*) values), size, "(Move) {.client = %u, .column = %u}",
                message.move.client, message.move.column);
        } break;

        case ConnMessage_Result: {
            result = snprintf(((char*) values), size, "(Result) {.client = %u}",
                message.result.client);
        } break;

        default: result = snprintf(((char*) values), size, "(None) {}"); break;
    }

    if (result >= 0 && result < size) return result;

    return 0;
}

#endif // CONN_MESSAGE_C
