@0x81b11f4257125a3d;

struct SkaleMessage {
    clientId @0 : UInt64;
    nonce @1 : UInt64;
    mchainHash @2 : UInt64;
    message @3 : Data;
}
