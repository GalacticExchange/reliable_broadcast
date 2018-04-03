from client import Client
from message import Message, MessageType
import socket
from random import randint


class UdpClient(Client):
    _socket = None
    _nodes = list()
    _client_id = 131
    _nonce = randint(0, 1e6)

    def __init__(self, nodes):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._nodes = nodes

    def send(self, mchain_hash, data):
        message = Message(mchain_hash, self._client_id, self._nonce, MessageType.SEND, data)
        buffer = message.encode()
        self._nonce += 1
        for node in self._nodes:
            self._socket.sendto(buffer, node)
