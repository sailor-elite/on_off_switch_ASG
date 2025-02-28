#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 1200000UL
#define DelayTimeOn 10000
#define DelayTimeOff 1000
#define RelayPin PB3

void Setup_Power_Save()
{
    ADCSRA &= ~(1<<ADEN); //Disable ADC
    ACSR = (1<<ACD); //Disable the analog comparator
    DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins.
    WDTCR |= (1<<WDP3 )|(0<<WDP2 )|(0<<WDP1)|(1<<WDP0); // Enable watchdog timer interrupts
    DDRB = 0x1F; // Set all pins as outputs
}


// Timer based delay
void delay_ms(uint16_t ms)
{
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01);
    OCR0A = 149;
    
    uint16_t i;
    for (i = 0; i < ms; i++) {
        TCNT0 = 0;
        while (!(TIFR0 & (1 << OCF0A)));
        TIFR0 |= (1 << OCF0A);
    }

    TCCR0B = 0;
}


// Function to switch Auto Stop & Go
void SwitchASG()
{
    PORTB &= ~_BV(RelayPin);
    delay_ms(DelayTimeOn);
    PORTB |= _BV(RelayPin);
    delay_ms(DelayTimeOff);
    PORTB &= ~_BV(RelayPin);
}

void Enter_Low_Power_Mode()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set as Power - Down
    cli(); // Disable Interrupts
    sleep_enable(); // Enable sleepmode 
    sleep_cpu(); // Enable cpu sleep
}

int main(void)
{
    Setup_Power_Save();
    SwitchASG();
    Enter_Low_Power_Mode();

    while (1);
    return 0;
}
