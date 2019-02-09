#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define stdin 0
#define stdout 1

void filecopy(int ifd, int ofd)
{
    char *c = (char *) calloc(2048, sizeof(char)); 
    int nbytes;
    while ((nbytes =  read(ifd, c ,2048)) > 0) {
        write(ofd, c, 2048);
    }
}

int main(int argc, char *argv[]){
    char ERR_OPEN[] = "Cannot open file\n";
    int fd;
    char *prog = argv[0];   /* program name for errors */
    
    if (argc == 1) {
        filecopy(stdin, stdout);
    } else {
        while (--argc > 0) {
            if ((fd = open(*++argv, O_RDONLY)) < 0) {

                write(stdout, ERR_OPEN, sizeof(ERR_OPEN));

                exit(-1);
            } else {
                filecopy(fd, stdout);
                close(fd);
            }
        }
    }

    return 0;
}