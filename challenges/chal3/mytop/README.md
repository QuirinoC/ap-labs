MyTop
=====
`top` tool implementation using data from /proc

Build

```bash
    $ make
```
Remove binaries and txt files

```bash
    $ make clean
```

Run with

```bash
    $ sudo ./mytop
```
    

Use `ctr+c` to take snapshot that will be stored to a txt 

Use `ctrl+\` to stop execution


Sample mytop output
-------------------
```
# ./mytop
| PID   | Parent | Name           | State    | Memory | # Threads | Open Files |
|-------|--------|----------------|----------|--------|-----------|------------|
| 120   | 1      | sshd           | Active   | 120M   | 1         | 5          |
| 3453  | 120    | sshd-child     | Sleeping | 4M     | 1         | 0          |
| 433   | 433    | single-process | Active   | 15M    | 2         | 1          |
| 3434  | 34     | multi-threads  | Active   | 1020M  | 4         | 5          |
| 16887 | 16886  | test-app       | Sleeping | 17M    | 1         | 7          |
```


