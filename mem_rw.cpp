//
// Created by Chuanjia Hou on 2021/11/30.
//
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <thread>
#include <sstream>

#define CONST_CHAR 4

#define KB (1024)
#define MB (1024 * 1024)

pid_t pid;


void memory_exclusive(int memory) {
    // Sleep 1秒等待亲和性配置？
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto* v = new std::vector<char>(memory * MB, CONST_CHAR);
    // 申请内存并读写
    for(uint64_t i=0; i< memory * MB; i++) {
        (*v)[i] = CONST_CHAR * (*v)[i] % 256;
    }
}

int main (int argc, char **argv) {
    /*Usage: ./mem_rw <number_of_processes> <memory_per_process> <global_memory> <cpu_binding_info>*/
    if (argc < 5) {
        std::cout << "Usage: ./mem_rw <number_of_processes> <memory_per_process> <global_memory> <cpu_binding_info>"
             << std::endl;
        exit(0);
    }

    unsigned proc_num = atoi(argv[1]);
    unsigned mem_per_proc = atoi(argv[2]);
    // TODO: global memory rw
    unsigned mem_global = atoi(argv[3]);
    // 将绑核参数转换为掩码
    auto mask = numa_parse_cpustring(argv[4])
    for (int i=1;i<=proc_num;i++) {
        // 开proc_num个线程进行处理
        std::thread t(memory_exclusive, mem_per_proc);
        // thread::id无法直接转为int类型，需要进行转换
        std::stringstream ss;
        ss << t.get_id();
        uint64_t id = std::stoull(ss.str());
        // 不确定这里的ID是否是Linux系统调用想要接受的pid_t，亲和性设置可能不成功
        if (sched_setaffinity(id, sizeof(mask), mask)) {
            std::cout << "set affinity failure, ERR:" << strerror(errno) << std::endl;
        }
        // TODO: 将线程绑定到主线程（join）
    }

    return 0;
}