import asyncio
import aioredis


class RedisListener:
    connections = dict()
    handlers = list()

    def __init__(self, loop):
        self.loop = loop

    # public

    def listen(self, mchains, addresses):
        asyncio.ensure_future(self._listen(mchains, addresses), loop=self.loop)

    def register_handler(self, handler):
        self.handlers.append(handler)

    # private

    async def _listen(self, mchains, addresses):
        for index, address in enumerate(addresses):
            self.connections[index] = await aioredis.create_redis(address, loop=self.loop)
        for connection in self.connections.values():
            for mchain in mchains:
                await connection.delete(str(mchain))

        tasks = [self._get_data(source_id, mchains) for source_id in self.connections.keys()]

        while True:
            completed, pending = await asyncio.wait(tasks, loop=self.loop)
            for task in completed:
                source_id, popped = task.result()
                mchain, data = int(popped[0].decode()), popped[1]
                for handler in self.handlers:
                    handler(source_id, mchain, data)
                pending.add(self._get_data(source_id, mchains))
            tasks = pending

    async def _get_data(self, source_id, mchains):
        return source_id, await self.connections[source_id].blpop(*mchains)
