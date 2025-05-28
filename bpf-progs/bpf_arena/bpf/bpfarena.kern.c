//
// Created by upgautam on 5/28/25.
//

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>

extern void *bpf_arena_alloc_pages(void *map, void *addr, u32 page_cnt, int node, u64 flags) __ksym;

// 4 GB arena = 4 * 1024 * 1024 * 1024 bytes
#define GB (1024ULL * 1024 * 1024) //ULL means unsigned long long
struct {
    __uint(type, BPF_MAP_TYPE_ARENA);
    __uint(max_entries, 4 * GB);  // 4 GB
} arena SEC(".maps");

SEC("kprobe.s/do_unlinkat")
int BPF_KPROBE(do_unlinkat_probe, int dfd, struct filename *name)
{
    // Allocate 1 page (4 KB) from arena
    void *mem = bpf_arena_alloc_pages(&arena, NULL, 1, 0, 0);
    if (!mem) return 0;

    *(u64*)mem = bpf_ktime_get_ns();
    bpf_printk("Allocated page at %p", mem);
    return 0;
}

char _license[] SEC("license") = "GPL";