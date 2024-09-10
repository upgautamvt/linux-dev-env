#include <linux/module.h>   // Required for all kernel modules
#include <linux/kernel.h>   // Needed for KERN_INFO
#include <linux/init.h>     // Needed for the macros

// Function executed when module is loaded
static int __init hello_init(void) {
    printk(KERN_INFO "Module loaded. Current PID: %d\n", current->pid);
    return 0; // Non-zero return means that the module couldn't be loaded.
}

// Function executed when module is unloaded
static void __exit hello_exit(void) {
    printk(KERN_INFO "Module unloaded. Current PID: %d\n", current->pid);
    }

// Macros to define module entry and exit points
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");           // License type
MODULE_AUTHOR("Ram Mude");      // Author name
MODULE_DESCRIPTION("A Hello World Kernel Module");  // Description
MODULE_VERSION("1.0");           // Module version
