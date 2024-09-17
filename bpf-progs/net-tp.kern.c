
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

SEC("tp/net/net_dev_xmit")
int empty(void *ctx)
{
    bpf_printk("Hello World\n");
    return 0;
}
