#ifndef _SERVO_H_
#define _SERVO_H_

#include "macro.h"

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#define NUM_SERVO 6
#define SERVO_PWM_PERIOD_US 1300

#define HIGH 1
#define LOW  0

// Structure for pin table
struct pin{
	volatile u8 *DDR;
	volatile u8 *PORT;
	volatile u8 *PIN;
	u8 p;
};

enum ServoName{
	CanSat1 = 0,
	CanSat2,
	CanSat3,
	Parachute,
	CanSatsHatchLock,
	ParachuteHatchLock
};

enum ServoAction{
	Init = 0,
	EnablePWM
};

struct ActiveChann_TypeDef{
	u8 Tab[NUM_SERVO];
	u8 Num;
	u8 State;
};

extern const u16 pwmDutyTab[NUM_SERVO];
extern const struct pin pinTab[NUM_SERVO];
extern struct ActiveChann_TypeDef ActiveChann;

void Servo(enum ServoName s, enum ServoAction a);
void PWM_AddChannel(u8 channel);
u8 PinOut(u8 channel, u8 out);
#endif