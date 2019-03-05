#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		    1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void textcolor(int attr, int fg, int bg);
int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infor(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	textcolor(BRIGHT, WHITE, BLACK);
	done = vfprintf (stdout, format, arg);
	textcolor(RESET, WHITE, BLACK);	
	va_end (arg);
	return done;
}
int warnf(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	textcolor(BRIGHT, YELLOW, BLACK);
	done = vfprintf (stdout, format, arg);
	textcolor(RESET, WHITE, BLACK);	
	va_end (arg);
	return done;
}
int errorf(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	textcolor(BRIGHT, RED, BLACK);
	done = vfprintf (stdout, format, arg);
	textcolor(RESET, WHITE, BLACK);	
	va_end (arg);
	return done;
}
int panicf(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	textcolor(BRIGHT, RED, WHITE);
	done = vfprintf (stdout, format, arg);
	textcolor(RESET, WHITE, BLACK);	
	va_end (arg);
	//abort();
	return done;
}