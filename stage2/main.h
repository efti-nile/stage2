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

// Buttons pins
#define BUTTONS_INIT {DDRE &= ~0xE0; DDRG &= ~0x0F;} 

#define CS1_IS_HIGH  (PINE & 1 << 5)
#define CS2_IS_HIGH  (PINE & 1 << 6)
#define CS3_IS_HIGH  (PINE & 1 << 7)
#define P_IS_HIGH    (PING & 1 << 0)
#define CSHL_IS_HIGH (PING & 1 << 1)
#define PHL_IS_HIGH  (PING & 1 << 2)
#define CSO_IS_HIGH  (PING & 1 << 3)

#define DEBOUNCE_DELAY 10
#define DOUBLECLICK_DELAY 500

#define CS1_CLOSED 850
#define CS1_OPENED 870

#define CS2_CLOSED 890
#define CS2_OPENED 910

#define CS3_CLOSED 930
#define CS3_OPENED 950

#define P_CLOSED 970
#define P_OPENED 990

#define CSHL_CLOSED 1010
#define CSHL_OPENED 1030

#define PHL_CLOSED 1050
#define PHL_OPENED 1070

#define CSO_CLOSED 1090
#define CSO_OPENED 1110


// Local protoypes
ISR(USART_RXC_vect);

