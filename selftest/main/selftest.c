#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


#define BLINK_GPIO 2

static uint8_t s_led_state = 0;


// Forward declaration of the funtion.
esp_err_t my_gpio_set_level(gpio_num_t gpio_num, uint32_t level);






static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    my_gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    printf("Example configured to blink GPIO LED!\n");
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








/**
 * Sets the specified gpio_in_pin as an input pin and the gpio_out_pin as output
 * and then sets the value on the output pin to be level and returns the value
 * that is read from the input pin. The value on the input pin should be the
 * same value on the output pin if the two pins are wired together.
 */
int test_pin_in_out(gpio_num_t gpio_in_pin, gpio_num_t gpio_out_pin, uint32_t level) {
    gpio_set_direction(gpio_in_pin, GPIO_MODE_INPUT);
    gpio_set_direction(gpio_out_pin, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(gpio_in_pin, GPIO_FLOATING);
    gpio_set_pull_mode(gpio_out_pin, GPIO_FLOATING);
    
    my_gpio_set_level(gpio_out_pin, level);
    vTaskDelay(pdMS_TO_TICKS(10));
    return gpio_get_level(gpio_in_pin);
}


/**
 * @brief  GPIO set output level. This is a reimplementation of the provided `gpio_set_level` function.
 *
 * @note This function is allowed to be executed when Cache is disabled within ISR context, by enabling `CONFIG_GPIO_CTRL_FUNC_IN_IRAM`
 *
 * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
 * @param  level Output level. 0: low ; 1: high
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG GPIO number error
 *
 */
esp_err_t my_gpio_set_level(gpio_num_t gpio_num, uint32_t level) {
    if (!GPIO_IS_VALID_GPIO(gpio_num)) {
        return ESP_ERR_INVALID_ARG;
    }
    
    const uintptr_t GPIO_BASE = (uintptr_t)0x60004000;
    if (level == 0) {
        // set the bit in the clear register, either GPIO_OUT_W1TC_REG
        // or GPIO_OUT1_W1TC_REG, depending on whether the gpio_num
        // is between 0 and 31 or 32 to 48. Those have offsets of
        // 0xC and 0x18 respectively. The offsets are relative to
        // the GPIO base address of 0x6000_4000.
        volatile uint32_t *w1tc;
        
        if (gpio_num < 32) {
           w1tc = (volatile uint32_t *)(GPIO_BASE + 0xC);
        } else {
            w1tc = (volatile uint32_t *)(GPIO_BASE + 0x18);
        }
        
        printf("my_gpio_set_level:: w1tc = %p, gpio_num = %u\n", (void *)w1tc, gpio_num);
        *w1tc = (1 << gpio_num);
    } else {
        // set the bit in the set register, GPIO_OUT_W1TS_REG
        volatile uint32_t *w1ts;
        
        if (gpio_num < 32) {
            w1ts = (volatile uint32_t *)(GPIO_BASE + 0x8);
        } else {
            w1ts = (volatile uint32_t *)(GPIO_BASE + 0x14);
        }
        
        printf("my_gpio_set_level:: w1ts = %p\n, gpio_num = %u\n", (void *)w1ts, gpio_num);
        *w1ts = (1 << gpio_num);
    }
    
    return ESP_OK;
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
    
    int in_out_pins[] = {
        GPIO_NUM_41,
        GPIO_NUM_42
    };
    int in_out_pin_count = sizeof(in_out_pins) / sizeof(in_out_pins[0]);
    
    /* Configure the peripheral according to the LED type */
    configure_led();

    int counter = 0;
    while (1) {
        printf("Turning the LED %s!\n", s_led_state == true ? "ON" : "OFF");

        // passively drive each pin via its pull-up or pull-down resistor
        for (int x=0; x<pin_count; x++) {
            int pin = pins[x];
            up = test_pin_pull_mode(pin, GPIO_PULLUP_ONLY);
            down = test_pin_pull_mode(pin, GPIO_PULLDOWN_ONLY);
            printf("  test_pull:: pin %d: up=%d, down=%d: success=%s\n", pin, up, down, up == 1 && down == 0 ? "YES" : "NO");
        }
        
        // actively drive pins that are connected such that the output pin
        // drives the value of the input pin
        for (int x=0; x<in_out_pin_count; x++) {
            for (int y=0; y<in_out_pin_count; y++) {
                if (x != y) {
                    int in_pin = in_out_pins[x];
                    int out_pin = in_out_pins[y];
                    for (uint32_t level = 0; level <= 1; level++) {
                        int actual_level = test_pin_in_out(in_pin, out_pin, level);
                        printf("  test_in_out:: in=%d, out=%d: expected=%lu, actual=%d: success=%s\n", in_pin, out_pin, level, actual_level, level == actual_level ? "YES" : "NO");
                    }
                }
            }
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
