import os
from logger import logger

FIFO_NAME = ["_test_fifo", "12345"]
FIFO_DIR = '/tmp/skale_fifo'

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
