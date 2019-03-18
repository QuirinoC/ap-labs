CC=gcc

#By putting the object files in the rule, make knows it must first compile the .c versions individually and then build the executable.
#Use -rdynamic flag to have access to method name

make:
	$(CC) testLogger.c -o testLogger.out logger.c -rdynamic

clean:
	rm testLogger.out