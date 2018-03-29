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

    mchain = 1234
    # mchain = 5
    client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    loop = asyncio.get_event_loop()
    tester = ConsistencyTester([mchain], client, [(address[0], address[2]) for address in config[mchain]], loop)
    test_result = asyncio.ensure_future(tester.test(3, rps=100, completion_time=1), loop=loop)
    loop.run_until_complete(test_result)
    print(test_result.result())


if __name__ == '__main__':
    main()
