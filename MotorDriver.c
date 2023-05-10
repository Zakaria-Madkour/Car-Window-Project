# include "MotorDriver.h"
void motor_module_init(){
	port_init();
	config_PWM_MOTOR();
}

void port_init(){
	//initialize PORT B
	DIO_Init(PORT_USED, MOTOR1_UP, OUT);
	DIO_Init(PORT_USED, MOTOR1_DOWN, OUT);
	DIO_Init(PORT_USED, MOTOR2_UP, OUT);
	DIO_Init(PORT_USED, MOTOR2_DOWN, OUT);
}

void motor1_up(){
	DIO_WritePin( PORT_USED, MOTOR1_DOWN, 0);
	DIO_WritePin( PORT_USED, MOTOR1_UP, 1);	
}
void motor1_down(){
	DIO_WritePin( PORT_USED, MOTOR1_UP, 0);
	DIO_WritePin( PORT_USED, MOTOR1_DOWN, 1);	
}
void motor1_off(){
	DIO_WritePin( PORT_USED, MOTOR1_DOWN, 0);
	DIO_WritePin( PORT_USED, MOTOR1_UP, 0);	
}
void motor2_up(){
	DIO_WritePin( PORT_USED, MOTOR2_DOWN, 0);
	DIO_WritePin( PORT_USED, MOTOR2_UP, 1);	
}
void motor2_down(){
	DIO_WritePin( PORT_USED, MOTOR2_UP, 0);
	DIO_WritePin( PORT_USED, MOTOR2_DOWN, 1);
}
void motor2_off(){
	DIO_WritePin( PORT_USED, MOTOR2_DOWN, 0);
	DIO_WritePin( PORT_USED, MOTOR2_UP, 0);	
}

void config_PWM_MOTOR(){
	/* Clock setting for PWM and GPIO PORT */
				SYSCTL_RCGCPWM_R |= 1U<<0;       /* Enable clock to PWM0 module */
				//__asm(" nop");
				//__asm(" nop");
				//write a while loop to wait for clock gating
				while( (SYSCTL_PRPWM_R & (1U<<0)) == 0);
				
	
				SYSCTL_RCGCGPIO_R |= 0x02;   /* Enable system clock to PORTB */
				//write a while loop to wait for clock gating
				while( (SYSCTL_RCGCGPIO_R & 0x00000002) ==0){}
					
				//--------------------------------------------------------------------------------------
				//SYSCTL->RCC &= ~0x00100000; /* Directly feed clock to PWM0 module without pre-divider */
				
				/* bits 17-19 as prescaler
				000 --2  ---> 8MHz
				001 --4  ---> 4MHz 
				010 --8  ---> 2MHz
				100 --16 ---> 1MHz
				101 --32 ---> 50kMHz
				111 --64 ---> 250kHz*/
				SYSCTL_RCC_R |= 0x00100000;  /*Divide the system clock feed to the PWM module */
				SYSCTL_RCC_R |= 1<<18;       /*set the PWM frequency to 2Mhz */
				//----------------------------------------------------------------------------------------

	/* Setting of PB6 PB7 pin for M0PWM0 M0PWM1 channel output pin */
				GPIO_PORTB_AFSEL_R |= 0xC0;     /* PB6 PB7 sets a alternate function */
				GPIO_PORTB_PCTL_R |= 0x44000000; /* make PB6 PB7 PWM output pin */
				GPIO_PORTB_DEN_R |= 0xC0;       /* set PB6 PB7 as a digital pin */


	/* PWM0 channel 0,1 setting */
				PWM0_0_CTL_R &= ~(1<<0);   /* Disable Generator 0 counter */
				PWM0_0_CTL_R &= ~(1<<1);   /* select down count mode of counter 0*/
				
				//--------------------- Calculations -------------------------------------------
				/*
				PWM0 Module Clock Period = 1/2Mhz = 0.5µs
				M0PWM0 Timer Period = 1/50Hz = 20ms  ----> REQUIRED for your application
				PWM0LOAD Register value = 20ms / 0.5µs = 40,000
				*/
				PWM0_0_LOAD_R = 40000;   /* set load value for 50 Hz claculation is above*
				
				/*
				Calculate the duty cycle in down mode
				PWMxCMPx = (100% - required duty cycle in %) * PWMxLOAD
				*/
				PWM0_0_CMPA_R = 20000;  /* set duty cyle to 10% PWM0CMPA */
				PWM0_0_CMPB_R = 20000;  /* set duty cyle to 10% PWM0CMPB */
				//-----------------------------------------------------------------------------
				
				PWM0_0_GENA_R = (1<<2)|(1<<3)|(1<<7);  /* Set PWM A output when counter reloaded and clear when matches PWMCMPA */
				PWM0_0_GENB_R = (1<<2)|(1<<3)|(1<<11); /* Set PWM B output when counter reloaded and clear when matches PWMCMPB */
				
				PWM0_0_CTL_R |= (1<<0);         /* Enable Generator 0 counter */
				PWM0_ENABLE_R = 0x03;            /* Enable PWM0 channels 0,1 output */
				
}


void test_motor(){
	motor_module_init();
	
	motor1_up();
	motor1_off();
	motor1_down();
	motor1_off();
	
	
	motor2_up();
	motor2_off();
	motor2_down();
	motor2_off();
}
