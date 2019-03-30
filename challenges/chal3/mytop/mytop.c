#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define SIZE 0xffffff
#define PID_MAX 32768
#define MAGIC 1900

FILE *output; 

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
    strcpy(data, "");
    strcat(data, "/proc/");
    strcat(data, pid);
    strcat(data, "/");
    strcat(data, folder);
    strcat(data, "\0");
}

void *read_pid(char *pid) {
    //if (atoi(pid) > 100) return "";
    char *path = malloc(30*sizeof(char));
    
    build_path(pid, path, "status");
    //printf("%s\n", path);
    char *ppid;
    char *name;
    char *process_state;
    char *memory;
    char *thread_count;
    int open_files;


    //Read process info
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        //fprintf(output, "%s\n", path);
        return "";
    }
    //Free path from memory
    free(path);

    //Data for pid

    //Data read from file
    char *c = (char *) calloc(1, sizeof(char));
    char *buffer = (char *) calloc(50, sizeof(char));
    char *value = malloc(50 * sizeof(char));

    int buffer_size = 0;
    int value_size = 0;

    //Sets if memory is set
    int mem_flag = 0;

    while(read(fd, c, 1) > 0){

        if (*c == ':') {
            while (read(fd, c, 1) > 0) {

                if (*c == ' ' | *c == '\t') {
                    continue;
                }

                if (*c != '\n') {
                    *(value + value_size++) = *c;
                }
                
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
                fprintf(output, "| %-19s |", value);
            } else if (strcmp(buffer, "State") == 0) {
                fprintf(output, " %-12s |", strcmp("S(sleeping)", value) == 0? "Sleeping" : "Active");
            } else if (strcmp(buffer, "Pid") == 0) {
                fprintf(output, " %-5s |", value);
            } else if (strcmp(buffer, "PPid") == 0) {
                fprintf(output, " %-6s |", value);
            } else if (strcmp(buffer, "VmHWM") == 0) {
                mem_flag = 1;
                fprintf(output, " %-8s |", value);
            } else if (strcmp(buffer, "Threads") == 0) {
                if (mem_flag == 0) {
                    fprintf(output," %-8s |", "0 kB");
                }
                fprintf(output, " %-8s |", value);
            }

            buffer_size = 0;
            value_size = 0; 
        }
        
        if ((*c != '\n')) {
            *(buffer + buffer_size++) = *c;
        }

    }
    
    //Set if flag not present
    if (mem_flag == 0) {
        memory = malloc(value_size * sizeof("0 kB")+ 1);
        strcpy(memory, "0 kB"); 
    }

    //Get files open
    int fd_count = 0;
    DIR *dir;
    struct dirent *ent;

    char *fd_path = malloc(30 * sizeof(char));
    build_path(pid, fd_path, "fd");

    if ((dir = opendir(fd_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL)  {
            if (ent->d_type == DT_REG) { /* If the entry is a regular file */
                fd_count++;
            } else {
                fd_count++;
            }    
        }

            
        closedir(dir);
    
    }
    free(fd_path);

    fprintf(output, " %-10d |", fd_count);
    
    //printf("| %-5s | %-6s | %-20s | %-11s | %-7s  | %-8s | %-10d |\n",
    //           pid, ppid , name, process_state, memory, thread_count, fd_count);
    

    free(c);
    free(buffer);
    free(value);
    fprintf(output, "\n");
    return "";
}

void process_table(char **pids, int pid_count) {
    int i;
    fprintf(output, "+---------------------+--------------+-------+--------+----------+----------+------------+\n");
    fprintf(output, "|       Name          |   State      | PID   |   PPID |  Memory  | #Threads | Open Files |\n");
    fprintf(output, "+---------------------+--------------+-------+--------+----------+----------+------------+\n");
    for (i = 0; i < pid_count; i++) {
        read_pid(pids[i]);
    }
    fprintf(output, "+---------------------+--------------+-------+--------+----------+----------+------------+\n");
}

void clear();
void clrscr();

void sig_handler(int sig) {
    clrscr();
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char filename[100];
    sprintf(filename, "mytop-status-%d-%d-%d.txt", tm.tm_mday, tm.tm_mon, tm.tm_year + MAGIC);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Cannot create file \n");
    }
    output = file;

    int pid_count;
    char **pids = get_pids(&pid_count);
    //char **pids = get_pids(&pid_count);
    //process_table(pids, pid_count);
    process_table(pids, pid_count);

    fclose(file);    
    output = stdout;
}



int main()
{
    signal(SIGINT, sig_handler);
    output = stdout;
    // Place your magic here
    //clear();
    int pid_count;
    char **pids = get_pids(&pid_count);
    
    //Allocate as many procesess we have
    process *table = malloc(PID_MAX * sizeof(pid_count));
    int c = 0;
    int i = 0;
    while (1) {

        pids = get_pids(&pid_count);
        clrscr();
        //for (i = 0; i < pid_count; i++) {
        //    printf("%s ", pids[i]);
        //}printf("\n");


        process_table(pids, pid_count);
        sleep(1);

    }
    
    return 0;
}

void clear()
{
   printf("\e[1;1H\e[2J");
}

#include <stdlib.h>

void clrscr()
{
    system("clear");
}