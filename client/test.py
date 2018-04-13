import itertools
import string
from udp_client import UdpClient
from test_config import config
from queue_size_tester import QueueSizeTester
import asyncio
import matplotlib.pyplot as plt
import sys


def string_generator():
    for length in itertools.count(1):
        for letters in itertools.product(string.ascii_lowercase, repeat=length):
            yield ''.join(letters)


def consistency_test():
    mchain = 1234
    # mchain = 5
    client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    loop = asyncio.get_event_loop()
    tester = QueueSizeTester([mchain], client, [(address[0], address[2]) for address in config[mchain]], loop)

    min_rps, max_rps = 1000, 10000
    test_number = 10
    rps_axis, good, partial, lost = list(), list(), list(), list()
    for i, rps in enumerate(range(min_rps, max_rps, (max_rps - min_rps) // (test_number - 1))):
        test_result = asyncio.ensure_future(tester.test(3, rps=rps, completion_time=2, iter_number=i), loop=loop)
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


def queue_test():
    mchain = 1234
    # mchain = 5
    client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    loop = asyncio.get_event_loop()
    tester = QueueSizeTester([mchain], client, [(address[0], address[2]) for address in config[mchain]], loop)

    min_rps, max_rps = 1000, 3000
    test_number = 3
    test_duration, completion_time = 3, 2
    fig, ax = plt.subplots()
    ax.set_xlabel('Time (seconds)')
    ax.set_ylabel('Messages in process')
    max_time, max_queue = 0, 0
    for i, rps in enumerate(range(min_rps, max_rps + 1, (max_rps - min_rps) // (test_number - 1))):
        test_result = asyncio.ensure_future(tester.test(test_duration,
                                                        rps=rps,
                                                        completion_time=completion_time,
                                                        iter_number=i), loop=loop)
        loop.run_until_complete(test_result)

        (x, y, _), rps = test_result.result()
        ax.plot(x, y, label='%d rps' % int(rps))

        max_time = max(max_time, x[-1])
        max_queue = max(max_queue, max(y))

    ax.plot([test_duration, test_duration], [0, max_queue], 'k:')

    ax.set_xlim([0, max_time])
    ax.set_ylim([0, max_queue])
    ax.legend()

    plt.show()


def main():
    test_function = consistency_test
    if 'queue' in sys.argv:
        test_function = queue_test
    test_function()

    # consistency_test()

    # mchain = 1234
    # # mchain = 5
    # client = UdpClient([(address[0], address[1]) for address in config[mchain]])
    # loop = asyncio.get_event_loop()
    # tester = ConsistencyTester([mchain], client, [(address[0], address[2]) for address in config[mchain]], loop)
    # test_result = asyncio.ensure_future(tester.test(3, rps=1, completion_time=1), loop=loop)
    # loop.run_until_complete(test_result)
    # print(test_result.result())

    # mchain = 1234
    # loop = asyncio.get_event_loop()
    # client = AsyncUdpClient([(address[0], address[1]) for address in config[mchain]], loop)
    # client.send(mchain, 'Test message'.encode())
    # loop.run_forever()


if __name__ == '__main__':
    main()
