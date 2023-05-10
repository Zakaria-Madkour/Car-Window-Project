# include "BoardPorting.h"


void BOARD_init(){
	PORTD_init();
	PORTE_init();
	PORTA_init();  // initialization of PORTA must come after PORTE to correctly set the on/off switch state
	PORTC_init();
}

void PORTD_init(){
	/*
	Intialize PortD for the limit switches
	PIN6 --> Driver UP
	PIN7 --> Driver DOWN
	PIN2 --> Passenger UP
	PIN3 --> Passenger DOWN
	*/
	DIO_Init(PORT_D, DRIVERUPLIMIT, IN);
	DIO_Init(PORT_D, DRIVERDOWNLIMIT, IN);
	DIO_Init(PORT_D, PASSENGERUPLIMIT, IN);
	DIO_Init(PORT_D, PASSENGERDOWNLIMIT, IN);
	
	
	//Configuration of the interrupts of PORTD
	//Here we use negative edge triggering mechanism due to pull up resistance
	GPIO_PORTD_IS_R &= 0x00;   // Clear the 4 LSB for edge triggering
	GPIO_PORTD_IBE_R &= 0x00;  // Disable both edge trigger
	GPIO_PORTD_IEV_R &= 0x00;  // Set interrupt at falling edge for pins 2,3,6,7
	GPIO_PORTD_IM_R |= 0xcc;   // Enable the interrupt from the port to reach the microcontroller
	GPIO_PORTD_ICR_R |= 0xcc;  // Clear the interrupt flags of pins 2,3,6,7
	
	//Allow PortD to raise interupts from the NVIC controller
	NVIC_EN0_R |= (1<<3);
	//Set the priority of PortD interrupt
	NVIC_PRI0_R |= 7U<<29; // Set PortD priority to 7 --> Bits(29-31)
}

void PORTA_init(){
	/*
	Intialize PortA for the Driver pannel
	PIN2 --> Driver UP
	PIN3 --> Driver DOWN
	PIN5 --> On/Off button
	*/
	DIO_Init(PORT_A, DRIVERUPBUTTON, IN);
	DIO_Init(PORT_A, DRIVERDOWNBUTTON, IN);
	DIO_Init(PORT_A, ON_OFFSWITCH, IN);
	
	
	//Configuration of the interrupts of PORTA
	//Here we use negative edge triggering mechanism due to pull up resistance
	GPIO_PORTA_IS_R &= 0x00;   // Clear the 8 LSB for edge triggering
	GPIO_PORTA_IBE_R |= 0x20;  // Disable both edge trigger for pins 2,3 and enable it for pin 5
	GPIO_PORTA_IBE_R &= 0x20;
	GPIO_PORTA_IEV_R &= 0x00;  // Set interrupt at falling edge
	GPIO_PORTA_IM_R |= 0x2C;   // Enable the interrupt from the port to reach the microcontroller
	GPIO_PORTA_ICR_R |= 0x2C;  // Clear the interrupt flags of pins 2,3,5
	
	//Allow PortA to raise interupts from the NVIC controller
	NVIC_EN0_R |= (1<<0);
	//Set the priority of PortA interrupt
	NVIC_PRI0_R |= 7U<<5; // Set PortA priority to 7 --> Bits(5-7)
	
	
	// For correctly initializing the state of the on/off switch
	if((GPIO_PORTA_DATA_R & (1<<ON_OFFSWITCH))== 0){// Enable the passengers control
			on_switch();
		}
		else{//Disable the passengers control
			off_switch();
		}	
}

bool driver_up_pressed(){
	if ((GPIO_PORTA_DATA_R & (1<<DRIVERUPBUTTON))== 0){
		return true;
	}
	else{
		return false;
	}
}

bool driver_down_pressed(){
	if ((GPIO_PORTA_DATA_R & (1<<DRIVERDOWNBUTTON))== 0){
		return true;
	}
	else{
		return false;
	}
}

