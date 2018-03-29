from udp_client import UdpClient
from redis_listener import RedisListener
import asyncio
from test_config import config
import functools
import aioredis
from time import perf_counter
from collections import deque
import itertools
import string



def string_generator():
    for length in itertools.count(1):
        for letters in itertools.product(string.ascii_lowercase, repeat=length):
            yield ''.join(letters)


class Tester:
    time_window_sec = 5
    message_generator = string_generator()

    def __init__(self, mchains, client, redis_addresses, loop):
        self.mchains = mchains
        self.client = client
        self.redis_addresses = redis_addresses
        self.listener = RedisListener(loop)
        self.listener.register_handler(self.on_delivery)
        self.listener.listen(mchains, redis_addresses)

    async def test(self, duration=10, rps=1, completion_time=5):
        start_time = perf_counter()
        request_times = deque()
        while True:
            for mchain in self.mchains:
                message = next(self.message_generator)
                data = message.encode()
                self.client.send(mchain, data)
                self.on_send(mchain, data)

                current_time = perf_counter()
                request_times.append(current_time)

                interval_start = max(start_time, current_time - self.time_window_sec)
                while request_times and request_times[0] < interval_start:
                    request_times.popleft()
                if request_times and request_times[0] < current_time:
                    print('RPS =', len(request_times) / (current_time - request_times[0]))

                elapsed_time = current_time - interval_start
                total_count = rps * elapsed_time
                if len(request_times) > total_count:
                    delay = len(request_times) / rps - elapsed_time
                    # print('Delay:', delay)
                    await asyncio.sleep(delay)

            if current_time > start_time + duration:
                break
        await asyncio.sleep(completion_time)
        return self.get_result()

    def on_send(self, mchain, data):
        pass

    def on_delivery(self, mchain, node_index, data):
        pass

    def get_result(self):
        return None


async def test(loop):
    mchain = 1234
    # mchain = 5
    client = UdpClient([(address[0], address[1]) for address in config[mchain]])

    strings = string_generator()

    redis_listener = RedisListener(loop)

    def handler(mchain, source_id, data):
        print(source_id, mchain, data)
    redis_listener.register_handler(handler)
    redis_listener.listen([mchain], [(address[0], address[2]) for address in config[mchain]])

    while True:
        print('Send data')
        client.send(mchain, next(strings).encode())
        await asyncio.sleep(1)


def main():
    loop = asyncio.get_event_loop()
    # loop.run_until_complete(test(loop))

    mchain = 5
    tester = Tester([mchain], None, None)
    loop.run_until_complete(tester.test(10))


if __name__ == '__main__':
    main()
