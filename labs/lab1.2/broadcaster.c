#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
   
    if (argc < 2) {
	printf("No message found\n");
	exit(1);
    }
    int i;
    int msg_size = argc - 2;
    for (i = 1; i < argc; i++) {
	msg_size += strlen(argv[i]);
    }
    char *message = malloc(msg_size * sizeof(char));
    strcpy(message, argv[1]);
    for (i = 2; i < argc; i++){
	strcat(message, " ");
	strcat(message, argv[i]);
    }
    const char *path = "/dev/pts/";
    struct dirent *pDirent;

    DIR *dir = opendir(path);
    
    if (dir == NULL){
        printf("Cannot open directory");
    }
    // Skip . and .. folders 
    pDirent = readdir(dir); 
    pDirent = readdir(dir);

    int current;

    while((pDirent = readdir(dir)) != NULL){
        //Skip ptmx
	if(strcmp(pDirent->d_name, "ptmx") == 0){
            continue;
        }

	//Create full path for file
        //10 for size of "/dev/pts"
        char *fullpath = (char *) malloc(strlen(pDirent->d_name) + 10);
        strcpy(fullpath, path);
        strcat(fullpath, pDirent->d_name);
        
	if (current = open(fullpath, O_WRONLY)< 0) {
		printf("Cannot write to %s\n", pDirent->d_name);
	}
 
		
	write(current, message, msg_size);
        close(current);
    }
    closedir(dir);
    
    return 0;
}
