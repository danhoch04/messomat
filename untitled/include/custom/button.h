#include <avr/interrupt.h>
#include "global.h"

void interrupt_setup(void);
void interrupt_start(void);
void interrupt_stop(void);
void interrupt_worker(void);

void interrupt_button1_pressed(void);
void interrupt_button2_pressed(void);
void interrupt_button3_pressed(void);