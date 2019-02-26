Lab 2.3 - Cross-referencer
==

### Build
```
make
```
### Clean
```
make clean
```
This cross-referencer will remove any special chars that are not part of a word ['!?.;,'] from each word, convert it to lower case and count each occurency of the word and keep track where was the word found

###  Run
```
 ./cross-ref [fileName]
```

### Example output

Output is given by the format 

    word [count]: {lines}

Ex.

    .
    .
    .
    adieu [1]: 582 
    abode [1]: 582 
    leave [1]: 582 
    divide [1]: 583 
    distracted [1]: 583 
    end [1]: 586 

