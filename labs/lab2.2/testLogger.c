#include "logger.c"

int main() {
	infof("This is some info. %s: %d\n" , "Info", 0);
	warnf("This is an warning. %s: %d\n" , "Warning", 1);
	errorf("This is some error. %s: %d\n" , "Error", 2);
	panicf("This is a panic log. %s: %d\n" , "Panic im out", 3);
	return 0;
}
