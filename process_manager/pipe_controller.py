import json
import os
import sys
from logger import logger


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
    if len(sys.argv) < 3:
        raise Exception("Path to configs not passed")

    node_conf = json.load(open(sys.argv[1]))
    chain_conf = json.load(open(sys.argv[2]))
    fifo_path = os.path.join(node_conf['pipesDir'], str(chain_conf['mChainHash']))
    make_skale_fifo(fifo_path)
