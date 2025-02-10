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

SEC("tp/net/net_dev_xmit")
int trace_net(struct signal_generate_ctx *ctx)
{
    return 0;
}
