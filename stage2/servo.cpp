#include "servo.h"

// Pin Table
const struct pin pinTab[NUM_SERVO] = {
	{&DDRA, &PORTA, &PINA, 0},
	{&DDRA, &PORTA, &PINA, 1},
	{&DDRA, &PORTA, &PINA, 2},
	{&DDRC, &PORTC, &PINC, 0},
	{&DDRC, &PORTC, &PINC, 1},
	{&DDRC, &PORTC, &PINC, 2},
	{&DDRE, &PORTE, &PINE, 1},
	{&DDRB, &PORTB, &PINB, 1}
};

// Between any 2 duties must be at least 10 LBM!
// In C-SPACE 2015 PCB PWM channels are inverted!
u16 pwmDutyTab[NUM_SERVO] = {
	19010,
	18990,
	18000,
	19020,
	18500,
	18690,
	2100,
	2000
};

struct ActiveChann_TypeDef ActiveChann = {{0, 0, 0, 0, 0, 0, 0, 0}, 0, NUM_SERVO + 1};
	
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
	case DisablePWM:
		PWM_DeleteChannel((u8)s);
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

void PWM_DeleteChannel(u8 channel){
	u8 i;
	while(!pwmCanBeChanged);
	asm("cli\n");
	for(i = 0; i < ActiveChann.Num; i++){
		if(ActiveChann.Tab[i] == channel){
			for(i++; i < ActiveChann.Num; i++){
				ActiveChann.Tab[i-1] = ActiveChann.Tab[i];
			}
			ActiveChann.Num--;
			break;
		}
	}
	asm("sei\n");
}

u8 Servo_ChangeDuty(enum ServoName s, u16 newDuty){
	u8 i;
	if(newDuty < 10 || newDuty > SERVO_PWM_PERIOD_US - 10){
		return 1;
	};
	while(!pwmCanBeChanged);
	asm("cli\n");
	for(i = 0; i < ActiveChann.Num; i++){
		if(ActiveChann.Tab[i] == (u8)s){
			for(i++; i < ActiveChann.Num; i++){
				ActiveChann.Tab[i-1] = ActiveChann.Tab[i];
			}
			ActiveChann.Num--;
			break;
		}
	}
	pwmDutyTab[(u8)s] = newDuty;
	asm("sei\n");
	PWM_AddChannel((u8)s);
	return 0;
}

void PinOut(u8 channel, u8 out){
	if(out){
		*pinTab[channel].PORT |= 1 << pinTab[channel].p;
	}else{
		*pinTab[channel].PORT &= ~(1 << pinTab[channel].p);
	}
}