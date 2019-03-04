/**
	\file
	\brief
		This is a starter file to implement a function able to produce an accurate delay
		using the PIT module.

	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	17/09/2018
 */

#include "MK64F12.h"
#include "PIT.h"
#include "NVIC.h"
#include "GPIO.h"
#include "Bits.h"
#include "fsl_debug_console.h"
#define SYSTEM_CLOCK (21000000U)
//#define DELAY (0.01F)//10 mS
//#define DELAY (0.0000002F)
#define DELAY (1)


int main(void)
{
	uint8_t pit_inter_status = FALSE;
	//pin de salida a osciloscopio
	gpio_pin_control_register_t pin_control_register_bit_d_0 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_D);
	GPIO_pin_control_register(GPIO_D, bit_0, &pin_control_register_bit_d_0);
	GPIO_data_direction_pin(GPIO_D,GPIO_OUTPUT, bit_0);
	GPIO_set_pin(GPIO_D, bit_0);

	//led rojo para el debug
	gpio_pin_control_register_t pin_control_register_bit_b22 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_B);
	GPIO_pin_control_register(GPIO_B, bit_22, &pin_control_register_bit_b22);
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_22);
	GPIO_clear_pin(GPIO_B,bit_22);
	GPIO_set_pin(GPIO_B, bit_22);



	//habilitando el pit_0
	PIT_clock_gating();
	PIT_enable();

    //habilitando las interrupciones para el pit_0
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_10);
	NVIC_global_enable_interrupts;//equal to macro sei() in AVRS :)

	//valor de retardo para las interrupciones periodicas del pit_0
	PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY);

	for(;;) {

		do
		{
			//printf("pit 0 count %i/n",PIT->CHANNEL[0].CVAL);
			pit_inter_status =  PIT_get_interrupt_flag_status();
		}
		while(FALSE == pit_inter_status);


		GPIO_toogle_pin(GPIO_D, bit_0);
		GPIO_toogle_pin(GPIO_B, bit_22);
		PIT_clear_interrupt_flag();//porque limpiar la bandera de interrupcion???

	}

	return 0;
}
