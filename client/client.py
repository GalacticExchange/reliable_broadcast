#!/usr/bin/python3

import asyncio
import websockets
from time import sleep
from enum import IntEnum
import socket
from message import Message, MessageType

uri = 'ws://localhost:9002'
message = 'Hello world'


class Client:
    def send(self, m_chain_hash, data):
        pass


async def hello():

    try:
        async with websockets.connect(uri) as websocket:
            counter = 0
            while True:
                send_message = message + ' - %d' % counter
                await websocket.send(send_message)
                print("> {}".format(send_message))

                # greeting = await websocket.recv()
                # print("< {}".format(greeting))
                counter += 1
                sleep(1)
    except Exception as e:
        print(e)


def main():
    # asyncio.get_event_loop().run_until_complete(hello())

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = ('localhost', 1234)

    nonce = 1
    while True:
        message = Message(5, 1, nonce, MessageType.SEND, 'hello'.encode())
        buffer = message.encode()
        sent = sock.sendto(buffer, server_address)
        print('Sent message with nonce %d (%d bytes)' % (message.nonce, sent))
        nonce += 1
        sleep(1)


if __name__ == '__main__':
    main()
