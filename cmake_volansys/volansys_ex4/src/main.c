#include "stm32f1xx_hal.h"

int main()
{
	GPIO_InitTypeDef GPIOC_Params; // Initilisation structure for GPIOC Settings
	
	uint32_t i; // Loop variable
	
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Turn on Clock of GPIOC
	
	// Configure the GPIO Pin 13 used for LEDs
	GPIOC_Params.Pin = GPIO_PIN_13; // Select pins 13
	GPIOC_Params.Mode = GPIO_MODE_OUTPUT_PP; // Set pins to push pull output mode
	GPIOC_Params.Speed = GPIO_SPEED_LOW; // Set low output speed
	HAL_GPIO_Init(GPIOC, &GPIOC_Params); // Initialise GPIOC according to parameters on GPIOC_Params
	
	while(1){
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Turn on LEDs
		
		for(i = 0; i < 2000000; i++){}; // Implement a 1 second (ish!) delay
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Turn off LEDs
			
		for(i = 0; i < 2000000; i++){}; // Implement a 1 second (ish!) delay
		
	}

	return 0;
}