#include "macro.h" // Contains typdefs and F_CPU definition for all project files

// toolchain
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// periphery inside MCU
// #include "USART.h"
#include "servo.h"

// Start Key
#define SKp 2
#define SK_DDR DDRD
#define SK_PIN PIND
#define SK_PORT PORTD
#define SK_PIN_INIT SK_DDR |= 1 << SKp
#define SK_PIN_LOW (!(SK_PIN & 1 << SKp))

#define INQUIRY_TIMES 300

// LEDs
#define LEDRp 0
#define LEDBp 1
#define LED_DDR DDRB
#define LED_PORT PORTB

#define LEDS_INIT LED_DDR |= 1<<LEDRp|1<<LEDBp; LED_PORT &= ~(1<<LEDRp|1<<LEDBp)
#define LEDR_ON LED_PORT |= 1<<LEDRp
#define LEDB_ON LED_PORT |= 1<<LEDBp
#define LEDR_OFF LED_PORT &= ~(1<<LEDRp)
#define LEDB_OFF LED_PORT &= ~(1<<LEDBp)
#define LEDR_INV LED_PORT ^= 1<<LEDRp

#define BOOSTER_DELAY 1000
#define CANSAT_DELAY 1000
#define PARACHUTE_DELAY 1000

// Local protoypes
ISR(USART_RXC_vect);

