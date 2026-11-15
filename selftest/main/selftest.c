#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


#define BLINK_GPIO 2

static uint8_t s_led_state = 0;


static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    printf("Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}



int test_pin_pull_mode(gpio_num_t gpio_num, gpio_pull_mode_t pull) {
    // make the pin an input (this comes from gpio.h)
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
    
    
    // switch on the chip's internal pull-up or pull-down resistor
    // this comes from gpio.h
    gpio_set_pull_mode(gpio_num, pull);
    

    // wait a few milliseconds
    // The macro pdMS_TO_TICKS( value_in_ms ) can be used to convert a time
    // specified in milliseconds to a time specified in ticks. This is defined
    // in freertos/task.h, along with the vTaskDelay function.
    vTaskDelay(pdMS_TO_TICKS(10));
    
    
    // read the pin (gpio.h)
    return gpio_get_level(gpio_num);
}

void app_main(void)
{
    int up = 0;
    int down = 0;
    int pins[] = {
        GPIO_NUM_1,
        GPIO_NUM_14,
        GPIO_NUM_21,
        GPIO_NUM_41,
        GPIO_NUM_42,
        GPIO_NUM_47
    };
    int pin_count = sizeof(pins) / sizeof(pins[0]);
    
    /* Configure the peripheral according to the LED type */
    configure_led();

    int counter = 0;
    while (1) {
        printf("Turning the LED %s!\n", s_led_state == true ? "ON" : "OFF");

        for (int x=0; x<pin_count; x++) {
            int pin = pins[x];
            up = test_pin_pull_mode(pin, GPIO_PULLUP_ONLY);
            down = test_pin_pull_mode(pin, GPIO_PULLDOWN_ONLY);
            printf("  pin %d: up=%d, down=%d: success=%s\n", pin, up, down, up == 1 && down == 0 ? "YES" : "NO");
        }
        
        blink_led();
        /* Toggle the LED state */
        if (counter == 0 || counter == 2 || counter == 3) {
            s_led_state = 1;
        } else {
            s_led_state = 0;
        }
        
        // one second delay
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        counter = (counter + 1) % 5;
    }
}
