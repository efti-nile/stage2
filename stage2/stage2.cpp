#include "main.h"

volatile u16 calvar = SERVO_PWM_PERIOD_US / 2; // This var used for actuators calibration

void ATmega8_16bitTimer(void)
{
	TCNT1 = 0xFFFF - SERVO_PWM_PERIOD_US;
	// Set timer clock to F_CPU / 8
	TCCR1B = 0x02;
	// Enable Overflow Interrupt
	TIMSK |= 1 << TOIE1;
}

// ISR to deal with software PWM
ISR(TIMER1_OVF_vect)
{
	if( ActiveChann.Num > 0 ){
		if( ActiveChann.State == NUM_SERVO + 1 ){
			for( u8 i = 0; i < ActiveChann.Num; ++i ){
				PinOut(ActiveChann.Tab[i], HIGH);
			}
			ActiveChann.State = 0;
			TCNT1 = 0xFFFF - pwmDutyTab[ActiveChann.Tab[0]];
			}else{
			PinOut(ActiveChann.Tab[ActiveChann.State], LOW);
			
			if( ActiveChann.State < ActiveChann.Num - 1 ){
				TCNT1 = 0xFFFF - ( pwmDutyTab[ActiveChann.Tab[ActiveChann.State+1]] - pwmDutyTab[ActiveChann.Tab[ActiveChann.State]] );
				++ActiveChann.State;
				}else{
				TCNT1 = 0xFFFF - ( SERVO_PWM_PERIOD_US - pwmDutyTab[ActiveChann.Tab[ActiveChann.State]] );
				ActiveChann.State = NUM_SERVO + 1;
			}
		}
		}else{
		TCNT1 = 0xFFFF - SERVO_PWM_PERIOD_US;
	}
}

u8 polling(u8 *reg, u8 bitno){
	u16 cntr = 0, dcntr;
	while(*reg & 1 << bitno){
		if(cntr++ > DEBOUNC_DELAY){
			// Wait until the button released
			dcntr = 0;
			while(dcntr++ < DOUBLECLICK_DELAY){
				cntr = 0;
				while(!(*reg & 1 << bitno)){
					if(cntr++ > DEBOUNC_DELAY){
						// Wait for the second click
						dcntr = 0;
						while(dcntr++ < DOUBLECLICK_DELAY){
							cntr = 0;
							while(*reg & 1 << bitno){
								if(cntr++ > DEBOUNC_DELAY){
									// So the second click happened
									return 2;
								}
								_delay_ms(1);
							}
							_delay_ms(1);
						}
						// So there was only one click
						return 1;
					}
					_delay_ms(1);
				}
				_delay_ms(1);
			}
		}
		_delay_ms(1);
	}
	return 0;
}

int main(void)
{
	SP = 0xFFF;
	
	/*
	// This loop used for actuators calibration
	// acn - number of an actuator to calibrate
	// calvar - duty that can be changed via JTAG
	u8 acn = 0;
	Servo(acn, Init);
	pwmDutyTab[acn] = calvar;
	Servo(acn, EnablePWM);
	ATmega8_16bitTimer();
	sei();
	while(1){
		_delay_ms(10);
		pwmDutyTab[acn] = calvar;
		_delay_ms(10);
	}*/
	
	BUTTONS_INIT; // Init buttons
	while(1){
		// CanSat1 actuator
		switch(polling(&PINE, 5)){
			case 1:
				pwmDutyTab[CanSat1] = CS1_CLOSED;
				Servo(CanSat1, EnablePWM);
				break;
			case 2:
				pwmDutyTab[CanSat1] = CS1_OPENED;
				Servo(CanSat1, EnablePWM);
				break;
				break;
		}
		// CanSat2 actuator
		switch(polling(&PINE, 6)){
			case 1:
			pwmDutyTab[CanSat2] = CS2_CLOSED;
			Servo(CanSat2, EnablePWM);
			break;
			case 2:
			pwmDutyTab[CanSat2] = CS2_OPENED;
			Servo(CanSat2, EnablePWM);
			break;
			break;
		}
		// CanSat3 actuator
		switch(polling(&PINE, 7)){
			case 1:
			pwmDutyTab[CanSat3] = CS3_CLOSED;
			Servo(CanSat3, EnablePWM);
			break;
			case 2:
			pwmDutyTab[CanSat3] = CS3_OPENED;
			Servo(CanSat3, EnablePWM);
			break;
			break;
		}
		// Parachute actuator
		switch(polling(&PING, 0)){
			case 1:
			pwmDutyTab[Parachute] = P_CLOSED;
			Servo(Parachute, EnablePWM);
			break;
			case 2:
			pwmDutyTab[Parachute] = P_OPENED;
			Servo(Parachute, EnablePWM);
			break;
			break;
		}
		// CanSat hatch lock actuator
		switch(polling(&PING, 1)){
			case 1:
			pwmDutyTab[CanSatsHatchLock] = CSHL_CLOSED;
			Servo(CanSatsHatchLock, EnablePWM);
			break;
			case 2:
			pwmDutyTab[CanSatsHatchLock] = CSHL_OPENED;
			Servo(CanSatsHatchLock, EnablePWM);
			break;
			break;
		}
		// Parachute hatch lock actuator
		switch(polling(&PING, 2)){
			case 1:
			pwmDutyTab[ParachuteHatchLock] = PHL_CLOSED;
			Servo(ParachuteHatchLock, EnablePWM);
			break;
			case 2:
			pwmDutyTab[ParachuteHatchLock] = PHL_OPENED;
			Servo(ParachuteHatchLock, EnablePWM);
			break;
			break;
		}
		// CanSat hatch opening actuator
		switch(polling(&PING, 3)){
			case 1:
			pwmDutyTab[CanSatHatchOpening] = CSO_CLOSED;
			Servo(CanSatHatchOpening, EnablePWM);
			break;
			case 2:
			pwmDutyTab[CanSatHatchOpening] = CSO_OPENED;
			Servo(CanSatHatchOpening, EnablePWM);
			break;
			break;
		}
	}
	
	
    /*Servo(CanSat1, Init);
    Servo(CanSat2, Init);
    Servo(CanSat3, Init);
    Servo(Parachute, Init);
    Servo(CanSatsHatchLock, Init);
    Servo(ParachuteHatchLock, Init);
	

	Servo(CanSat1, EnablePWM);
	Servo(CanSat2, EnablePWM);
	Servo(CanSat3, EnablePWM);
	Servo(Parachute, EnablePWM);
	Servo(CanSatsHatchLock, EnablePWM);
	Servo(ParachuteHatchLock, EnablePWM);*/
	

	
	while(1);
}