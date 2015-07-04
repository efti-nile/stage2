#include "main.h"

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

int main(void)
{
    /*Servo(CanSat1, Init);
    Servo(CanSat2, Init);
    Servo(CanSat3, Init);
    Servo(Parachute, Init);
    Servo(CanSatsHatchLock, Init);
    Servo(ParachuteHatchLock, Init);*/
	
	ATmega8_16bitTimer();
	
	/*Servo(CanSat1, EnablePWM);
	Servo(CanSat2, EnablePWM);
	Servo(CanSat3, EnablePWM);
	Servo(Parachute, EnablePWM);
	Servo(CanSatsHatchLock, EnablePWM);
	Servo(ParachuteHatchLock, EnablePWM);*/
    
	while(1);
}