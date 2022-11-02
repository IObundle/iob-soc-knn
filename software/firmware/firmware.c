#include <stdlib.h>

#include "system.h"
#include "periphs.h"
#include "iob-uart.h"
#include "iob-timer.h"
#include "printf.h"

#include "iob-knn-sw.h"

int main()
{
  // init uart
  uart_init(UART_BASE,FREQ/BAUD);

  // init timer
  timer_init(TIMER_BASE);

  uart_puts("\n\n\nHello world!\n\n\n");
  printf("Value of Pi = %f\n\n", 3.1415);

  knn();

  uart_finish();
}