void PORTE_init(){
	/*
	Intialize PortE for the Driver/Passenger pannels (passenger controls)
	PIN1 --> PASSENGERUP_Driver Pannel
	PIN2 --> PASSENGERDOWN_Driver Pannel
	PIN4 --> PASSENGERUP_Passenger Pannel
	PIN5 --> PASSENGERDOWN_Passenger Pannel
	*/
	DIO_Init(PORT_E, PASSENGERUP_DP, IN);
	DIO_Init(PORT_E, PASSENGERDOWN_DP, IN);
	DIO_Init(PORT_E, PASSENGERUP_PP, IN);
	DIO_Init(PORT_E, PASSENGERDOWN_PP, IN);
	
	//Configuration of the interrupts of PORTE
	//Here we use negative edge triggering mechanism due to pull up resistance
	GPIO_PORTE_IS_R &= 0x00;   // Clear the 8 LSB for edge triggering
	GPIO_PORTE_IBE_R &= 0x00;  // Disable both edge trigger for pins 1,2,4,5
	GPIO_PORTE_IEV_R &= 0x00;  // Set interrupt at falling edge
	GPIO_PORTE_IM_R |= 0x36;   // Enable the interrupt from the port to reach the microcontroller
	GPIO_PORTE_ICR_R |= 0x36;  // Clear the interrupt flags of pins 1,2,4,5

	//Allow PortE to raise interupts from the NVIC controller
	NVIC_EN0_R |= (1<<4);
	//Set the priority of PortE interrupt
	NVIC_PRI1_R |= 7U<<5; // Set PortE priority to 7 --> Bits(5-7)
}


bool passenger_up_pressed(){
	if (((GPIO_PORTE_DATA_R & (1<<PASSENGERUP_DP))== 0) || ((GPIO_PORTE_DATA_R & (1<<PASSENGERUP_PP))== 0)){
		return true;
	}
	else{
		return false;
	}
}

bool passenger_down_pressed(){
	if (((GPIO_PORTE_DATA_R & (1<<PASSENGERDOWN_DP))== 0) || ((GPIO_PORTE_DATA_R & (1<<PASSENGERDOWN_PP))== 0)){
		return true;
	}
	else{
		return false;
	}
}
void off_switch(){
	// Disable the interrupts from PORTE
	GPIO_PORTE_ICR_R |= 0x36;  // Clear the interrupt flags of pins 1,2,4,5
	GPIO_PORTE_IM_R = 0x00;   // Disable the interrupt from the port to reach the microcontroller
}
void on_switch(){
	// Enable the interrupts from PORTE
	GPIO_PORTE_ICR_R |= 0x36;  // Clear the interrupt flags of pins 1,2,4,5
	GPIO_PORTE_IM_R |= 0x36;   // Enable the interrupt from the port to reach the microcontroller
}

void PORTC_init(){
	/*
	Intialize PortC for the Jam sensors
	PIN4 --> Driver Jam
	PIN5 --> Passenger Jam
	*/
	DIO_Init(PORT_C, DRIVERJAM, IN);
	DIO_Init(PORT_C, PASSENGERJAM, IN);
	
	//Configuration of the interrupts of PORTC
	//Here we use negative edge triggering mechanism due to pull up resistance
	GPIO_PORTC_IS_R &= 0x00;   // Clear the 8 LSB for edge triggering
	GPIO_PORTC_IBE_R &= 0x00;  // Disable both edge trigger for pins 1,2,4,5
	GPIO_PORTC_IEV_R &= 0x00;  // Set interrupt at falling edge
	GPIO_PORTC_IM_R |= 0x30;   // Enable the interrupt from the port to reach the microcontroller
	GPIO_PORTC_ICR_R |= 0x30;  // Clear the interrupt flags of pins 1,2,4,5
	
	//Allow PortC to raise interupts from the NVIC controller
	NVIC_EN0_R |= (1<<2);
	//Set the priority of PortC interrupt
	NVIC_PRI0_R |= 7U<<21; // Set PortC priority to 7 --> Bits(21-23)
}





