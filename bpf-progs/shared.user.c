/**
 * User program for loading programs that access a shared map
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <stdlib.h>

#include <bpf/bpf.h>
#include <bpf/libbpf.h>

int main()
{

    // Open the shared1.kern object
    struct bpf_object * prog = bpf_object__open("empty.kern.o");
    
    // Try and load this program
    // This should make the map we need
    if (bpf_object__load(prog)) {
        printf("Failed");
        return 0;
    }

    struct bpf_program * s1 = bpf_object__find_program_by_name(prog, "empty");

    if (s1 == NULL) {
        printf("Shared 1 failed\n");
        return 0;
    }

    bpf_program__attach(s1);

    while (1) {
        sleep(1);
    }

    return 0;
}
