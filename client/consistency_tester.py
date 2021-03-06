from tester import Tester


class ConsistencyTester(Tester):
    def __init__(self, mchains, client, redis_addresses, loop):
        super().__init__(mchains, client, redis_addresses, loop)
        self.node_count = len(redis_addresses)
        self.messages = {mchain: dict() for mchain in mchains}
        self.total_messages_count = {mchain: 0 for mchain in mchains}
        self.good = {mchain: 0 for mchain in mchains}

    def on_test_start(self):
        self.messages = {mchain: dict() for mchain in self.mchains}
        self.total_messages_count = {mchain: 0 for mchain in self.mchains}
        self.good = {mchain: 0 for mchain in self.mchains}

    def on_send(self, mchain, data):
        # print('Send', data, 'to', mchain)
        self.total_messages_count[mchain] += 1
        message = data.decode()
        self.messages[mchain][message] = set()

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
                    self.good[mchain] += 1

    def get_result(self):
        result = dict()
        for mchain in self.mchains:
            partial = sum(1 for count in self.messages[mchain].values() if len(count) > 0)
            result[mchain] = self.good[mchain], \
                partial, \
                self.total_messages_count[mchain] - (self.good[mchain] + partial)
        return result

