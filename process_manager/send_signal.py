import os
import signal
import sys


def send_signal(pid):
    print("sending SIGPOLL to {}".format(pid))
    os.kill(int(pid), signal.SIGPOLL)


if len(sys.argv) == 1:
    raise Exception("PID not passed")

send_signal(sys.argv[1])
