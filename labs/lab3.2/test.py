import os
import signal
import time
import sys

def spam(pid):
    print("Spamming SIGINT to " + str(pid))
    while 1:
        os.kill(pid, signal.SIGINT);
        time.sleep(0.01)

def main():
    if len(sys.argv) < 2:
        print("Not enough args")
        return

    try:
        spam(int(sys.argv[1]))
    except:
        print("Program execution terminated")

if __name__ == "__main__":
    main()