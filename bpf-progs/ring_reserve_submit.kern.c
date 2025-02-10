#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>


char LISENSE[] SEC("license") = "Dual BSD/GPL";

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024 /* 256 KB */);
} ring_rs SEC(".maps");

struct event {
    int pid;
};


SEC("tp/syscalls/sys_enter_getcwd")
int empty(void *ctx) /* empty is bpf_prg */
{
    // Check first if space is available, and then do allocation.
    // This provides more insights and is helpful for debugging

    struct event *e;

    /* get PID and TID of exiting thread/process */
    __u64 id = bpf_get_current_pid_tgid();
    __u32 pid = id >> 32;
    __u32 tid = (__u32) id;

    /* ignore thread exits */
    if (pid != tid)
        return -1;

    /* reserve sample from BPF ringbuf */

    /* reserve sample from BPF ringbuf */
    e = bpf_ringbuf_reserve(&ring_rs, sizeof(*e), 0);
    if (!e) {
        bpf_printk("Size reserve successful!\n");
        return -1;
    }

    bpf_printk("Size reserve successful!\n");
    bpf_ringbuf_submit(e, 0);

    return 0;
}


//root@fedora:~# echo 1 > /sys/kernel/debug/tracing/tracing_on
//root@fedora:~# echo 0 > /sys/kernel/debug/tracing/tracing_on

//if busy, we can kill those process(es) that are using trace_pipe
//root@fedora:~# lsof -t /sys/kernel/debug/tracing/trace_pipe | xargs -I {} kill -9 {}
//[1]+  Killed                  cat /sys/kernel/debug/tracing/trace_pipe
