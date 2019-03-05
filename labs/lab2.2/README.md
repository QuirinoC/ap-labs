Lab 2.2 - Logger Library
========================

Function specifications 
- `INFO` General information.
- `WARN` Warnings.
- `ERROR` Errors.
- `PANIC` Panics. Non recoverable issues with core dump.

Function declarations
```
int infof(const char *format, ...)
int warnf(const char *format, ...)
int errorf(const char *format, ...)
int panicf(const char *format, ...)
```

Make

    make

Clean

    make clean

Run

    ./testLogger