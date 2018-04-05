from enum import IntEnum


class MessageType(IntEnum):
    SEND = 0,
    ECHO = 1,
    READY = 2


class Message:
    mChain_hash = 0
    client_id = 0
    nonce = 0
    message_type = MessageType.SEND
    data = ''.encode()

    def encode(self):
        return self.mChain_hash.to_bytes(8, byteorder='little') \
               + self.client_id.to_bytes(8, byteorder='little') \
               + self.nonce.to_bytes(8, byteorder='little') \
               + int(0).to_bytes(8, byteorder='little') \
               + self.message_type.to_bytes(1, byteorder='little') \
               + self.data

    def packet_encode(self):
        m_buffer = self.mChain_hash.to_bytes(8, byteorder='little') \
                   + self.client_id.to_bytes(8, byteorder='little') \
                   + self.nonce.to_bytes(8, byteorder='little') \
                   + int(0).to_bytes(8, byteorder='little') \
                   + self.message_type.to_bytes(1, byteorder='little') \
                   + self.data

        return \
            int(1).to_bytes(8, byteorder='little') + \
            int(len(m_buffer)).to_bytes(8, byteorder='little') + \
            m_buffer

    def __init__(self, mChain_hash, client_id, nonce, message_type, data):
        self.mChain_hash = mChain_hash
        self.client_id = client_id
        self.nonce = nonce
        self.message_type = message_type
        self.data = data