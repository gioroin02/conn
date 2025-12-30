#ifndef CONN_MESSAGE_C
#define CONN_MESSAGE_C

#include "./message.h"

#include <stdio.h>

ConnMessage
connMessageDecode(u8* values, ssize size)
{
    ConnMessage result;

    if (size < CONN_MESSAGE_SIZE_HEADER) return result;

    result.version = connMemoryReadU32(values, 0, size);
    result.length  = connMemoryReadU32(values, 4, size);

    if (size < result.length) return result;

    result.kind = ((ConnMessageKind) connMemoryReadU32(values, 8, size));

    switch (result.kind) {
        case ConnMessage_Join:
            result.join.flag = ((ConnClientFlag) connMemoryReadU32(values, 12, size));
        break;

        case ConnMessage_Data: {
            result.data.flag   = ((ConnClientFlag) connMemoryReadU32(values, 12, size));
            result.data.client = connMemoryReadU32(values, 16, size);
        } break;

        case ConnMessage_Turn:
            result.turn.client = connMemoryReadU32(values, 12, size);
        break;

        case ConnMessage_Move: {
            result.move.client = connMemoryReadU32(values, 12, size);
            result.move.column = connMemoryReadU32(values, 16, size);
        } break;

        case ConnMessage_Result:
            result.result.client = connMemoryReadU32(values, 12, size);
        break;

        default: break;
    }

    return result;
}

ssize
connMessageEncode(ConnMessage message, u8* values, ssize size)
{
    ssize version = 1;
    ssize length  = CONN_MESSAGE_SIZE_HEADER + 4;

    switch (message.kind) {
        case ConnMessage_Join:   length += 4; break;
        case ConnMessage_Data:   length += 8; break;
        case ConnMessage_Turn:   length += 4; break;
        case ConnMessage_Move:   length += 8; break;
        case ConnMessage_Result: length += 4; break;

        default: break;
    }

    if (size < length) return 0;

    connMemoryWriteU32(version,      values, 0, size);
    connMemoryWriteU32(length,       values, 4, size);
    connMemoryWriteU32(message.kind, values, 8, size);

    switch (message.kind) {
        case ConnMessage_Join:
            connMemoryWriteU32(message.join.flag, values, 12, size);
        break;

        case ConnMessage_Data: {
            connMemoryWriteU32(message.data.flag,   values, 12, size);
            connMemoryWriteU32(message.data.client, values, 16, size);
        } break;

        case ConnMessage_Turn:
            connMemoryWriteU32(message.turn.client, values, 12, size);
        break;

        case ConnMessage_Move: {
            connMemoryWriteU32(message.move.client, values, 12, size);
            connMemoryWriteU32(message.move.column, values, 16, size);
        } break;

        case ConnMessage_Result:
            connMemoryWriteU32(message.result.client, values, 12, size);
        break;

        default: break;
    }

    return length;
}

ConnMessage
connMessageJoin(ConnClientFlag flags)
{
    return (ConnMessage) {
        .kind = ConnMessage_Join,

        .join = {
            .flag = flags,
        }
    };
}

ConnMessage
connMessageData(ConnClientFlag flag, u32 client)
{
    return (ConnMessage) {
        .kind = ConnMessage_Data,

        .data = {
            .flag   = flag,
            .client = client,
        }
    };
}

ConnMessage
connMessageTurn(u32 code)
{
    return (ConnMessage) {
        .kind = ConnMessage_Turn,

        .turn = {
            .client = code,
        }
    };
}

ConnMessage
connMessageMove(u32 code, u32 column)
{
    return (ConnMessage) {
        .kind = ConnMessage_Move,

        .move = {
            .client = code,
            .column     = column,
        }
    };
}

ConnMessage
connMessageResult(u32 code)
{
    return (ConnMessage) {
        .kind = ConnMessage_Result,

        .result = {
            .client = code,
        }
    };
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

        case ConnMessage_Data: {
            result = snprintf(((char*) values), size, "(Data) {.flag = %u, .client = %lu}",
                message.data.flag, message.data.client);
        } break;

        case ConnMessage_Turn: {
            result = snprintf(((char*) values), size, "(Turn) {.client = %lu}",
                message.turn.client);
        } break;

        case ConnMessage_Move: {
            result = snprintf(((char*) values), size, "(Move) {.client = %lu, .column = %lu}",
                message.move.client, message.move.column);
        } break;

        case ConnMessage_Result: {
            result = snprintf(((char*) values), size, "(Result) {.client = %lu}",
                message.result.client);
        } break;

        default: result = snprintf(((char*) values), size, "(None) {}"); break;
    }

    if (result >= 0 && result < size) return result;

    return 0;
}

#endif // CONN_MESSAGE_C
