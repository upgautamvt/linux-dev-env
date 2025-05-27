//
// Created by upgautam on 5/27/25.
//

/* SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause) */
#define BPF_NO_GLOBAL_DATA
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

typedef unsigned int u32;
typedef long long s64;

/* Declare the external kfunc */
extern int bpf_strstr(const char *str, u32 str__sz, const char *substr, u32 substr__sz) __ksym;

char LICENSE[] SEC("license") = "Dual BSD/GPL";

//executes when kernel executes  unlinkat() syscall
//userspace program: unlinkat(AT_FDCWD, "/tmp/foo.txt", 0); triggers this
SEC("kprobe/do_unlinkat")
int handle_kprobe(struct pt_regs *ctx)
{
    __u32 pid = bpf_get_current_pid_tgid() >> 32;
    char str[] = "Hello, world!";
    char substr[] = "wor";
    int result = bpf_strstr(str, sizeof(str) - 1, substr, sizeof(substr) - 1);
    if (result != -1)
    {
        bpf_printk("'%s' found in '%s' at index %d\n", substr, str, result);
    }
    bpf_printk("Hello, world! (pid: %d) bpf_strstr %d\n", pid, result);
    return 0;
}