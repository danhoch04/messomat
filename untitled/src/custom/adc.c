#include "custom/adc.h"

static volatile int16_t adcValue = 0;
int16_t ldrValue = 0;

void adcSetup(){
    //Setzt Referenzspannung auf AVCC
    ADMUX |= (1 << REFS0);
    //Setzt Prescaler auf 128; ADEN: aktiviert ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);

    // ADC auf Kanal 5
    ADMUX |= 5;
}

void adcStart(){
    // Startet ADC
    ADCSRA |= (1 << ADSC);
}

void adcWorker(){
    while (ADCSRA & (1 << ADSC));
    ldrValue = ADC;
}