Lab 3.2 - Progress Notifier with Signals
========================================
The program implements `Base64` [algorithm](https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c).
The base64 algorithm was not made by me, autor [ryyst](https://stackoverflow.com/users/282635/ryyst)

The program catches for SIGINFO and SIGINT to show progress on the encoding and decoding

Plain text files are supported, for decoding the program assumes the encoding is correct, it has been tested encoding with the program and decoding with other algorithms but for simplicity use the same one for encoding decoding


Encoding
--------
```
./base64 --encode input.txt
```
Generates a file `encoded.txt` with the contents from `input.txt` encoded

Decoding
--------
```
./base64 --decode encoded.txt
```
Generates a file `decpded.txt` with the contents from `input.txt` decoded

How to test?
------------
- Get process ID
```
ps aux | grep base64
```

- Send the signals
```
kill -SIGINT <PID>
kill -SIGINFO <PID>
```

### Note for small files

Since small files might be processed very quickly, one way to test it is by running the program then inmediately press ctrl-z to stop the execution then getting pid by ps aux with grep, then using test.py to spam a signal to the program.

Example
```
./base64 --decode encoded.txt
```
    # Immediate ctrl-z
```
ps aux | grep base64
```
    python test.py pid


    fg