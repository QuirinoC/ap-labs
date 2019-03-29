#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <signal.h>

#define SIZE 0xffffff
#define PID_MAX 32768

typedef struct {
    char *pid;
    char *ppid;
    char *name;
    char *process_state;
    char *memory;
    char *thread_count;
    int open_files;
} process;

void print_process(process p) {
    //printf("PID %s Parent %s Name %s State %s",
     //       p->pid, p->ppid, p->name, p->state);
}

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
        *size = 0;
    }
    //free(dir);
    *size = pid_count;
    return pids;
}

void build_path(char *pid, char *data, char*folder) {
    strcat(data, "/proc/");
    strcat(data, pid);
    strcat(data, "/");
    strcat(data, folder);
    strcat(data, "\0");
}

void *read_pid(char *pid, char *data) {
    char *path = malloc(30*sizeof(char));
    build_path(pid, path, "status");
    //printf("%s\n", path);
    if (strcmp(pid, "697") != 0) {
    //    return "";
    }

    char *ppid;
    char *name;
    char *process_state;
    char *memory;
    char *thread_count;
    int open_files;


    //Read process info
    int fd = open(path, O_RDONLY);
    //Free path from memory
    free(path);

    //Data for pid
    process *pid_data = malloc(sizeof(process));

    //Data read from file
    char *c = (char *) calloc(1, sizeof(char));
    char *buffer = (char *) calloc(50, sizeof(char));
    char *value = malloc(50 * sizeof(char));

    int buffer_size = 0;
    int value_size = 0;
    printf("\n--------------\n");
    while(read(fd, c, 1) > 0){

        if (*c == ':') {
            while (read(fd, c, 1) > 0) {

                if (*c == ' ' | *c == '\t') {
                    continue;
                }


                *(value + value_size++) = *c;
                
                //If line jump is found make a valid string by adding null byte
                if (*c == '\n') {
                    *(buffer + buffer_size++) = '\0';
                    *(value + value_size++) = '\0';             
                    break;
                }
            }
            
            int map_val = strcmp(buffer, "Name") == 0 ? 0 : 
                          strcmp(buffer, "State") == 0 ? 1 :
                          strcmp(buffer, "Pid") == 0 ? 2 :
                          strcmp(buffer, "PPid") == 0 ? 3 :
                          strcmp(buffer, "Threads") == 0 ? 4 : 
                          -1;

            strcat(value, "\0");
            if (strcmp(buffer, "Name") == 0) {
                name = malloc(value_size * sizeof(value) + 1);
                strcpy(name, value);
            } else if (strcmp(buffer, "State") == 0) {
                process_state = malloc(value_size * sizeof(value) + 1);
                strcpy(process_state, value);
            } else if (strcmp(buffer, "Pid") == 0) {
                pid = malloc(value_size * sizeof(value) + 1);
                strcpy(pid, value);
            } else if (strcmp(buffer, "PPid") == 0) {
                ppid = malloc(value_size * sizeof(value)+ 1);
                strcpy(ppid, value);
            } else if (strcmp(buffer, "VmHWM") == 0) {
                memory = malloc(value_size * sizeof(value)+ 1);
                strcpy(memory, value); 
            } else if (strcmp(buffer, "Threads") == 0) {
                thread_count = malloc(value_size * sizeof(value)+ 1);
                strcpy(thread_count, value); 
            }

            buffer_size = 0;
            value_size = 0; 
        } 
        if (*c != '\n') {
            *(buffer + buffer_size++) = *c;
        }
        

    }
    
    //Get files open
    int fd_count = 0;
    DIR *dir;
    struct dirent *ent;

    char *fd_path = malloc(30 * sizeof(char));
    build_path(pid, fd_path, "fd");

    if ((dir = opendir(fd_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) 
            fd_count++;
        closedir(dir);
    }
    free(fd_path);
    
    printf("%s: %s\n", "Name", name);
    printf("%s: %s\n", "Pid", pid);
    printf("%s: %s\n", "PPid", ppid);
    printf("%s: %s\n", "State", process_state);
    printf("%s: %s\n", "Memory", memory);
    printf("%s: %s\n", "Threads", thread_count);
    
    free(c);
    free(buffer);
    free(value);
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
