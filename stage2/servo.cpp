#include "servo.h"

// Pin Table
const struct pin pinTab[NUM_SERVO] = {
	{&DDRA, &PORTA, &PINA, 0},
	{&DDRA, &PORTA, &PINA, 2},
	{&DDRA, &PORTA, &PINA, 4},
	{&DDRA, &PORTA, &PINA, 6},
	{&DDRC, &PORTC, &PINC, 2},
};

// Between any 2 duties must be at least 10 LBM!
const u16 pwmDutyTab[NUM_SERVO] = {
	302,
	295,
	278,
	128,
	128
};

struct ActiveChann_TypeDef ActiveChann = {{0, 0, 0, 0, 0}, 0, NUM_SERVO + 1};
	
void Servo(enum ServoName s, enum ServoAction a){
	switch(a){
	case Init:
		// Set proper pins as outputs
		*pinTab[(u8)s].DDR |= 1 << pinTab[(u8)s].p;
		PinOut((u8)s, LOW);
	    break;
	case EnablePWM:
		PWM_AddChannel((u8)s);	
	    break;
	}
}

void PWM_AddChannel(u8 channel){
	u8 i, j;
	
	// Search for the first element not less than pwmDutyTab[channel]
	for( i = 0; 
	     i < ActiveChann.Num && pwmDutyTab[ActiveChann.Tab[i]] < pwmDutyTab[channel]; 
		 ++i );
	
	// Move all element not less than
	// pwmDutyTab[channel] up for 1 index.
	// After add new element in the formed gap.
	asm("cli\n");
    for( j = ActiveChann.Num; j > i; --j){
		ActiveChann.Tab[j] = ActiveChann.Tab[j-1];
	}
	ActiveChann.Tab[j] = channel;
	asm("sei\n");
	
	++ActiveChann.Num;
}

void PinOut(u8 channel, u8 out){
	if(out){
		*pinTab[channel].PORT |= 1 << pinTab[channel].p;
	}else{
		*pinTab[channel].PORT &= ~(1 << pinTab[channel].p);
	}
}