# Conn

## Protocol v1

Protocol header:
    - 4 byte: protocol version (0x0000_0001)
    - 4 byte: total message length (payload + header)
    - 4 byte: message type

Join message (0x0003):
    - 2 byte: client type

Quit message (0x0004):
    - 2 byte: client code

Data message (0x0005):
    - 4 byte: client type
    - 2 byte: client code

Turn message (0x0006):
    - 2 byte: client code

Move message (0x0007):
    - 2 byte: client code
    - 2 byte: selected column

Result message (0x0008):
    - 2 byte: client code

## Terminal

### Input

Ricezione di byte e conversione in comandi costante, che va a riempire una coda di comandi. La coda di comandi viene gestita quando possibile.

Elenco di comandi:

1. A, muove selezione a sinistra (ignorato se fuori dal turno)
2. D, muove selezione a destra (ignorato se fuori dal turno)
3. Invio, invia mossa (ignorato se fuori dal turno)
4. Escape, esce dalla partita.

### Output


