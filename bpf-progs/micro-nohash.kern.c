#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

__u32 idx = 0;

struct sys_enter_newfstatat {
    unsigned short common_type;          // offset: 0, size: 2, signed: 0
    unsigned char common_flags;          // offset: 2, size: 1, signed: 0
    unsigned char common_preempt_count;  // offset: 3, size: 1, signed: 0
    int common_pid;                      // offset: 4, size: 4, signed: 1

    int __syscall_nr;                    // offset: 8, size: 4, signed: 1
    int padding1;                        // padding to align next field to 8 bytes

    __u64 dfd;                             // offset: 16, size: 8, signed: 0
    const char *filename;                // offset: 24, size: 8, signed: 0
    struct stat *statbuf;                // offset: 32, size: 8, signed: 0
    __u64 flag;                            // offset: 40, size: 8, signed: 0
};

struct event {
    const char *filename;
    struct stat *statbuf;
    __u64 dfd;
};


struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} rb SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, struct sys_enter_newfstatat);
    __uint(max_entries, 256);
} ar SEC(".maps");

/**
 * Counter program that traces the number of times this
 * hookpoint has been hit
 */
SEC("tp/syscalls/sys_enter_newfstatat")
int array(struct sys_enter_newfstatat * ctx)
{
   
    if (!ctx) return 0;

    struct event *e;

    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (!e) return 0;

    e->filename = ctx->filename;
    e->statbuf = ctx->statbuf;
    e->dfd = ctx->dfd;

    bpf_ringbuf_submit(e, 0);

    return 0;
}
