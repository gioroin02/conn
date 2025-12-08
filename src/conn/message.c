#ifndef CONN_MESSAGE_C
#define CONN_MESSAGE_C

#include "./message.h"

ConnMessage
connMessageDecode(u8* values, ssize size)
{
    ConnMessage result = {0};

    if (size < CONN_MESSAGE_SIZE_HEADER) return result;

    result.version = connMemoryReadU32(values, 0, size);
    result.length  = connMemoryReadU32(values, 4, size);

    if (size < result.length) return result;

    result.kind = ((ConnMessageKind) connMemoryReadU32(values, 8, size));

    switch (result.kind) {
        case ConnMessage_Join:
            result.join.clientFlags = ((ConnClientFlag) connMemoryReadU32(values, 12, size));
        break;

        case ConnMessage_Data: {
            result.data.clientFlags  = ((ConnClientFlag) connMemoryReadU32(values, 12, size));
            result.data.clientCode   = connMemoryReadU32(values, 16, size);
            result.data.clientSymbol = connMemoryReadU8(values, 20, size);
        } break;

        case ConnMessage_Turn:
            result.turn.clientCode = connMemoryReadU32(values, 12, size);
        break;

        case ConnMessage_Move: {
            result.move.clientCode = connMemoryReadU32(values, 12, size);
            result.move.column     = connMemoryReadU32(values, 16, size);
        } break;

        case ConnMessage_Result:
            result.result.clientCode = connMemoryReadU32(values, 12, size);
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
        case ConnMessage_Data:   length += 9; break;
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
            connMemoryWriteU32(message.join.clientFlags, values, 12, size);
        break;

        case ConnMessage_Data: {
            connMemoryWriteU32(message.data.clientFlags, values, 12, size);
            connMemoryWriteU32(message.data.clientCode,  values, 16, size);
            connMemoryWriteU8(message.data.clientSymbol, values, 20, size);
        } break;

        case ConnMessage_Turn:
            connMemoryWriteU32(message.turn.clientCode, values, 12, size);
        break;

        case ConnMessage_Move: {
            connMemoryWriteU32(message.move.clientCode, values, 12, size);
            connMemoryWriteU32(message.move.column,     values, 16, size);
        } break;

        case ConnMessage_Result:
            connMemoryWriteU32(message.result.clientCode, values, 12, size);
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
            .clientFlags = flags,
        }
    };
}

ConnMessage
connMessageData(ConnPlayer player)
{
    return (ConnMessage) {
        .kind = ConnMessage_Data,

        .data = {
            .clientFlags  = player.flags,
            .clientCode   = player.code,
            .clientSymbol = player.symbol,
        }
    };
}

ConnMessage
connMessageTurn(u32 code)
{
    return (ConnMessage) {
        .kind = ConnMessage_Turn,

        .turn = {
            .clientCode = code,
        }
    };
}

ConnMessage
connMessageMove(u32 code, u32 column)
{
    return (ConnMessage) {
        .kind = ConnMessage_Move,

        .move = {
            .clientCode = code,
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
            .clientCode = code,
        }
    };
}

#endif // CONN_MESSAGE_C
