#include "esp_output.h"
#include "driver/gpio.h"

void output_create(int pin)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;//cau hinh chan output
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1ULL<<pin;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void output_set_level(int pin, int level)
{
    gpio_set_level(pin, level);
}

void output_toggle(int pin)
{
    output_set_level(pin, !gpio_get_level(pin));
}