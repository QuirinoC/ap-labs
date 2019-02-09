from random import randint
import subprocess
from time import time
from pprint import pprint

import os

def cls():
    os.system('cls' if os.name=='nt' else 'clear')

def timer(*command):
    start = time()
    subprocess.run(command)
    return time()-start

def run(prog1, prog2):
    results = []
    file_name = 'test.txt'
    file_size = 1 
    for i in range(10):
        with open(file_name, 'w+') as file:
            file_size *= 5
            print(f"Creating file with {file_size} chars...")
            for i in range(file_size):
                file.write(chr(randint(97,123)))

        results.append(
            (timer(f'./{prog1}', file_name), timer(f'./{prog2}', file_name))
        )
        os.system('cls' if os.name=='nt' else 'clear')

    return results
