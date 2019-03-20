#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/inotify.h>
#include <limits.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,*/
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include "logger.h"
#define BUF_SIZE 0xFFFFFF
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))


//Directory data
char **dirs;
int dir_count;
int inotifyFd;
int wd;
char *tree;

static int
store_walk(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    
    if (tflag == FTW_D | tflag == FTW_DP | tflag == FTW_DNR){
        //printf("%s\n", fpath);
        *(dirs + dir_count) = (char *) calloc(BUF_SIZE, sizeof(char));
        strcpy(*(dirs + dir_count), fpath);
        dir_count++;
    }
    return 0;
}

static void
reload_watch_list() {
    int limit = 20;
    int j;
    dirs = (char **) calloc(BUF_SIZE, sizeof(char *));
    dir_count = 0;
    if (nftw(tree, store_walk, limit, 0)
            == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    /* For each command-line argument, add a watch for all events */
    for (j = 0; j < dir_count; j++) {
        wd = inotify_add_watch(inotifyFd, *(dirs + j), IN_ALL_EVENTS);
        if (wd == -1) {
            errorf("Cannot add watch to %s\n", *(dirs + j));
            //exit(-1);
        }

        //printf("Watching %s using wd %d\n", *(dirs + j), wd);
    }
}

static void             /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i)
{
    if (i->mask & IN_CREATE | i->mask & IN_DELETE | i->mask & IN_DELETE_SELF | i->mask & IN_MOVED_FROM | i->mask & IN_MOVED_TO) {
        
        infof("    wd =%2d; ", i->wd);
        if (i->cookie > 0) printf("cookie =%4d; ", i->cookie);

        //if (i->mask & IN_ACCESS)        infof("IN_ACCESS ");
        //if (i->mask & IN_ATTRIB)        infof("IN_ATTRIB ");
        //if (i->mask & IN_CLOSE_NOWRITE) infof("IN_CLOSE_NOWRITE ");
        //if (i->mask & IN_CLOSE_WRITE)   infof("IN_CLOSE_WRITE ");
        if (i->mask & IN_CREATE)        infof("IN_CREATE ");
        if (i->mask & IN_DELETE)        infof("IN_DELETE ");
        if (i->mask & IN_DELETE_SELF)   infof("IN_DELETE_SELF ");
        //if (i->mask & IN_IGNORED)       infof("IN_IGNORED ");
        //if (i->mask & IN_ISDIR)         infof("IN_ISDIR ");
        //if (i->mask & IN_MODIFY)        infof("IN_MODIFY ");
        //if (i->mask & IN_MOVE_SELF)     infof("IN_MOVE_SELF ");
        if (i->mask & IN_MOVED_FROM)    infof("IN_MOVED_FROM ");
        if (i->mask & IN_MOVED_TO)      infof("IN_MOVED_TO ");
        //if (i->mask & IN_OPEN)          infof("IN_OPEN ");
        //if (i->mask & IN_Q_OVERFLOW)    infof("IN_Q_OVERFLOW ");
        //if (i->mask & IN_UNMOUNT)       infof("IN_UNMOUNT ");
        infof("\n");

        if (i->len > 0)
            infof("        name = %s\n", i->name);
        warnf("Reloading Tree\n");
        reload_watch_list(); 
    }
    
}

int
main(int argc, char *argv[])
{

    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    


    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        panicf("Usage: %s pathname...\n", argv[0]);
        exit(-1);
    }

    tree = (char *) calloc(BUF_SIZE, sizeof(char));
    strcpy(tree, argv[1]);

    inotifyFd = inotify_init();                 /* Create inotify instance */
    if (inotifyFd == -1) {
        panicf("Cannot create inotify instance\n");
        exit(-1);
    } else {
        infof("\nWatching tree of: %s\n", argv[1]);
    }
    
    reload_watch_list();

    for (;;) {                                  /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            panicf("read() from inotify fd returned 0!");

        if (numRead == -1) errorf("read");

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
//		printf("%s Debug\n", p);
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    exit(EXIT_SUCCESS);
}
