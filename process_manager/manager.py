import os
import logging
import sys

FIFO_NAME = ["_test_fifo"]
FIFO_DIR = '/tmp/skale_fifo'

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler(sys.stdout)
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

if not os.path.exists(FIFO_DIR):
    logger.info('FIFO dir not found')
    os.makedirs(FIFO_DIR)

for f in FIFO_NAME:
    fifo_path = os.path.join(FIFO_DIR, f)
    if os.path.exists(fifo_path): os.remove(fifo_path)

    try:
        os.mkfifo(fifo_path)
    except OSError as e:
        print(e)
        pass
