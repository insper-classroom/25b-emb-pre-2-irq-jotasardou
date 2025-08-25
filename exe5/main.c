#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
volatile int btn_flag = 0;

uint32_t start_time = 0; // guarda tempo do FALL

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) { 
    btn_flag = 1;
  } else if (events & GPIO_IRQ_EDGE_RISE) { 
    btn_flag = 2;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if(btn_flag == 1){
      // FALL → guarda tempo
      start_time = to_ms_since_boot(get_absolute_time());
      btn_flag = 0;

    } else if (btn_flag == 2) {
        // RISE → calcula duração
        uint32_t end_time = to_ms_since_boot(get_absolute_time());
        uint32_t duration = end_time - start_time;

        if (duration >= 800) {
            printf("Aperto longo!\n");
        } else {
            printf("Aperto curto!\n");
        }

        btn_flag = 0;
    }

  }
}