//-------------------------------------  ISR  ---------------------------------------------------------------
void GPIOD_Handler(){
	//ISR of limit switches port
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	ACTION action_taken;
	
	if((GPIO_PORTD_MIS_R & (1<<DRIVERUPLIMIT))!=0){//PD6 --> DriverUp limit
		
		action_taken = DRIVER_STOP;
		xSemaphoreGiveFromISR( limit_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( limit_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTD_ICR_R |= (1<<DRIVERUPLIMIT);  // Clear the interrupt flags of pin 6
	}
	else if((GPIO_PORTD_MIS_R & (1<<DRIVERDOWNLIMIT))!=0){//PD7 --> DriverDown limit
		
		action_taken = DRIVER_STOP;
		xSemaphoreGiveFromISR( limit_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( limit_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTD_ICR_R |= (1<<DRIVERDOWNLIMIT);  // Clear the interrupt flags of pin 7
		
	}
	else if((GPIO_PORTD_MIS_R & (1<<PASSENGERUPLIMIT))!=0){//PD2 --> PassengerUp limit
		
		action_taken = PASSENGER_STOP;
		xSemaphoreGiveFromISR( limit_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( limit_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTD_ICR_R |= (1<<PASSENGERUPLIMIT);  // Clear the interrupt flags of pin 2
		
	}
	else if((GPIO_PORTD_MIS_R & (1<<PASSENGERDOWNLIMIT))!=0){//PD3 --> PassengerDown limit
		
		action_taken = PASSENGER_STOP;
		xSemaphoreGiveFromISR( limit_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( limit_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTD_ICR_R |= (1<<PASSENGERDOWNLIMIT);  // Clear the interrupt flags of pin 3
		
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}



void GPIOA_Handler(){
//ISR of the driver pannel -> driver window controls and On/Off switch
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	ACTION action_taken;
	
	if((GPIO_PORTA_MIS_R & (1<<DRIVERUPBUTTON))!=0){//PA2 --> DriverUp Pressed
		
		action_taken = DRIVER_UP;
		xSemaphoreGiveFromISR( driver_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( driver_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTA_ICR_R |= (1<<DRIVERUPBUTTON);  // Clear the interrupt flags of pin 2
		
	}
	else if((GPIO_PORTA_MIS_R & (1<<DRIVERDOWNBUTTON))!=0){//PA3 --> DriverDown Pressed
		
		action_taken = DRIVER_DOWN;
		xSemaphoreGiveFromISR( driver_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( driver_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTA_ICR_R |= (1<<DRIVERDOWNBUTTON);  // Clear the interrupt flags of pin 3
		
	}
	else if((GPIO_PORTA_MIS_R & (1<<ON_OFFSWITCH))!=0){//PA5 --> On/Off state changed
		for(int i=0; i<1000;i++){
			__asm(" nop"); // Provide delay to allow for the signal to be written in the data register
		}
		if((GPIO_PORTA_DATA_R & (1<<ON_OFFSWITCH))== 0){// Enable the passengers control
			on_switch();
		}
		else{//Disable the passengers control
			off_switch();
		}	
		GPIO_PORTA_ICR_R |= (1<<ON_OFFSWITCH);  // Clear the interrupt flags of pin 5
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}



void GPIOE_Handler(){
	//ISR of the driver/passenger pannel -> passenger window controls
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	ACTION action_taken;
	
	if(((GPIO_PORTE_MIS_R & (1<<PASSENGERUP_DP))!=0) || ((GPIO_PORTE_MIS_R & (1<<PASSENGERUP_PP))!=0)){//PE1,4 --> Passenger Up Pressed
		
		action_taken = PASSENGER_UP;
		xSemaphoreGiveFromISR( passenger_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( passenger_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTE_ICR_R |= (1<<PASSENGERUP_DP)|(1<<PASSENGERUP_PP);  // Clear the interrupt flags of pin 1,4
		
	}
	else if(((GPIO_PORTE_MIS_R & (1<<PASSENGERDOWN_DP))!=0) || ((GPIO_PORTE_MIS_R & (1<<PASSENGERDOWN_PP))!=0)){//PE2,5 --> Passenger Down Pressed
		
		action_taken = PASSENGER_DOWN;
		xSemaphoreGiveFromISR( passenger_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( passenger_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTE_ICR_R |= (1<<PASSENGERDOWN_DP)|(1<<PASSENGERDOWN_PP);  // Clear the interrupt flags of pin 2,5
		
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}



void GPIOC_Handler(){
//ISR of the Jam sensors
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	ACTION action_taken;
	
	if((GPIO_PORTC_MIS_R & (1<<DRIVERJAM))!=0){//PC4 --> Driver Jammed
		
		action_taken = DRIVER_STOP;
		xSemaphoreGiveFromISR( jam_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( jam_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTC_ICR_R |= (1<<DRIVERJAM);  // Clear the interrupt flags of pin 4
		
	}
	else if((GPIO_PORTC_MIS_R & (1<<PASSENGERJAM))!=0){//PC5 --> Passenger Jammed
		
		action_taken = PASSENGER_STOP;
		xSemaphoreGiveFromISR( jam_semaphore, &xHigherPriorityTaskWoken );
		xQueueSendToBackFromISR( jam_queue, &action_taken, &xHigherPriorityTaskWoken );
		
		GPIO_PORTC_ICR_R |= (1<<PASSENGERJAM);  // Clear the interrupt flags of pin 5
		
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
