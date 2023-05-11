# include "FreeRTOS.h"
# include "task.h"
# include "FreeRTOSConfig.h"

# include "MotorDriver.h"	
# include "BoardPorting.h"
# include "States.h"

//--------------------- Semaphores ------------------------						 
extern xSemaphoreHandle driver_semaphore = NULL;
extern xSemaphoreHandle passenger_semaphore = NULL;
extern xSemaphoreHandle limit_semaphore = NULL;
extern xSemaphoreHandle jam_semaphore = NULL;							 

//--------------------- Queues ----------------------------							 
extern xQueueHandle driver_queue = NULL;
extern xQueueHandle passenger_queue = NULL;	
extern xQueueHandle limit_queue = NULL;							 
extern xQueueHandle jam_queue = NULL;
							 
//--------------------- Mutex Locks -----------------------						 
extern xSemaphoreHandle driver_motor_mutex = NULL;
extern xSemaphoreHandle passenger_motor_mutex = NULL;	

extern xSemaphoreHandle driver_state_mutex = NULL;
extern xSemaphoreHandle passenger_state_mutex = NULL;		

//--------------------- Shared states ----------------------------							 
extern STATE driver_state = FULLY_UP;
extern STATE passenger_state = FULLY_UP;	

//--------------------- Tasks -----------------------						 
void driver_task();
void passenger_task();
void limit_task();
void jam_task();

/* Breadboard scheme
DRIVER DOWN BUTTON PA3 -> YELLOW DOWN RIGHT  
DRIVER UP BUTTON PA2 -> BROWN DOWN LEFT
JAMMING DRIVER   PC4 -> BLUE DOWN LEFT


DRIVER PASSENGER DOWN BUTTON PE2 -> YELLOW MIDDLE RIGHT  
DRIVER PASSENGER UP BUTTON PE2 -> BROWN MIDDLE LEFT
JAMMING PASSENGER   PC5 -> BLUE MIDDLE LEFT


PASSENGER DOWN P PE5 -> YELLOW UP RIGHT
PASSENGER UP   P PE4 -> BROWN UP LEFT
*/

int main(){
	// Create the required semaphores, queues, locks  ---> All the interrupts must be disabled until queues and semaphores are created	
	create_queues();
	create_semaphores();
	create_mutex();

	// Create the required tasks
	xTaskCreate( driver_task, "Driver", 240, NULL, 6, NULL );
	xTaskCreate( passenger_task, "Passenger", 240, NULL, 4, NULL );

	xTaskCreate( limit_task, "Limit", 240, NULL, 10, NULL );
	xTaskCreate( jam_task, "Jam", 240, NULL, 8, NULL );
	
	// Initialization of the used ports
	/* Note-> Board initialization must come at last because 
	if a port throws an interrupt the semaphores and queues must be already created
	*/
	
	motor_module_init();
	BOARD_init();

	// Start scheduler
	vTaskStartScheduler();
	
	// If the program is working correctly this part wont be reached
	while(1){
		__asm(" nop");
	}
	return 0;
}





