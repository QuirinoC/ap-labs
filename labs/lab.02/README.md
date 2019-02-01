# Lab 02

## How to compile 

    #Compile str-final with
    
    make

    #Remove binary with

    make clean
    
## Usage 

Run with:

    make
    ./str-final.o [str1] [str2] [str3]

The program will check for the len of str1, print out the result of concatenating str1 and str2, and will test if str3 is inside the concatenated word str1str2.

### Note
strlen returns the size of the string including the null byte '\0'