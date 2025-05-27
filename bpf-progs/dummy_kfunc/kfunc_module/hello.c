//
// Created by upgautam on 5/27/25.
//

#include <linux/init.h>       // Macros for module initialization
#include <linux/module.h>     // Core header for loading modules
#include <linux/kernel.h>     // Kernel logging macros
#include <linux/bpf.h>
#include <linux/btf.h>
#include <linux/btf_ids.h>

/* Declare the kfunc prototype */
__bpf_kfunc int bpf_strstr(const char *str, u32 str__sz, const char *substr, u32 substr__sz);

/* Begin kfunc definitions */
__bpf_kfunc_start_defs();

/* Define the bpf_strstr kfunc */
// __bpf_kfunc annotation adds BTF metadata kfunc bpf_strstr in vmlinux
__bpf_kfunc int bpf_strstr(const char *str, u32 str__sz, const char *substr, u32 substr__sz)
{
    // Edge case: if substr is empty, return 0 (assuming empty string is found at the start)
    if (substr__sz == 0)
    {
        return 0;
    }
    // Edge case: if the substring is longer than the main string, it's impossible to find
    if (substr__sz > str__sz)
    {
        return -1; // Return -1 to indicate not found
    }
    // Iterate through the main string, considering the size limit
    for (size_t i = 0; i <= str__sz - substr__sz; i++)
    {
        size_t j = 0;
        // Compare the substring with the current position in the string
        while (j < substr__sz && str[i + j] == substr[j])
        {
            j++;
        }
        // If the entire substring was found
        if (j == substr__sz)
        {
            return i; // Return the index of the first match
        }
    }
    // Return -1 if the substring is not found
    return -1;
}

/* End kfunc definitions */
__bpf_kfunc_end_defs();

/* Define the BTF kfuncs ID set */
BTF_KFUNCS_START(bpf_kfunc_example_ids_set)
BTF_ID_FLAGS(func, bpf_strstr)
BTF_KFUNCS_END(bpf_kfunc_example_ids_set)

/* Register the kfunc ID set */
static const struct btf_kfunc_id_set bpf_kfunc_example_set = {
    .owner = THIS_MODULE,
    .set = &bpf_kfunc_example_ids_set,
};

/* Function executed when the module is loaded */
static int __init hello_init(void)
{
    int ret;

    printk(KERN_INFO "Hello, world!\n");
    /* Register the BTF kfunc ID set for BPF_PROG_TYPE_KPROBE */
    ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_KPROBE, &bpf_kfunc_example_set);
    if (ret)
    {
        pr_err("bpf_kfunc_example: Failed to register BTF kfunc ID set\n");
        return ret;
    }
    printk(KERN_INFO "bpf_kfunc_example: Module loaded successfully\n");
    return 0; // Return 0 if successful
}

/* Function executed when the module is removed */
static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye, world!\n");
}

/* Macros to define the module’s init and exit points */
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");                 // License type (GPL)
MODULE_AUTHOR("Your Name");            // Module author
MODULE_DESCRIPTION("A simple module"); // Module description
MODULE_VERSION("1.0");                 // Module version