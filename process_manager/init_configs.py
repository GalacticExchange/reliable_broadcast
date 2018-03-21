import os
import logging
import sys
import json

NODE_DIR = "/tmp/node"
CHAINS_CONF_DIR = os.path.join(NODE_DIR, 'm_chains')
NODE_CONF_PATH = os.path.join(NODE_DIR, "node_conf.json")
PIPES_DIR = os.path.join(NODE_DIR, "pipes")

NODE_CONF = {
    "id": 1,
    "ip": "127.0.0.1",
    "port": 1234,
    "chainConfigDirPath": CHAINS_CONF_DIR
}

CHAIN_CONF = {
    "chain_1": {
        "mChainPath": CHAINS_CONF_DIR,
        "mChainHash": "1234",
        "id": 0,
        "nodes": [
            {
                "id": 0,
                "host": "127.0.0.1",
                "port": 1400
            }
        ],
        "comments": [
            {
                "id": 1,
                "host": "127.0.0.1",
                "port": 1401
            },
            {
                "id": 2,
                "host": "127.0.0.1",
                "port": 1402
            },
            {
                "id": 3,
                "host": "127.0.0.1",
                "port": 1403
            },
            {
                "id": 4,
                "host": "127.0.0.1",
                "port": 1404
            }
        ]
    }
}

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler(sys.stdout)
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)


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


create_dir(NODE_DIR)
create_dir(PIPES_DIR)
create_dir(CHAINS_CONF_DIR)

# if not os.path.exists(CHAINS_CONF_DIR):
#     logger.info('CHAINS_CONF_DIR dir not found')
#     os.makedirs(CHAINS_CONF_DIR)

write_json(NODE_CONF_PATH, NODE_CONF)

for f in CHAIN_CONF:
    conf_path = os.path.join(CHAINS_CONF_DIR, f)
    write_json('{}.json'.format(conf_path), CHAIN_CONF[f])
