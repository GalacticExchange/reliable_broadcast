from tester import Tester
from time import perf_counter
from enum import IntEnum


class EventType(IntEnum):
    DELIVER_MESSAGE = -1
    STOP_SENDING = 0
    SEND_MESSAGE = 1


class Event:
    def __init__(self, event_type):
        self.time = perf_counter()
        self.type = event_type


class QueueSizeTester(Tester):
    messages = None
    events = None
    start_time = None

    def __init__(self, mchains, client, redis_addresses, loop):
        super().__init__(mchains, client, redis_addresses, loop)
        self.node_count = len(redis_addresses)

    def on_test_start(self):
        self.messages = {mchain: dict() for mchain in self.mchains}
        self.events = list()
        self.start_time = perf_counter()

    def on_send(self, mchain, data):
        # print('Send', data, 'to', mchain)
        message = data.decode()
        self.messages[mchain][message] = set()
        self.events.append(Event(EventType.SEND_MESSAGE))

    def on_delivery(self, mchain, node_index, data):
        # print('Deliver', data, 'of', mchain, 'on #', node_index)
        message = data.decode()
        if message not in self.messages[mchain]:
            print('Node #%d deliver non sent message %s on chain %s' % (node_index, message, mchain))
        else:
            if node_index in self.messages[mchain][message]:
                print('Node #%d deliver message %s several times on chain %s' % (node_index, message, mchain))
            else:
                self.messages[mchain][message].add(node_index)
                count = len(self.messages[mchain][message])
                if count == self.node_count:
                    self.messages[mchain].pop(message)
                    self.events.append(Event(EventType.DELIVER_MESSAGE))

    def on_stop_sending(self):
        self.events.append(Event(EventType.STOP_SENDING))

    def get_result(self):
        times = [event.time - self.start_time for event in self.events if event.type != EventType.STOP_SENDING]
        queue_size, queue_sizes = 0, list()
        stop_time = None
        for event in self.events:
            if event.type == EventType.STOP_SENDING:
                stop_time = event.time - self.start_time
            else:
                queue_size += int(event.type)
                queue_sizes.append(queue_size)

        return times, queue_sizes, stop_time
