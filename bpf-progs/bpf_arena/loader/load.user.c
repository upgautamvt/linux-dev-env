//
// Created by upgautam on 5/27/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>

#define MAP_NAME "arena"
#define BPF_REL_PATH "../bpf/bpfarena.kern.o"

static volatile sig_atomic_t exiting = 0;

void handle_signal(int sig) {
    exiting = 1;
}

int main(int argc, char **argv) {
    char bpf_path[PATH_MAX];
    struct bpf_object *obj = NULL;
    struct bpf_map *map;
    int map_fd;
    void *arena_mem = MAP_FAILED;
    size_t arena_size;
    int err;

    // Setup signal handling
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // 1. Construct path
    if (realpath(BPF_REL_PATH, bpf_path) == NULL) {
        perror("realpath failed");
        return 1;
    }

    printf("Loading BPF object from: %s\n", bpf_path);

    // 2. Load BPF object
    err = bpf_prog_load(bpf_path, BPF_PROG_TYPE_UNSPEC, &obj, &map_fd);
    if (err) {
        fprintf(stderr, "Error loading BPF object: %d\n", err);
        fprintf(stderr, "Make sure:\n");
        fprintf(stderr, "1. You're running as root\n");
        fprintf(stderr, "2. Kernel >= 6.8 with BPF arena support\n");
        fprintf(stderr, "3. BPF object is compiled: clang -O2 -target bpf -c bpfarena.kern.c -o bpfarena.kern.o\n");
        return 1;
    }

    // 3. Find arena map
    map = bpf_object__find_map_by_name(obj, MAP_NAME);
    if (!map) {
        fprintf(stderr, "Arena map '%s' not found. Available maps:\n", MAP_NAME);
        struct bpf_map *m;
        bpf_object__for_each_map(m, obj) {
            fprintf(stderr, "  - %s\n", bpf_map__name(m));
        }
        goto cleanup;
    }

    map_fd = bpf_map__fd(map);
    arena_size = bpf_map__max_entries(map);

    printf("Arena size: %.2f MB (%zu pages)\n",
           (double)arena_size / (1024 * 1024),
           arena_size / 4096);

    // 4. Map arena into userspace
    arena_mem = mmap(NULL, arena_size, PROT_READ | PROT_WRITE,
                     MAP_SHARED, map_fd, 0);
    if (arena_mem == MAP_FAILED) {
        perror("mmap failed");
        fprintf(stderr, "Try increasing memory limits:\n");
        fprintf(stderr, "  ulimit -l unlimited\n");
        fprintf(stderr, "  sysctl -w net.core.bpf_map_mem_limit=%zu\n", arena_size * 2);
        goto cleanup;
    }

    printf("Mapped arena at %p\n\n", arena_mem);
    printf("Press Ctrl+C to exit\n\n");

    // 5. Interact with arena
    uint64_t *counter = arena_mem;
    while (!exiting) {
        printf("Counter value: %20lu (0x%016lx)\n", *counter, *counter);
        (*counter)++;
        sleep(1);
    }
    printf("\nExiting...\n");

cleanup:
    // Cleanup resources
    if (arena_mem != MAP_FAILED) {
        munmap(arena_mem, arena_size);
        printf("Unmapped arena memory\n");
    }

    if (obj) {
        bpf_object__close(obj);
        printf("Closed BPF object\n");
    }

    return 0;
}