from udp_client import UdpClient
from redis_listener import RedisListener
import asyncio
from test_config import config
from test import string_generator
import functools
import aioredis


async def test(loop):
    mchain = 1234
    # mchain = 5
    client = UdpClient([(address[0], address[1]) for address in config[mchain]])

    strings = string_generator()

    redis_listener = RedisListener(loop)

    def handler(source_id, mchain, data):
        print(source_id, mchain, data)
    redis_listener.register_handler(handler)
    redis_listener.listen([mchain], [(address[0], address[2]) for address in config[mchain]])

    while True:
        print('Send data')
        client.send(mchain, next(strings).encode())
        await asyncio.sleep(1)


def main():
    loop = asyncio.get_event_loop()
    loop.run_until_complete(test(loop))


if __name__ == '__main__':
    main()
