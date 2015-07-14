#ifndef _SERVO_H_
#define _SERVO_H_

#include "macro.h"

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#define NUM_SERVO 8
#define SERVO_PWM_PERIOD_US 20000

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
	ParachuteHatchLock,
	CanSatHatchOpening,
	Fins
};

enum ServoAction{
	Init = 0,
	EnablePWM,
	DisablePWM
};

struct ActiveChann_TypeDef{
	u8 Tab[NUM_SERVO];
	u8 Num;
	u8 State;
};

extern volatile u8 pwmCanBeChanged;

extern u16 pwmDutyTab[NUM_SERVO];
extern const struct pin pinTab[NUM_SERVO];
extern struct ActiveChann_TypeDef ActiveChann;

void Servo(enum ServoName s, enum ServoAction a);
u8 Servo_ChangeDuty(enum ServoName s, u16 newDuty);
void PWM_AddChannel(u8 channel);
void PWM_DeleteChannel(u8 channel);
void PinOut(u8 channel, u8 out);
#endif