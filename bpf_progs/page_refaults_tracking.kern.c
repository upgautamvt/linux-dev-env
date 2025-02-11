#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#define MAX_ENTRIES 1024
#define REFRESH_WINDOW_NS 5000000000 // 5 seconds (in nanoseconds)

struct page_info {
    __u64 refaults;  // number of refaults for a page
    __u64 evictions; // number of evictions for a page
    __u64 eviction_timestamp; // timestamp of last eviction
};

struct {
      __uint(type, BPF_MAP_TYPE_HASH);
      __uint(max_entries, MAX_ENTRIES);
      __type(key, __u32);
      __type(value, struct page_info);
} page_stats SEC(".maps");

SEC("tracepoint/mm/page_fault")
int on_page_fault(struct trace_event_raw_mm_page_fault *ctx)
{
    __u64 page_id = ctx->address;  // Using the address as the unique page identifier
    struct page_info *info;
    __u64 current_time = bpf_ktime_get_ns();  // Current time in nanoseconds

    // Lookup the page entry from the map
    info = bpf_map_lookup_elem(&page_stats, &page_id);
    if (!info) {
        // If this is the first time, initialize the struct
        struct page_info new_info = {0};
        bpf_map_update_elem(&page_stats, &page_id, &new_info, BPF_ANY);
        info = &new_info;
    }

    // If page was evicted recently, consider it a refault
    if (info->eviction_timestamp > 0 &&
        current_time - info->eviction_timestamp <= REFRESH_WINDOW_NS) {
        info->refaults += 1;  // Count it as a refault
    }

    return 0;
}

SEC("tracepoint/mm/page_evict")
int on_page_evict(struct trace_event_raw_mm_page_evict *ctx)
{
    __u64 page_id = ctx->address;
    struct page_info *info;
    __u64 current_time = bpf_ktime_get_ns();  // Current time in nanoseconds

    // Lookup the page entry from the map
    info = bpf_map_lookup_elem(&page_stats, &page_id);
    if (info) {
        // Record the eviction timestamp when a page is evicted
        info->eviction_timestamp = current_time;
        info->evictions += 1;  // Increment the eviction count
    }

    return 0;
}

SEC("prog")
int count_page_refault_ratio(void *ctx)
{
    __u64 total_refaults = 0, total_evictions = 0;
    struct page_info *info;
    __u64 key = 0;

    // Iterate over the map and sum the refaults and evictions
    while (bpf_map_get_next_key(&page_stats, &key)) {
        info = bpf_map_lookup_elem(&page_stats, &key);
        if (info) {
            total_refaults += info->refaults;
            total_evictions += info->evictions;
        }
    }

    // Calculate page refault ratio (avoid division by zero)
    if (total_evictions > 0) {
        __u64 refault_ratio = total_refaults * 100 / total_evictions;
        bpf_printk("Page refault ratio: %llu%%\n", refault_ratio);
    }

    return 0;
}

char _license[] SEC("license") = "GPL";