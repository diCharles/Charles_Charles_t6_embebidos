/*
 * PIT.c
 *
 *  Created on: 03/03/2019
 *      Author: Charles
 */

#include "PIT.h"
#include "MK64F12.h"
#include "Bits.h"
#include "GPIO.h"

void PIT0_IRQHandler(void)// ISR para el pit0
{
	GPIO_toogle_pin(GPIO_B, bit_21);
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;//writing one claers interrput
	uint32_t dummyRead;
	dummyRead = PIT->CHANNEL[0].TFLG; //read control register for clear PIT flag, this is silicon bug

}
void PIT1_IRQHandler(void)// ISR para el pit1
{

}
void PIT2_IRQHandler(void)// ISR para el pit2
{

}
void PIT3_IRQHandler(void)// ISR para el pit3
{

}




void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
	//to abort current cycle of timer, it must be dissabled and enabled again
	PIT->MCR=0x02;//disabling timer clocks
	PIT->CHANNEL[pit_timer].TCTRL&=~(0x01);//disable timer
	PIT->MCR=0;//enabling timer clocks again
	PIT->CHANNEL[pit_timer].TCTRL|=(0x01);//enable timer , ALSO SETS TIF FLAG
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;//for clearing tif flag
	uint32_t dummyRead;
	dummyRead = PIT->CHANNEL[0].TCTRL;
	float period=1/(float)(system_clock);
	PIT->CHANNEL[pit_timer].LDVAL|= (uint32_t)(delay/period)-1;//TIMER ENABLE
}

/********************************************************************************************/

void PIT_clock_gating(void)
{
	SIM->SCGC6|=0x800000;//Enabling pit clock source
	PIT->MCR=0x00;//ENABLE PIT TIMERS CLK WRITING ZERO TO MCR[MDIS], WRITE 1 DISSABLES CLCK
}

uint8_t PIT_get_interrupt_flag_status(void)
{
 uint32_t interruptFlagVal;
 interruptFlagVal= (uint8_t)PIT->CHANNEL[0].TFLG;
 if(0==interruptFlagVal)
 {
	return 0;// WHEN TFLG ONE, TIMER OVRFLW APPEARED
 }
 else
 {
	 return 1;
 }
}


void PIT_clear_interrupt_flag(void)
{
	// Para poder borrar la interrupción del PIT es necesario tener las siguientes líneas de código en la ISR:
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;//writing one claers interrput
	uint32_t dummyRead;
	dummyRead = PIT->CHANNEL[0].TCTRL; //read control register for clear PIT flag, this is silicon bug
	//writing zero has no effect
}


void PIT_enable(void)
{
	PIT->MCR=0x00;//ENABLE PIT TIMERS CLK WRITING ZERO TO MCR[MDIS], WRITE 1 DISSABLES CLCK


}

void PIT_enable_interrupt(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TFLG |= PIT_TFLG_TIF_MASK;//writing one clears interrput
	PIT->CHANNEL[pit].TCTRL|=0x01;//TIMER ENABLE
	PIT->CHANNEL[pit].TCTRL|=0x02;//TIMER INTERRUPT ENABLE ENABLE tie flag
}

