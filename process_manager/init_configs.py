import os
import json

from logger import logger

NODE_DIR = "/tmp/node"
CHAINS_CONF_DIR = os.path.join(NODE_DIR, 'm_chains')
NODE_CONF_PATH = os.path.join(NODE_DIR, "node_conf.json")
PIPES_DIR = os.path.join(NODE_DIR, "pipes")
LOCAL_PORT = 1122

NODE_CONF = {
    "id": 1,
    "ip": "127.0.0.1",
    "port": 1401,
    "localPort": LOCAL_PORT,
    "chainConfigDir": CHAINS_CONF_DIR,
    "pipesDir": PIPES_DIR
}

CHAIN_CONF = {
    "chain_1": {
        "mChainHash": "1234",
        "nodes": [
            {
                "id": 0,
                "host": "127.0.0.1",
                "port": 1400
            },
            {
                "id": 1,
                "host": "127.0.0.1",
                "port": 1401
            }
        ]
    }
}


def create_dir(path):
    if not os.path.exists(path):
        logger.info("{} dir not found, creating..".format(path))
        os.makedirs(path)


def write_json(path, data):
    if os.path.exists(path):
        logger.info("{} file exists.. removing".format(path))
        os.remove(path)
    try:
        with open(path, 'w') as outfile:
            json.dump(data, outfile)
    except OSError as ex:
        print(ex)
        pass


def make_skale_fifo(path):
    if os.path.exists(path):
        logger.info("{} file exists.. removing".format(path))
        os.remove(path)
    try:
        os.mkfifo(path)
    except OSError as e:
        print(e)
        pass


if __name__ == '__main__':

    create_dir(NODE_DIR)
    create_dir(PIPES_DIR)
    create_dir(CHAINS_CONF_DIR)

    write_json(NODE_CONF_PATH, NODE_CONF)

    for f in CHAIN_CONF:
        conf_path = os.path.join(CHAINS_CONF_DIR, f)
        fifo_path = os.path.join(PIPES_DIR, CHAIN_CONF[f]['mChainHash'])
        make_skale_fifo(fifo_path)
        write_json('{}.json'.format(conf_path), CHAIN_CONF[f])
