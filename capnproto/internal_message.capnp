@0xbf4ddef67d0e61e6;

struct InternalMessage {
    clientId @0 : UInt64;
    nonce @1 : UInt64;
    mchainHash @2 : UInt64;
    nodeId @4 : UInt64;
    type @5 : MessageType;
    message @3 : Data;

    enum MessageType {
        send @0;
        echoMessage @1;
        ready @2;
    }
}
