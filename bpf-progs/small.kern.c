#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

SEC("tp/syscalls/sys_enter_getcwd")
int empty(void *ctx)
{
    int pid = bpf_get_current_pid_tgid();
    bpf_printk("pid %d\n", pid);
    return 0;
}
