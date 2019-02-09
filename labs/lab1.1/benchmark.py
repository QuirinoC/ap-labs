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

results = []
file_name = 'test.txt'
for i in range(3,8):
    with open(file_name, 'w+') as file:
        file_size = int('0x0f'+ 'f'*i ,16)
        print(f"Creating file with {file_size} chars...")
        for i in range(file_size):
            file.write(chr(randint(97,123)))

        results.append(
            (timer('./mycat', file_name), timer('./cat', file_name))
        )
    os.system('cls' if os.name=='nt' else 'clear')

avg= 0
for a,b in results:
    avg += b/a
avg /= len(results)

print(results)
print(f"mycat has a speed up of {avg} over cat")