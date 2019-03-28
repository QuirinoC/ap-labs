#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

#define SIZE 0xffffff
#define PID_MAX 32768

typedef struct {
    char *pid;
    char *ppid;
    char *name;
    char *process_state;
    char *memory;
    int thread_count;
    int open_files;
} process;

//Get every pid from /proc
char **get_pids(int *size) {
    char **pids = (char **) calloc(SIZE, sizeof(char *));
    int pid_count = 0;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("/proc")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {
            if (isdigit(ent->d_name[0])) {
                *(pids + pid_count++) = ent->d_name;
            }
        }
        //Use the memory needed
        pids = realloc(pids, pid_count);
        closedir(dir);
    }
    else {
        /* could not open directory */
        *size = -1;
    }
    *size = pid_count;
    return pids;
}

void build_path(char *pid, char *data) {
    strcat(data, "/proc/");
    strcat(data, pid);
    strcat(data, "/status");
}

void *read_pid(char *pid, char *data) {
    char *path = (char *) calloc(30, sizeof(char));
    build_path(pid, path);
    //printf("%s\n", path);
    

    //Read process info
    int fd = open(path, O_RDONLY);
    //Free path from memory
    free(path);

    //Data for pid
    process *pid_data = malloc(sizeof(process));
    
    
    char *c = (char *) calloc(1, sizeof(char));
    char *buffer = (char *) calloc(50, sizeof(char));
    char *value = malloc(50 * sizeof(char));

    int buffer_size = 0;
    int value_size = 0;

    printf("__________________\n");
    while(read(fd, c, 1) > 0){

        if (*c == ':') {
            while (read(fd, c, 1) > 0) {

                if (*c == ' ') {
                    continue;
                }

                *(value + value_size++) = *c;
                if (*c == ' ') {
                    printf("space");
                }
                if (*c == '\n') {
                    *(buffer + buffer_size++) = '\0';
                    *(value + value_size++) = '\0';
                    buffer_size = 0;
                    value_size = 0;
                    break;
                }
            }
            printf("%s:%s\n", buffer, value);
        } 

        *(buffer + buffer_size++) = *c;

    }
    free(c);
    free(buffer);
    //Free current buffer
    //free(buffer);
    return "";
}

void process_table(process *table, char **pids, int pid_count) {
    int i;
    char *status;
    char *data = "";

    for (i = 0; i < pid_count; i++) {
        status = read_pid(pids[i], data);
    }
}

void clear();

int main()
{
    // Place your magic here
    //clear();
    int pid_count;
    char **pids = get_pids(&pid_count);
    
    //Allocate as many procesess we have
    process *table = malloc(PID_MAX * sizeof(pid_count));

    process_table(table, pids, pid_count);
    return 0;
}

void clear()
{
    printf("\e[1;1H\e[2J");
}
