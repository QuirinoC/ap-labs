#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include "logger.h"

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

//Output destination 0 for stdout | 1 for syslog
int dest = 0;


int initLogger(char *logType) {
    //Set output
    if (strlen(logType) == 0 | strcmp(logType, "stdout") == 0) {
        dest = 0;
    } else if (strcmp(logType, "syslog") == 0) {
        dest = 1;
    }

    printf("Initializing Logger on: %s\n", logType);
    return 0;
}
void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	if (dest == 0) {
		textcolor(BRIGHT, WHITE, BLACK);
		done = vfprintf (stdout, format, arg);
		textcolor(RESET, WHITE, BLACK);	
		va_end (arg);
	} else {
		openlog ("Logger-INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_INFO, format, arg);
		closelog();
	}
	
	return done;
}
int warnf(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	if (dest == 0) {
		textcolor(BRIGHT, YELLOW, BLACK);
		done = vfprintf (stdout, format, arg);
		textcolor(RESET, WHITE, BLACK);	
		
	} else if(dest == 1) {
		openlog ("Logger-WARN", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_WARNING, format, arg);
		closelog();
	}
	va_end (arg);
	return done;
}
int errorf(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	if (dest == 0) {
		textcolor(BRIGHT, RED, BLACK);
		done = vfprintf (stdout, format, arg);
		textcolor(RESET, WHITE, BLACK);	
	
	} else if(dest == 1) {
		openlog ("Logger-ERROR", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_ERR, format, arg);
		closelog();
	}
	va_end (arg);
	return done;
}
int panicf(const char *format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	if (dest == 0) {
		textcolor(BRIGHT, RED, WHITE);
		done = vfprintf (stdout, format, arg);
		textcolor(RESET, WHITE, BLACK);	
	} else if(dest == 1) {
		openlog ("Logger-PANIC", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_ERR, format, arg);
		closelog();
	}
	va_end (arg);
	//abort();
	return done;
}