void driver_task(){
	ACTION driver_action;

	while(1)
		{
			xSemaphoreTake(driver_semaphore,portMAX_DELAY);
			xQueueReceive( driver_queue, &driver_action, portMAX_DELAY);
		switch(driver_action)
		{
			case DRIVER_UP:
				if ((driver_state == IN_BETWEEN) || (driver_state == FULLY_DOWN) || (driver_state == MOVING_DOWN) || (driver_state == AUTOMATIC_DOWN))
				{
					if (driver_up_pressed())
					{// delay to give time for one touch
						vTaskDelay(100/portTICK_RATE_MS);
					
					if (!driver_up_pressed())
					{	// auto up
						xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
						driver_state = AUTOMATIC_UP;
						xSemaphoreGive(driver_state_mutex);
						xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
						motor1_up();
						xSemaphoreGive(driver_motor_mutex);
						break;
					}
					
					else 
						{ // manual up
							xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
							driver_state = MOVING_UP;
							xSemaphoreGive(driver_state_mutex);
							xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
							motor1_up();
							xSemaphoreGive(driver_motor_mutex);
						
						
							while(driver_up_pressed());
						
							if ( driver_state==MOVING_UP)
							{
							xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
							motor1_off();
							xSemaphoreGive(driver_motor_mutex);
							xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
							driver_state = IN_BETWEEN;
							xSemaphoreGive(driver_state_mutex);
							break;
							}
						}
					}	
			}	
			case DRIVER_DOWN:
				if ((driver_state == IN_BETWEEN) || (driver_state == FULLY_UP) || (driver_state == MOVING_UP) || (driver_state == AUTOMATIC_UP))
				{
					if (driver_down_pressed())
					{// delay to give time for one touch
						vTaskDelay(100/portTICK_RATE_MS);
					
					if (!driver_down_pressed())
					{	// auto down
						xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
						driver_state = AUTOMATIC_DOWN;
						xSemaphoreGive(driver_state_mutex);
						xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
						motor1_down();
						xSemaphoreGive(driver_motor_mutex);
						break;
					}
					
					else 
						{ // manual down
							xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
							driver_state = MOVING_DOWN;
							xSemaphoreGive(driver_state_mutex);
							xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
							motor1_down();
							xSemaphoreGive(driver_motor_mutex);
						
						
							while(driver_down_pressed());
						
							if ( driver_state==MOVING_DOWN)
							{
							xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
							motor1_off();
							xSemaphoreGive(driver_motor_mutex);
							xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
							driver_state = IN_BETWEEN;
							xSemaphoreGive(driver_state_mutex);
							break;
							}
						}
					}		
			}	
	}	
}
}

void passenger_task(){
	
	ACTION passenger_action;
	while(1)
		{
			xSemaphoreTake(passenger_semaphore,portMAX_DELAY);
			xQueueReceive( passenger_queue, &passenger_action, portMAX_DELAY);
		switch(passenger_action)
		{
			case PASSENGER_UP:
				if ((passenger_state == IN_BETWEEN) || (passenger_state == FULLY_DOWN) || (passenger_state == MOVING_DOWN) || (passenger_state == AUTOMATIC_DOWN))
				{
					if (passenger_up_pressed())
					{// delay to give time for one touch
						vTaskDelay(100/portTICK_RATE_MS);
					
					if (!passenger_up_pressed())
					{	// auto up
						xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
						passenger_state = AUTOMATIC_UP;
						xSemaphoreGive(passenger_state_mutex);
						xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
						motor2_up();
						xSemaphoreGive(passenger_motor_mutex);
						break;
					}
				
					
					else 
						{ // manual up
							xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
							passenger_state = MOVING_UP;
							xSemaphoreGive(passenger_state_mutex);
							xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
							motor2_up();
							xSemaphoreGive(passenger_motor_mutex);
						
						
							while(passenger_up_pressed());
						
							if ( passenger_state==MOVING_UP)
							{
							xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
							motor2_off();
							xSemaphoreGive(passenger_motor_mutex);
							xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
							passenger_state = IN_BETWEEN;
							xSemaphoreGive(passenger_state_mutex);
							break;
							}
						}
					}	
				}
	
			case PASSENGER_DOWN:
				if ((passenger_state == IN_BETWEEN) || (passenger_state == FULLY_UP) || (passenger_state == MOVING_UP) || (passenger_state == AUTOMATIC_UP))
				{
					if (passenger_down_pressed())
					{// delay to give time for one touch
						vTaskDelay(100/portTICK_RATE_MS);
					
					if (!passenger_down_pressed())
					{	// auto down
						xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
						passenger_state = AUTOMATIC_DOWN;
						xSemaphoreGive(passenger_state_mutex);
						xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
						motor2_down();
						xSemaphoreGive(passenger_motor_mutex);
						break;
					}
					
					else 
						{ // manual down
							xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
							passenger_state = MOVING_DOWN;
							xSemaphoreGive(passenger_state_mutex);
							xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
							motor2_down();
							xSemaphoreGive(passenger_motor_mutex);
						
						
							while(passenger_down_pressed());
						
							if ( passenger_state==MOVING_DOWN)
							{
							xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
							motor2_off();
							xSemaphoreGive(passenger_motor_mutex);
							xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
							passenger_state = IN_BETWEEN;
							xSemaphoreGive(passenger_state_mutex);
							break;
							}
						}
					}		
			}	
	}	
}
}

