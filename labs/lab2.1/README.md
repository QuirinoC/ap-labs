Lab 2.1 - ClockWall
===================

Build
```
make
```
Remove binaries
```
make clean
```
Run as many clock2 as you want, set env TZ in the same command and specify port in this format
```
TZ=US/Eastern    ./clock2 -port 10001
```
Run multiple
```
TZ=US/Eastern    ./clock2 -port 10001 & TZ=Asia/Tokyo    ./clock2 -port 10002 & TZ=Europe/London ./clock2 -port 10003
```
For each clock pass an argument to clockWall
```
./clockWall NewYork=localhost:10001 Tokyo=localhost:10002 London=localhost:10003
```
If running multiple clocks with & use 
```
kill -9 [PID]
```
Other wise the program will continue to execute and the port will not be unbinded

Once clockwWall has no other clock to run the program will stop

