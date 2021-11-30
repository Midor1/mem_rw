//
// Created by Chuanjia Hou on 2021/11/30.
//

#include <cstdlib>
#include <cstdio>
#include <vector>

#include <sys/types.h>
#include <unistd.h>

using std::vector;

#define KB (1024)
#define MB (1024 * 1024)

pid_t pid;

int main (int argc, char **argv) {
    /*Usage: ./mem_rw <number_of_processes> <memory_per_process> <global_memory> <cpu_binding_info>*/
    if (argc < 5) {
        printf("Usage: ./mem_rw <number_of_processes> <memory_per_process> <global_memory> <cpu_binding_info>\n");
        exit(0);
    }

    unsigned proc_num = atoi(argv[1]);
    unsigned mem_per_proc = atoi(argv[2]);
    unsigned mem_global = atoi(argv[3]);

    for(int i=1;i<=proc_num;i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failure!");
            exit(1);
        }
    }
    printf("PID: %d\n", pid);
    vector<char>* v = new vector<char>(mem_per_proc * MB);
    // Sequential write.
    for (char & it : *v) {
        it = 't';
    }

}