void limit_task(){
	
	ACTION limit_action;

	while(1)
		{
			xSemaphoreTake(limit_semaphore,portMAX_DELAY);
			xQueueReceive( limit_queue, &limit_action, portMAX_DELAY);
		switch(limit_action)
		{
			case DRIVER_STOP:
				if ((driver_state == MOVING_UP) || (driver_state == AUTOMATIC_UP))
				{
					xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
					motor1_off();
					xSemaphoreGive(driver_motor_mutex);
					xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
					driver_state=FULLY_UP;
					xSemaphoreGive(driver_state_mutex);				
				}
				else if ((driver_state == MOVING_DOWN) || (driver_state == AUTOMATIC_DOWN))
				{
					xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
					motor1_off();
					xSemaphoreGive(driver_motor_mutex);
					xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
					driver_state=FULLY_DOWN;
					xSemaphoreGive(driver_state_mutex);				
				}

				
				
			case PASSENGER_STOP:
				if ((passenger_state == MOVING_UP) || (passenger_state == AUTOMATIC_UP))
				{
					xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
					motor2_off();
					xSemaphoreGive(passenger_motor_mutex);
					xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
					passenger_state=FULLY_UP;
					xSemaphoreGive(passenger_state_mutex);				
				}
				else if ((passenger_state == MOVING_DOWN) || (passenger_state == AUTOMATIC_DOWN))
				{
					xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
					motor2_off();
					xSemaphoreGive(passenger_motor_mutex);
					xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
					passenger_state=FULLY_DOWN;
					xSemaphoreGive(passenger_state_mutex);				
				}

			}
		}
	}

void jam_task(){
	
	ACTION jam_action;

	while(1)
		{
				xSemaphoreTake(jam_semaphore,portMAX_DELAY);
				xQueueReceive( jam_queue, &jam_action, portMAX_DELAY);
		switch(jam_action)
		{
			case DRIVER_STOP:
				if (driver_state == AUTOMATIC_UP)
				{
					xSemaphoreTake(driver_state_mutex,portMAX_DELAY);
					driver_state=AUTOMATIC_DOWN;
					xSemaphoreGive(driver_state_mutex);	
					xSemaphoreTake(driver_motor_mutex,portMAX_DELAY);
					motor1_down();
					xSemaphoreGive(driver_motor_mutex);
					vTaskDelay(500/portTICK_RATE_MS);
					motor1_off();
					if (driver_state == AUTOMATIC_DOWN)
					{
						driver_state = IN_BETWEEN;
					}
					break;
				}
				
			case PASSENGER_STOP:
				if (passenger_state == AUTOMATIC_UP)
				{
					xSemaphoreTake(passenger_state_mutex,portMAX_DELAY);
					passenger_state=AUTOMATIC_DOWN;
					xSemaphoreGive(passenger_state_mutex);	
					xSemaphoreTake(passenger_motor_mutex,portMAX_DELAY);
					motor2_down();
					xSemaphoreGive(passenger_motor_mutex);
					vTaskDelay(500/portTICK_RATE_MS);
					motor2_off();
					if (passenger_state == AUTOMATIC_DOWN)
					{
						passenger_state = IN_BETWEEN;
					}
					break;
				}
			}
		}
	}
		
