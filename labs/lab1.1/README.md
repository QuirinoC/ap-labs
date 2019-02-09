# mycat

Build binaries

    make

Remove binaries

    make clean

Run cat or mycat

    ./[cat-mycat] 
    ./[cat-mycat] file1.txt file2 in.txt

Run benchmark

    python3 benchmark.py

This Python program will create multiple files with diferent sizes (16^[1-6]) of characters get the difference between running cat and mycat with the same file, and get an averge of the overall performance,
when it seems that the program has stopped it is actualy being stop since the file is being created

Example output:

    [(0.004081010818481445, 0.004010915756225586),
    (0.004216670989990234, 0.003932952880859375),
    (0.003947019577026367, 0.0038919448852539062),
    (0.038697004318237305, 0.050965070724487305),
    (0.9030590057373047, 1.0169708728790283),
    (12.673371076583862, 15.18569278717041)]
    
    mycat has a speed up of x over cat