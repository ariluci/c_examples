/**
 * Baremetal firmware example to toggle LED on and off using
 * a GPIO pin 
 */
 #include <stdint.h>

 // Define memory-mapped GPIO register addresses (specific to MCU!!)
 #define GPIO_BASE          0x40021000
 #define GPIO_MODDER        *((volatile uint32_t *)(GPIO_BASE + 0x00))
 #define GPIO_ODR           *((volatile uint32_t *)(GPIO_BASE + 0x14))
 #define LED_PIN            5 //LED is connected to pin 5

 void delay(int count) {
    while (count-- > 0);
 }

 int main() {
    // Configure GPIO pin as output
    GPIO_MODDER &= ~(0x3 << (LED_PIN * 2));   // Clear mode bits for pin
    GPIO_MODDER |=  (0x1 << (LED_PIN * 2));   // Set pin as output

    while (1) {
        // Togle a LED
        GPIO_ODR ^= (1 << LED_PIN);
        delay(100000);                        // To percieve the blink
    }
 }