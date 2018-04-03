import redis
import itertools
import string
from udp_client import UdpClient
from test_config import config
from time import sleep, time
from numpy import mean, std
from redis_listener import RedisListener
from consistency_tester import ConsistencyTester
import asyncio
import matplotlib.pyplot as plt
from udp_client import AsyncUdpClient


def string_generator():
    for length in itertools.count(1):
        for letters in itertools.product(string.ascii_lowercase, repeat=length):
            yield ''.join(letters)


class MChainTester:
    _client = None
    _mchain_hash = None
    _string_generator = string_generator()
    _redis_connections = list()

    def __init__(self, mchain_hash, client, redis_addresses):
        self._mchain_hash = mchain_hash
        self._client = client
        self._redis_connections = self._init_redis_connections(redis_addresses)

    # public

    def test(self, concurrency=1):
        sent_messages = set()
        for _ in range(concurrency):
            message = next(self._string_generator)
            print('Send message \'%s\'' % message)
            self._client.send(self._mchain_hash, message.encode())
            sent_messages.add(message)
        for index, redis_connection in enumerate(self._redis_connections):
            print('Check delivery for node #%d' % index)
            sent_messages_copy = set(sent_messages)
            # for _ in range(len(sent_messages)):
            while sent_messages_copy:
                data = redis_connection.blpop(str(self._mchain_hash))
                delivered_message = data[1].decode()
                print('\tDelivered message \'%s\'' % delivered_message)
                if delivered_message not in sent_messages_copy:
                    # raise KeyError('Message \'%s\' was not sent' % delivered_message)
                    print('Message \'%s\' was not sent' % delivered_message)
                else:
                    sent_messages_copy.remove(delivered_message)
            if sent_messages_copy:
                # raise KeyError('[' + ', '.join(sent_messages_copy) + '] were not delivered')
                print('[' + ', '.join(sent_messages_copy) + '] were not delivered')
        print('OK')

    # private

    def _init_redis_connections(self, redis_addresses):
        return [redis.StrictRedis(host=address[0], port=address[1]) for address in redis_addresses]


def test():
    mchain = 1234
    # mchain = 5
    client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    loop = asyncio.get_event_loop()
    tester = ConsistencyTester([mchain], client, [(address[0], address[2]) for address in config[mchain]], loop)

    min_rps, max_rps = 700, 800
    test_number = 20
    rps_axis, good, partial, lost = list(), list(), list(), list()
    for rps in range(min_rps, max_rps, (max_rps - min_rps) // (test_number - 1)):
        test_result = asyncio.ensure_future(tester.test(10, rps=rps, completion_time=3), loop=loop)
        loop.run_until_complete(test_result)

        test_result = test_result.result()
        counters, rps = test_result
        rps_axis.append(rps)

        _good, _partial, _lost = counters[mchain]
        total = _good + _partial + _lost
        good.append(_good / total)
        partial.append(_partial / total)
        lost.append(_lost / total)

    fig, ax1 = plt.subplots()
    ax1.plot(rps_axis, good, 'g-', label='delivered')
    ax1.plot(rps_axis, lost, 'orange', label='lost')
    ax1.set_xlabel('Messages per second')
    # Make the y-axis label, ticks and tick labels match the line color.
    ax1.set_ylabel('Share')
    ax1.set_ylim([0, 1.1])
    ax1.legend()
    # ax1.tick_params('y', colors='b')

    ax2 = ax1.twinx()
    max_partial = max(partial)
    if max_partial > 0:
        # partial = [partial / max_partial for partial in partial]
        ax2.set_ylim([0, max_partial * 1.1])
    else:
        ax2.set_ylim([0, 1.1])
    ax2.plot(rps_axis, partial, 'r')
    ax2.set_ylabel('Partially delivered share')
    # ax2.tick_params('y', colors='r')

    fig.tight_layout()
    plt.show()

    # plt.plot(rps_axis, good, rps_axis, partial, rps_axis, lost)
    # plt.xlabel('Messages per second')
    # plt.ylabel()
    # plt.show()


def main():
    # r = redis.StrictRedis(host='localhost', port=6379)
    # key = str(5)
    # r.rpush(key, 'test')
    # result = r.lpop(key)
    # print(result)
    #
    # return

    # mchain = 1234
    # # mchain = 5
    # client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    # tester = MChainTester(mchain, client, [(address[0], address[2]) for address in config[mchain]])
    # rps_list = list()
    # while True:
    #     n = 100
    #     start = time()
    #     tester.test(n)
    #     finish = time()
    #     duration = finish - start
    #     rps = n / duration
    #     rps_list.append(rps)
    #     print('Process %d messages per second' % int(rps))
    #     print('Mean %d messages per second with deviation %d' % (int(mean(rps_list)), int(std(rps_list))))
    #     sleep(1)

    # test()

    # mchain = 1234
    # # mchain = 5
    # client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    # loop = asyncio.get_event_loop()
    # tester = ConsistencyTester([mchain], client, [(address[0], address[2]) for address in config[mchain]], loop)
    # test_result = asyncio.ensure_future(tester.test(3, rps=1, completion_time=1), loop=loop)
    # loop.run_until_complete(test_result)
    # print(test_result.result())

    mchain = 1234
    loop = asyncio.get_event_loop()
    client = AsyncUdpClient([(address[0], address[1]) for address in config[mchain]], loop)
    client.send(mchain, 'Test message'.encode())
    loop.run_forever()


if __name__ == '__main__':
    main()
