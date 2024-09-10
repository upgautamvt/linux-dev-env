#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

struct signal_generate_ctx {
    char pad[8];
    int sig;
    int errno;
    int code;
    char comm[16];
    int pid;
    int group;
    int result;
};

SEC("tp/signal/signal_generate")
int trace_signal(struct signal_generate_ctx *ctx)
{
    __u32 pid = bpf_get_current_pid_tgid() >> 32;
    //if (pid != 0) return 0; // in-ebpf
    if (ctx == NULL)
        return 0;

    bpf_printk("Generated signal %d\n", ctx->sig);
    return 0;
}
