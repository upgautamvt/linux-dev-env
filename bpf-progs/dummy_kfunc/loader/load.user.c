//
// Created by upgautam on 5/27/25.
//

// loader.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>

static volatile bool exiting = false;

static void handle_sigint(int sig)
{
    exiting = true;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <bpf-object-file> <prog-name>\n", argv[0]);
        return 1;
    }

    const char *bpf_file = argv[1];
    const char *prog_name = argv[2];

    struct bpf_object *obj = NULL;
    struct bpf_program *prog = NULL;
    struct bpf_link *link = NULL;

    libbpf_set_strict_mode(LIBBPF_STRICT_ALL);
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    // Open object
    obj = bpf_object__open_file(bpf_file, NULL);
    if (!obj) {
        fprintf(stderr, "Failed to open BPF object file '%s'\n", bpf_file);
        return 1;
    }

    // Load object (verifies & loads programs + maps)
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        bpf_object__close(obj);
        return 1;
    }

    // Find program by section/prog name
    prog = bpf_object__find_program_by_name(obj, prog_name);
    if (!prog) {
        fprintf(stderr, "Program '%s' not found in BPF object\n", prog_name);
        bpf_object__close(obj);
        return 1;
    }

    // Attach to kprobe for do_unlinkat
    link = bpf_program__attach_kprobe(prog, false, "do_unlinkat");
    if (!link) {
        fprintf(stderr, "Failed to attach kprobe to 'do_unlinkat'\n");
        bpf_object__close(obj);
        return 1;
    }

    printf("BPF program loaded and attached. Press Ctrl+C to exit.\n");

    // Wait until user interrupts
    while (!exiting) {
        sleep(1);
    }

    bpf_link__destroy(link);
    bpf_object__close(obj);
    printf("Detached and exiting.\n");
    return 0;
}
