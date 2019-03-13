# Lab 2.4 - Syslog Logger Library

Build project with

    make

Clean binaries with

    make clean


Functions to test on testLogger.c
- int infof(const char *format, ...);
- int warnf(const char *format, ...);
- int errorf(const char *format, ...);
- int panicf(const char *format, ...);
- int initLogger(char *logType);

Use infof, warnf, errorf and panicf as you would in a printf

`initLogger` sets the output, by default its set to `stdout` 

```
initLogger("stdout"); // Will set to stdout
```
```
initLogger("syslog"); // Will set to syslog
```
```
initLogger(""); // Will set to stdout
```