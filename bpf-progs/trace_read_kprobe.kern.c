#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/ksys_read")
int trace_kprobe()
{
    return 0;
}
