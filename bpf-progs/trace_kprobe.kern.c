#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LISENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/ksys_read+41")
int trace_kprobe()
{
    return 0;
}
