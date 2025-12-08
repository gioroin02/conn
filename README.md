# Conn

## Protocol v1

Protocol header:
    - 4 byte: protocol version (0x0000_0001)
    - 4 byte: total message length (payload + header)

Join message (0x0001):
    - 4 byte: message type
    - 4 byte: client type

Quit message (0x0002):
    - 4 byte: message type

Data message (0x0003):
    - 4 byte: message type
    - 4 byte: client type
    - 4 byte: client code
    - 1 byte: client ASCII symbol

Turn message:
    - 4 byte: message type (0x0004)
    - 4 byte: client code

Move message:
    - 4 byte: message type (0x0005)
    - 4 byte: client code
    - 4 byte: selected column

Result message:
    - 4 byte: message type (0x0006)
    - 4 byte: client code
    - 1 byte: winner
