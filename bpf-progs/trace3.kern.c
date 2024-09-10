#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

SEC("tp/syscalls/sys_enter_open")
int empty(void *ctx)
{
    __u32 pid = bpf_get_current_pid_tgid() >> 32;
    bpf_printk("PID: %u called getcwd()\n", pid);
    return 0;
}
