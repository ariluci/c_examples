/**
 * Linux kernel driver example to toggle LED on and off using
 * a GPIO pin 
 */

 #include <linux/module.h>
 #include <linux/fs.h>
 #include <linux/gpio.h>

 #define GPIO_LED_PIN 18
 #define DEVICE_NAME "gpio_led"

/* Turn LED ON */
static void led_on(void) {
    gpio_set_value(GPIO_LED_PIN, 1); /* Set GPIO high */
}

/* Turn LED OFF */
static void led_off(void) {
    gpio_set_value(GPIO_LED_PIN, 0); /*Set GPIO low */    
}

/* File write operation to toggle the LED */
static ssize_t led_write(struct file *File, const char __user *buffer, size_t len, loff_t *offset) {
    char command;

    if (len < 1)
        return -EINVAL;

    if (copy_from_user(&command, buffer, 1))
        return -EFAULT;

    if (command == '1')
        led_on();
    else if (command == '0')
        led_off();
    else
        return -EINVAL;

    return len;
}

/* File operation structure */
static const struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write,
};

/* Module init */
static int __init ModuleInit(void) {
    int ret;

    /* Reqiest the GPIO pin */
    if (!gpio_is_valid(GPIO_LED_PIN)) {
        pr_err("Invalid GPIO pin\n");
        return -ENODEV;
    }

    ret = gpio_request(GPIO_LED_PIN, "LED");
    if (ret) {
        pr_err("Failed to request GPIO\n");
        return ret;
    }

    gpio_direction_output(GPIO_LED_PIN, 0);

    /* Register device */
    ret = register_chardev(240, DEVICE_NAME, &led_fops); /* Assing major number 240 to device */
    if(ret < 0) {
        gpio_free(GPIO_LED_PIN);
        pr_err("Failed to register character device \n");
        return ret;
    }

    pr_info("LED driver initialized\n");
    return 0;
}

/* Module cleanup */
static void __exit ModuleExit(void) {
    unregister_chrdev(240, DEVICE_NAME);
    gpio_free(GPIO_LED_PIN);
    pr_info("LED driver removed\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LA");
MODULE_DESCRIPTIOM("Toggle GPIO LED driver");