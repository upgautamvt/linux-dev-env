/*
 * Trace program that dumps to a ringbuf
 */
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} ring SEC(".maps");


SEC("tp/syscalls/sys_enter_getcwd")
int empty(void *ctx)
{
    __u32 pid = bpf_get_current_pid_tgid() >> 32;
    if (bpf_ringbuf_output(&ring, &pid, sizeof(__u32), 0)) {
        bpf_printk("Ringbuf output success!\n");
    } else {
        bpf_printk("Ringbuf output fail\n");
    }
        
    return 0;
}
