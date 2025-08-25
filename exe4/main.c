#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>


const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

const int BTN_PIN_G = 26;
const int LED_PIN_G = 6;

volatile int btn_flag_r = 0;
volatile int btn_flag_g = 1;

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) { 
    btn_flag_r = 1;
  }
  if (events & GPIO_IRQ_EDGE_RISE) {
    btn_flag_g = 0;
  }
}

void pin_init(void){
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);
  gpio_put(LED_PIN_R, 0);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);
  gpio_put(LED_PIN_G, 0);
}


int main() {
  stdio_init_all();

  pin_init();

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);  
   gpio_set_irq_enabled_with_callback(
      BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true, &btn_callback); 

  int led_state_r = 0;
  int led_state_g = 1;

  while (true) {
    if(btn_flag_r == 1){
      // printf("on %d\n", btn_flag_r);
      btn_flag_r = 0;
      led_state_r = !led_state_r;
      gpio_put(LED_PIN_R, led_state_r);

    }
    if(btn_flag_g == 0){
      // printf("off %d\n", btn_flag_g);
      btn_flag_g = 1;
      led_state_g = !led_state_g;
      gpio_put(LED_PIN_G, led_state_g);
    }
  }
}

